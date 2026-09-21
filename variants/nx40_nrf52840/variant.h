/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry All right reserved.
  Copyright (c) 2018, Adafruit Industries (adafruit.com)
  Copyright (c) 2026 KORLINX

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_NX40_NRF52840_
#define _VARIANT_NX40_NRF52840_

#ifndef USE_TINYUSB
#define USE_TINYUSB
#endif

/** Master clock frequency */
#define VARIANT_MCK       (64000000ul)

#define USE_LFXO      // Board uses 32khz crystal for LF
// define USE_LFRC    // Board uses RC for LF

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// P0.xx -> xx, P1.xx -> 32 + xx
#define PINS_COUNT           (48)
#define NUM_DIGITAL_PINS     (48)
#define NUM_ANALOG_INPUTS    (8)
#define NUM_ANALOG_OUTPUTS   (0)

/*
 * LEDs, all active low
 */
#define PIN_LED1             (11)   // P0.11  LED_R
#define PIN_LED2             (8)    // P0.08  LED_B
#define PIN_LED3             (19)   // P0.19  LED_G
#define PIN_LED4             (14)   // P0.14  LED_ON ("run" LED)

#define LED_BUILTIN          PIN_LED1
#define LED_CONN             PIN_LED2

#define LED_RED              PIN_LED1
#define LED_BLUE             PIN_LED2
#define LED_GREEN            PIN_LED3
#define LED_RUN              PIN_LED4

#define LED_STATE_ON         0         // State when LED is lit

/*
 * Buttons, SW1 is on P0.18/RESET so only SW2 is a GPIO
 */
#define PIN_BUTTON1          (7)    // P0.07  SW2, external pull-up, active LOW

/*
 * Analog, A0..A6 on the J3 header, A7 on the battery divider
 */
#define PIN_A0               (2)    // P0.02/AIN0  J3-1
#define PIN_A1               (3)    // P0.03/AIN1  J3-2
#define PIN_A2               (4)    // P0.04/AIN2  J3-3
#define PIN_A3               (5)    // P0.05/AIN3  J3-4
#define PIN_A4               (28)   // P0.28/AIN4  J3-5
#define PIN_A5               (29)   // P0.29/AIN5  J3-6
#define PIN_A6               (30)   // P0.30/AIN6  J3-7
#define PIN_A7               (31)   // P0.31/AIN7  CK_BAT (on-module)

static const uint8_t A0  = PIN_A0 ;
static const uint8_t A1  = PIN_A1 ;
static const uint8_t A2  = PIN_A2 ;
static const uint8_t A3  = PIN_A3 ;
static const uint8_t A4  = PIN_A4 ;
static const uint8_t A5  = PIN_A5 ;
static const uint8_t A6  = PIN_A6 ;
static const uint8_t A7  = PIN_A7 ;
#define ADC_RESOLUTION    14

// Other pins
#define PIN_AREF           (2)
#define PIN_VBAT           PIN_A7
#define PIN_NFC1           (9)    // P0.09/NFC1  J4-4
#define PIN_NFC2           (10)   // P0.10/NFC2  J4-5

static const uint8_t AREF = PIN_AREF;

/*
 * Charger
 */
#define PIN_CHG_STATUS     (13)   // P0.13  CHG,   active LOW while charging
#define PIN_CHG_HICHG      (36)   // P1.04  HICHG, high-current charge select
#define PIN_CHG_CE         (47)   // P1.15  CE,    charger enable

/*
 * Serial is USB CDC, Serial1 is on the J4 header
 */
#define PIN_SERIAL1_TX      (20)   // P0.20  J4-6
#define PIN_SERIAL1_RX      (21)   // P0.21  J4-7

/*
 * SPI, on the J3 header alongside A4..A6
 */
#define SPI_INTERFACES_COUNT 1

#define PIN_SPI_MISO         (28)  // P0.28  J3-5 (A4)
#define PIN_SPI_MOSI         (29)  // P0.29  J3-6 (A5)
#define PIN_SPI_SCK          (30)  // P0.30  J3-7 (A6)

static const uint8_t SS   = 5 ;    // P0.05  J3-4 (A3)
static const uint8_t MOSI = PIN_SPI_MOSI ;
static const uint8_t MISO = PIN_SPI_MISO ;
static const uint8_t SCK  = PIN_SPI_SCK ;

/*
 * Wire, shared with the SHTC3 and LSM6DS3TR-C
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (6)   // P0.06  I2C_SDA
#define PIN_WIRE_SCL         (23)  // P0.23  I2C_SCL

/*
 * Sensors
 */
#define SHTC3_ADDRESS        (0x70)

#define LSM6DS3TR_C_ADDRESS  (0x6A)
#define PIN_6D_CS            (17)  // P0.17  hold high for I2C mode
#define PIN_6D_INT1          (32)  // P1.00  6D_INT1

/*
 * PDM microphone (MSM261D3526H1CPM)
 */
#define PIN_PDM_CLK          (34)  // P1.02  PDM_CLK
#define PIN_PDM_DIN          (35)  // P1.03  PDM_DATA

/*
 * QSPI flash (P25Q32SU)
 */
#define PIN_QSPI_SCK         (39)  // P1.07
#define PIN_QSPI_CS          (43)  // P1.11
#define PIN_QSPI_IO0         (42)  // P1.10
#define PIN_QSPI_IO1         (46)  // P1.14
#define PIN_QSPI_IO2         (45)  // P1.13
#define PIN_QSPI_IO3         (44)  // P1.12

// Adafruit_SPIFlash has no P25Q32SU entry; declare it in the sketch.

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#endif
