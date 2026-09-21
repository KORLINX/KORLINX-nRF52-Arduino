# KORLINX nRF52 Arduino Core

Arduino board support package for KORLINX nRF52 boards. Forked from
[Adafruit_nRF52_Arduino](https://github.com/adafruit/Adafruit_nRF52_Arduino).

Sketches upload over native USB. Boards ship a UF2/DFU bootloader, so a debug
probe is needed only once, to install it —
see [KORLINX-nRF52-Bootloader](https://github.com/KORLINX/KORLINX-nRF52-Bootloader).

## Supported boards

<!-- AUTO-GENERATED from boards.txt -->

| Board | FQBN | Documentation |
|---|---|---|
| NX40 nRF52840 | `korlinx:nrf52:nx40` | [NX40 Dev Kit](https://wiki.korlinx.com/docs/Network/Bluetooth/NX40_Dev_Kit/NX40_Dev_Kit_Overview) |

<!-- END AUTO-GENERATED -->

Pinouts, schematics and hardware notes live on the
[KORLINX wiki](https://wiki.korlinx.com). Pin names follow the variant headers
in `variants/<board>/variant.h`.

## Install

### Board Manager

Add this to **Arduino IDE → Settings → Additional Boards Manager URLs**, then
install **KXBoards nRF52** from **Tools → Board → Boards Manager**:

```
https://raw.githubusercontent.com/KORLINX/KORLINX-nRF52-Arduino/refs/heads/master/package_korlinx_index.json
```

### From source

Submodules are required; without them the build fails on a missing
`Adafruit_USBD_CDC.h`.

```bash
mkdir -p ~/Documents/Arduino/hardware/korlinx
git clone --recursive \
  https://github.com/KORLINX/KORLINX-nRF52-Arduino.git \
  ~/Documents/Arduino/hardware/korlinx/nrf52
```

Restart the IDE afterwards — it scans `hardware/` only at startup.

## Upload

Select the board and port and upload as usual. The IDE performs a 1200 bps touch
to enter DFU, so no button press is needed.

```bash
arduino-cli compile --upload -p <port> -b korlinx:nrf52:nx40 <sketch>
```

Uploading over the serial port needs
[adafruit-nrfutil](https://github.com/adafruit/Adafruit_nRF52_nrfutil). Windows
and macOS binaries ship in `tools/adafruit-nrfutil/`. On Linux:

```bash
pip3 install --user adafruit-nrfutil
```

## Install the bootloader

A blank board, or one running another RTOS, has no UF2 bootloader. Install it
once over SWD with
[nRF Util](https://www.nordicsemi.com/Products/Development-tools/nrf-util):

```bash
nrfutil device program \
  --firmware bootloader/<variant>/<variant>_bootloader-<ver>_<sd>.hex \
  --options chip_erase_mode=ERASE_ALL,reset=RESET_SYSTEM
```

> **Warning:** `chip_erase_mode=ERASE_ALL` erases the whole device.

## Troubleshooting

**Board missing from Tools → Board** — restart the IDE.

**`fatal error: Adafruit_USBD_CDC.h: No such file or directory`** — run
`git submodule update --init --recursive`.

**`undefined reference to 'Serial'`** — add `#include <Adafruit_TinyUSB.h>` to
the sketch.

**No serial port after the 1200 bps touch** — the VID/PID in `boards.txt` must
match `USB_DESC_VID` and `USB_DESC_UF2_PID` in the bootloader's `board.h`.

**`Tools → Burn Bootloader` fails** — the bundled `nrfjprog` tool package ships
J-Link libraries but no `nrfjprog` binary. Use `nrfutil device program` instead.

## License

Same license as the upstream Adafruit core; see `LICENSE`. Bootloader images
include the Nordic SoftDevice S140, covered by
`bootloader/s140_nrf52_6.1.1_license-agreement.txt`.

## Credits

Based on [Adafruit_nRF52_Arduino](https://github.com/adafruit/Adafruit_nRF52_Arduino)
by Adafruit, which is based on
[Arduino-nRF5](https://github.com/sandeepmistry/arduino-nRF5) by Sandeep Mistry,
which is based on the
[Arduino SAMD Core](https://github.com/arduino/ArduinoCore-samd).

Uses [FreeRTOS](https://www.freertos.org/),
[LittleFS](https://github.com/ARMmbed/littlefs),
[nrfx](https://github.com/NordicSemiconductor/nrfx) and
[TinyUSB](https://github.com/hathach/tinyusb).
