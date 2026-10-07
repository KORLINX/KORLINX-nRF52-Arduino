#!/usr/bin/env python3
"""Build the KORLINX nRF52 core release archive and register it in the index.

One archive serves both installers:

- the Arduino Board Manager, through package_korlinx_index.json
- PlatformIO, through package.json at the archive root, which the
  KORLINX-PlatformIO platform pins by URL

Usage:
    make_release.py check
    make_release.py build [--out DIR]
    make_release.py index ARCHIVE

`check` verifies that platform.txt, package.json and changelog.md agree on the
version. `build` packs the tracked files of the checkout, submodules included,
into dist/KXduino_nRF52-<version>.tar.gz and prints its checksum and size.
`index` adds that archive to package_korlinx_index.json as a new platform
release, copying the tool dependencies of the newest existing release.
"""

import argparse
import gzip
import hashlib
import io
import json
import os
import re
import subprocess
import sys
import tarfile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
INDEX = os.path.join(ROOT, "package_korlinx_index.json")
ARCHIVE_PREFIX = "KXduino_nRF52"
RELEASE_URL = "https://github.com/KORLINX/KORLINX-nRF52-Arduino/releases/download/v{version}/{name}"
SUBMODULES = ("libraries/Adafruit_TinyUSB_Arduino", "libraries/Adafruit_nRFCrypto")

# Tracked files that are repository plumbing, not part of the installed core,
# in this repository and in its submodules alike. The index is left out
# because it lists the archive that contains it.
EXCLUDE_PREFIXES = ("tools/release/",)
EXCLUDE_FILES = {"package_korlinx_index.json"}
EXCLUDE_ANY_DEPTH = {".github", ".gitignore", ".gitmodules", ".gitattributes"}


def fail(msg):
    sys.exit("error: " + msg)


def git(*args):
    return subprocess.run(
        ["git", "-C", ROOT, *args], check=True, capture_output=True, text=True
    ).stdout


def versions():
    with open(os.path.join(ROOT, "platform.txt")) as fp:
        m = re.search(r"^version=(\S+)$", fp.read(), re.M)
    platform_txt = m.group(1) if m else None
    with open(os.path.join(ROOT, "package.json")) as fp:
        package_json = json.load(fp)["version"]
    with open(os.path.join(ROOT, "changelog.md")) as fp:
        m = re.search(r"^## (\d+\.\d+\.\d+)", fp.read(), re.M)
    changelog = m.group(1) if m else None
    return {"platform.txt": platform_txt, "package.json": package_json,
            "changelog.md (newest entry)": changelog}


def checked_version():
    found = versions()
    if len(set(found.values())) != 1:
        fail("versions disagree: " + ", ".join("%s=%s" % kv for kv in found.items()))
    return found["platform.txt"]


def tracked_files():
    for sub in SUBMODULES:
        if not os.listdir(os.path.join(ROOT, sub)):
            fail("submodule %s is empty; run git submodule update --init" % sub)
    out = git("ls-files", "-z", "--recurse-submodules")
    files = []
    for path in out.split("\0"):
        if not path or path in EXCLUDE_FILES or path.startswith(EXCLUDE_PREFIXES):
            continue
        if EXCLUDE_ANY_DEPTH.intersection(path.split("/")):
            continue
        files.append(path)
    return sorted(files)


def build(out_dir):
    version = checked_version()
    if git("status", "--porcelain", "--untracked-files=no").strip():
        print("warning: the checkout has uncommitted changes; they are packed as they are on disk")
    name = "%s-%s.tar.gz" % (ARCHIVE_PREFIX, version)
    top = "%s-%s" % (ARCHIVE_PREFIX, version)
    # Pin every timestamp to the commit time so that rebuilding the same
    # commit gives the same bytes, and therefore the same index checksum.
    mtime = int(git("log", "-1", "--format=%ct").strip())

    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w", format=tarfile.PAX_FORMAT) as tar:
        dirs = set()
        for rel in tracked_files():
            parts = rel.split("/")[:-1]
            for i in range(1, len(parts) + 1):
                d = "/".join(parts[:i])
                if d not in dirs:
                    dirs.add(d)
                    info = tarfile.TarInfo("%s/%s" % (top, d))
                    info.type, info.mode, info.mtime = tarfile.DIRTYPE, 0o755, mtime
                    tar.addfile(info)
            src = os.path.join(ROOT, rel)
            info = tarfile.TarInfo("%s/%s" % (top, rel))
            info.mtime = mtime
            if os.path.islink(src):
                info.type, info.linkname, info.mode = tarfile.SYMTYPE, os.readlink(src), 0o777
                tar.addfile(info)
                continue
            st = os.stat(src)
            info.size = st.st_size
            info.mode = 0o755 if st.st_mode & 0o111 else 0o644
            with open(src, "rb") as fp:
                tar.addfile(info, fp)

    os.makedirs(out_dir, exist_ok=True)
    path = os.path.join(out_dir, name)
    with open(path, "wb") as fp:
        fp.write(gzip.compress(buf.getvalue(), 9, mtime=0))

    print("archive  %s" % path)
    print("version  %s" % version)
    print("checksum SHA-256:%s" % sha256(path))
    print("size     %d" % os.path.getsize(path))
    print("url      %s" % RELEASE_URL.format(version=version, name=name))
    return path


def sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as fp:
        for chunk in iter(lambda: fp.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def add_to_index(archive):
    name = os.path.basename(archive)
    m = re.fullmatch(re.escape(ARCHIVE_PREFIX) + r"-(\d+\.\d+\.\d+)\.tar\.gz", name)
    if not m:
        fail("%s is not named %s-<version>.tar.gz" % (name, ARCHIVE_PREFIX))
    version = m.group(1)

    with open(INDEX) as fp:
        index = json.load(fp)
    platforms = index["packages"][0]["platforms"]
    if any(p["version"] == version for p in platforms):
        fail("%s is already in the index; a published archive must never change" % version)

    def key(p):
        return tuple(int(x) for x in p["version"].split("."))

    entry = json.loads(json.dumps(max(platforms, key=key)))
    entry.update({
        "version": version,
        "url": RELEASE_URL.format(version=version, name=name),
        "archiveFileName": name,
        "checksum": "SHA-256:" + sha256(archive),
        "size": str(os.path.getsize(archive)),
    })
    platforms.append(entry)
    with open(INDEX, "w") as fp:
        fp.write(json.dumps(index, indent=2, ensure_ascii=False) + "\n")
    print("added %s to %s" % (version, os.path.relpath(INDEX, ROOT)))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("check")
    b = sub.add_parser("build")
    b.add_argument("--out", default=os.path.join(ROOT, "dist"))
    i = sub.add_parser("index")
    i.add_argument("archive")
    args = ap.parse_args()

    if args.cmd == "check":
        print("version %s" % checked_version())
    elif args.cmd == "build":
        build(args.out)
    else:
        add_to_index(args.archive)


if __name__ == "__main__":
    main()
