[![Mac/Linux Build Status](https://travis-ci.org/muwerk/ustd.svg?branch=master)](https://travis-ci.org/muwerk/ustd)
[![Dev Docs](https://img.shields.io/badge/docs-dev-blue.svg)](https://muwerk.github.io/ustd/docs/index.html)

# ustd

ustd provides minimal and highly portable implementations for:

- [`array`](https://muwerk.github.io/ustd/docs/classustd_1_1array.html)
- [`queue`](https://muwerk.github.io/ustd/docs/classustd_1_1queue.html)
- [`map`](https://muwerk.github.io/ustd/docs/classustd_1_1map.html)

classes. The libraries are header-only and should work with any c++11 compiler and support platforms starting with 8k attiny, avr, arduinos, up to esp8266, esp32 and mac and linux.

- [`functional.h`](https://muwerk.github.io/ustd/docs/functional_8h.html) supports a drop-in replacement for `std::function<>` for AVRs: `ustd::function<>` for low-resource AVRs (see project [functional-avr](https://github.com/winterscar/functional-avr))

Documentation: [ustd:: documentation.](https://muwerk.github.io/ustd/docs/index.html)

## Platform defines

Make sure to use the appropriate platform define before including from ustd.

| Platform | #define       | Comment                        |
| -------- | ------------- | ------------------------------ |
| Mac      | `__APPLE__`   | Should be defined already      |
| Linux    | `__linux__`   | Should be defined already      |
| Arduino  | `__ATMEGA__`  | Should work with most arduinos |
| ATtiny   | `__ATTINY__`  | For very low resource ATMELs   |
| ESP8266  | `__ESP__`     | For ESP8266 and ESP32          |
| ESP32    | `__ESP__` and | ESP32 needs either `__ESP32__` |
|          | `__ESP32__`   | or `__ESP32DEV__` in addition  |

For ESP8266 additionally:

- Define `__USE_OLD_FS__` to continue to use old SPIFFS instead of LittleFS. New default for ESP8266 is LittleFS.

```c++
// Sample usage:
#define __ATTINY__ 1
#include "platform.h"
#include "queue.h"
```

## Installation

`ustd` is available via Arduino library manager or platformio:

- [Arduino ustd](https://www.arduinolibraries.info/libraries/muwerk-ustd-library)
- [Platformio ustd](https://platformio.org/lib/show/5710/ustd/examples?file=ustd-test.cpp), library ID 5710.

## Example

See [ustdmin](https://github.com/muwerk/Examples/tree/master/ustdmin) for a complete build example with `ustd` and `platformio`.

## Related projects

- ustd is used by [muwerk](https://github.com/muwerk/muwerk) to implement a portable cooperative scheduler with MQTT-like communication queues.

## History

- 0.3.2 (2021-01-07) More consistent debug interface (@tuxpoldo) using `DBG()` macros (s. [https://github.com/muwerk/ustd/blob/2a64a88b59e8bc880d7a1ad63e96d8a66bb1aaf8/platform.h#L151](`platform.h`) ), fixes to `USTD_ASSERT` macro that was inconsistently named ASSERT*S*.
- 0.3.1 (2020-12-25) More SPIFFS vs LittleFS preparations
- 0.3.0 (2020-10-26) Cleanup platform.h: ESP32 continues to use SPIFFS by default, ESP8266 LittleFS (since SPIFFS is
  deprecated for ESP8266, and LittleFS is not (yet) available for ESP32). This is a breaking change for ESP8266 installations
  that require the filesystem, since an upgrade from SPIFFS to LittleFS is required, see [munet Readme](https://github.com/muwerk/munet/blob/master/README.md) for additional information.
- 0.2.2 (2020-09-27) Support for LittleFS as successor of ESP8266/ESP32 filesystem
- 0.2.1 (2019-09-19) Functional support for AVRs added (from project [functional-avr](https://github.com/winterscar/functional-avr) by [winterscar](https://github.com/winterscar)).

## References

- `functional.h` is taken from project [functional-avr](https://github.com/winterscar/functional-avr) by [winterscar](https://github.com/winterscar)
- `ustd` and `muwerk` are derivatives and lightweight versions of [Meisterwerk](https://github.com/yeasoft/Meisterwerk).
