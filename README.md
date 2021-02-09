ustd
====

[![Mac/Linux Build Status](https://travis-ci.org/muwerk/ustd.svg?branch=master)](https://travis-ci.org/muwerk/ustd)
[![Dev Docs](https://img.shields.io/badge/docs-dev-blue.svg)](https://muwerk.github.io/ustd/docs/index.html)
[![Cmake](https://github.com/muwerk/ustd/workflows/CMake/badge.svg)](https://github.com/muwerk/ustd/actions)
[![PlatformIO CI](https://github.com/muwerk/ustd/workflows/PlatformIO%20CI/badge.svg)](https://github.com/muwerk/ustd/actions)

ustd provides minimal and highly portable implementations  of the following classes:

- [`ustd::array`](https://muwerk.github.io/ustd/docs/classustd_1_1array.html), a lightweight c++11
  array implementation (`ustd_array.h`).
- [`ustd::queue`](https://muwerk.github.io/ustd/docs/classustd_1_1queue.html), a lightweight c++11
  queue implementation (`ustd_queue.h`).
- [`ustd::map`](https://muwerk.github.io/ustd/docs/classustd_1_1map.html), a lightweight c++11
  map implementation (`ustd_map.h`).

The libraries are header-only and should work with any c++11 compiler and support platforms
starting with 8k attiny, avr, arduinos, up to esp8266, esp32 and mac and linux.

- [`ustd_functional.h`](https://muwerk.github.io/ustd/docs/functional_8h.html) provides a drop-in
  replacement for `std::function<>` for AVRs: `ustd::function<>` for low-resource AVRs (see
  project [functional-avr](https://github.com/winterscar/functional-avr))

Documentation: [ustd:: documentation](https://muwerk.github.io/ustd/docs/index.html)

Platform defines
----------------

Make sure to use the appropriate platform define before including from `ustd`.

| Platform | #define (by user) | Comment                                    |
| -------- | ----------------- | ------------------------------------------ |
| ATtiny   | `__ATTINY__`      | For very low resource ATMELs               |
| Arduino  | `__UNO__`         | Should work with low resource arduinos     |
| Arduino  | `__ATMEGA__`      | Should work with most arduinos             |
| FeatherM0| `__FEATHER_M0__`  | Adafruit feather M0 (Wifi)                 |
| FeatherM4| `__FEATHER_M4__`  | Adafruit feather M4 (Wifi)                 |
| STM32    | `__BLUEPILL__`    | STM32F103C8T6 ARM Cortex-M3                |
| STM32    | `__BLACKPILL__`   | STM32F411 ARM Cortex-M4                    |
| NRF52    | `__NRF52__`       | Feather NRF52832 Cortex-M4                 |
| Nano33BLE| `__NANOBLE__`     | Arduino Nano 33 BLE (Sense) nRF52840       |
| ESP8266  | `__ESP__`         | For ESP8266 and ESP32                      |
| ESP32    | `__ESP32__`       | ESP32                                      |
| ESP32DEV | `__ESP32DEV__`    | ESP32 git head                             |
| Maix Bit | `__MAIXBIT__`     | Sipeed Maix Bit RISC-V                     |
| Mac      | `__APPLE__`       | Should be defined already                  |
| Linux    | `__linux__`       | Should be defined already                  |

**Note:** If the desired MCU is not in that list, select one with similar characteristics, these
platform defines are used to generate feature-lists that are used by muwerk's modules.

### Additional selectable options

| Option | Commend                                                            |
| ------ | ------------------------------------------------------------------ |
| `USTD_OPTION_FS_FORCE_SPIFFS`| to continue to use old SPIFFS instead of LittleFS. New default for ESP8266 is LittleFS. |
| `USTD_OPTION_FS_FORCE_NO_FS` | Disable all filesystem related functionality |

### Defines generated by `ustd_platform.h` depending on the platform define above

**Note:** All defines below are _automatically_ generated, they are derived from the platform define above:

#### Family defines

| Platform define | Automatically defined family                        | Comment
| --------------- | --------------------------------------------------- | ----------------------------- 
| `__UNO__`       | `__ARDUINO__`                                       | 8-bit Atmel Arduinos
| `__MEGA__`      | `__ARDUINO__`                                       |    "
| `__FEATHER_MO__`| `__ARM__`                                           | ARM cortex
| `__FEATHER_M4__`| `__ARM__`                                           |   "
| `__BLUEPILL__`  | `__ARM__`                                           |   "
| `__BLACKPILL__` | `__ARM__`                                           |   "
| `__NRF52__`     | `__ARM__`                                           |   "
| `__NANOBLE__`   | `__ARM__`                                           |   "
| `__ESP__`       |  t.b.d.                                             | t.b.d.
| `__ESP32__`     |    "                                                |   "
| `__ESPDEV__`    |    "                                                |   "
| `__MAIXBIT__`   | `__RISC_V__`                                        | RISC-V based MCUs
| `__APPLE__`     | `__UNIXOID__`                                       | macOS computer
| `__linux__`     | `__UNIXOID__`                                       | Linux computer

#### Features

| Define                     | Comment                                             |
| -------------------------- | --------------------------------------------------- |
| `USTD_FEATURE_MEMORY`      | This is set to a class of available memory, see below for possible values. |
| `USTD_FEATURE_FILESYSTEM`  | The system has a filesystem, muwerk APIs defined in `filesystem.h` and `jsonfile.h` are available. |
| `USTD_FEATURE_FS_SPIFFS`   | Filesystem is SPIFFS format (legacy ESP8266 and all ESP32) |
| `USTD_FEATURE_FS_LITTLEFS` | Filesystem is LittleFS (standard for ESP8266)
| `USTD_FEATURE_FS_SD`       | Future: SD Filesystem                               |
| `USTD_FEATURE_EEPROM`      | Platform has EEPROM storage                         |
| `USTD_FEATURE_SYSTEMCLOCK` | System has a clock                                  |
| `USTD_FEATURE_CLK_READ`    | Time can be read                                    |
| `USTD_FEATURE_CLK_SET`     | Time can be set                                     |
| `USTD_FEATURE_NET`         | Network access available                            |
| `USTD_FEATURE_FREE_MEMORY` | freeMemory() is available                           |
| `USTD_FEATURE_SUPPORTS_NEW_OPERATOR` | Platform SDK has it's own `new` operator  |

#### Possible values for `USTD_FEATURE_MEMORY`

(Automatically derived by `ustd_platform.h` from platform define `__xxx__`)

| Value                      | Example platform                         |
| -------------------------- | ---------------------------------------- |
| `USTD_FEATURE_MEM_512B`    | ATtiny85                                 |
| `USTD_FEATURE_MEM_2K`      | Arduino UNO, ATtiny1614, AT328P          |
| `USTD_FEATURE_MEM_8K`      | Arduino MEGA                             |
| `USTD_FEATURE_MEM_32K`     | ESP8266, Bluepill, Cortex M0, M3, M4F    |
| `USTD_FEATURE_MEM_128K`    | Blackpill, ESP32                         |
| `USTD_FEATURE_MEM_512K`    |                                          |
| `USTD_FEATURE_MEM_1M`      | Unixoids & RISC-V                        |

To make code dependent on a memory-class, use something like:

```c++
#if USTD_FEATURE_MEMORY >= USTD_FEATURE_MEM_2K
// Feature that requires at least 2k RAM
#endif
```

### Example

```c++
// first a platform define (see table above):
#define __ESP32__
#include "ustd_array.h"

const int ci[]={1,2,3,4,5};
ustd::array<int> ia(ci,5);
for (auto i : ia) {
    printf("%d\n",i);
}
```

Installation
------------

`ustd` is available via Arduino library manager or platformio:

- [Arduino ustd](https://www.arduinolibraries.info/libraries/muwerk-ustd-library)
- [Platformio ustd](https://platformio.org/lib/show/5710/ustd/examples?file=ustd-test.cpp),
  library ID 5710.

Complete example
----------------

See [Examples](https://github.com/muwerk/Examples) for a complete build
example with `ustd` and linux, Arduino-IDE or platformio.

Related projects
----------------

- ustd is used by [muwerk](https://github.com/muwerk/muwerk) to implement a portable cooperative
scheduler with MQTT-like communication queues.

History
-------

- 0.6.0 (2021-02-09) New platforms, iterator and copy-constructor support.
  - Support for iterators and copy-constructors in `ustd::array`, `ustd::queue`, and `ustd::map`.
    (Thanks @proddy for iterator sample implementation.)
  - Platforms: Feather M4 added: `__FEATHER_M4__`
  - Blackpill STM32F411 added.
  - Arduino Nano 33 BLE added. **Note:** requires platformio's `lib_ldf_mode = chain+` definition
    in `platform.ini`, otherwise platformio will get confused about including `WiFi.h`, because
    of faulty `#ifdef` parsing of `ustd_platformio.h`.
  - ASSERT() macros removed due to clashes.
- 0.5.0 (2021-01-30) **BREAKING CHANGE:** In order to prevent name-clashes for include-files for Arduino-IDE users, all
        ustd include files now have an `ustd_` prefix. Compatibility-versions without `ustd_` that
        include the `ustd_` versions are provided, with exception of queue.h (clash with ESP8266-Wifi) and
        platform.h (clash with RISC-V sdk).
- CI    (2021-01-28) Use Github actions to test build for all platform defines.
- 0.4.4 (2021-01-25) Missing `USTD_FEATURE_FILESYSTEM` defines for ESPs added.
- 0.4.2 (2021-01-24) New platforms `__FEATHER_M0__` (ARM Cortex M0), `__BLUEPILL__` (ARM Cortex M3), 
        `__NRF42__` (ARM Cortex M4F), `__MAIXBIT__` (RISC-V). Bugfix for zero-initialisation of array,
        map, and queue (no longer uses memset).
- 0.4.1 (2021-01-22) Bugfix for USTD_FEATURE_MEMORY handling. ATtiny no longer supports ustd::function<>.
- 0.4.0 (2021-01-19) Feature defines in `ustd_platform.h` for better hardware
        specific adaptations.
- 0.3.6 (2021-01-12) Support for UNO and MEGA in functional.h via `__ARDUINO__` define.
- 0.3.5 (2021-01-12) New function `freeMemory()`, new platform define `__UNO__`. (Both `__UNO__`
        and `__ATMEGA__` implicitly define `__ARDUINO__`)
- 0.3.4 (2021-01-11) Small documentation fixes.
- 0.3.3 (2021-01-08) `ustd::array::resize()` did not correctly update the array size, which would
        lead to memory corrupts (tuxpoldo). Improvements for debug macros.
- 0.3.2 (2021-01-07) More consistent debug interface using `DBG()` macros (see
        [`ustd_platform.h`](https://github.com/muwerk/ustd/blob/2a64a88b59e8bc880d7a1ad63e96d8a66bb1aaf8/platform.h#L151)),
        fixes to `USTD_ASSERT` macro that was inconsistently named ASSERT*S*.
- 0.3.1 (2020-12-25) More SPIFFS vs LittleFS preparations
- 0.3.0 (2020-10-26) Cleanup ustd_platform.h: ESP32 continues to use SPIFFS by default, ESP8266
        LittleFS (since SPIFFS is deprecated for ESP8266, and LittleFS is not (yet) available
        for ESP32). This is a breaking change for ESP8266 installations that require the
        filesystem, since an upgrade from SPIFFS to LittleFS is required, see
        [munet Readme](https://github.com/muwerk/munet/blob/master/README.md) for additional
        information.
- 0.2.2 (2020-09-27) Support for LittleFS as successor of ESP8266/ESP32 filesystem
- 0.2.1 (2019-09-19) Functional support for AVRs added (from project
        [functional-avr](https://github.com/winterscar/functional-avr) by
        [winterscar](https://github.com/winterscar)).

References
----------

- `ustd_functional.h` is taken from project [functional-avr](https://github.com/winterscar/functional-avr)
  by [winterscar](https://github.com/winterscar)
- `ustd` and `muwerk` are derivatives and lightweight versions of
  [Meisterwerk](https://github.com/yeasoft/Meisterwerk).
