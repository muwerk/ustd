Mac Linux: [![Mac/Linux Build Status](https://travis-ci.org/muwerk/ustd.svg?branch=master)](https://travis-ci.org/muwerk/ustd)

# ustd

ustd provides minimal and highly portable implementations for:

* [array](https://muwerk.github.io/ustd/docs/classustd_1_1array.html)
* [queue](https://muwerk.github.io/ustd/docs/classustd_1_1queue.html)
* [map](https://muwerk.github.io/ustd/docs/classustd_1_1map.html)

classes. The libraries are header-only and should work with any c++11 compiler and support platforms starting with 8k attiny, avr, arduinos, up to esp8266, esp32 and mac and linux.

Documentation: [mustd ustd:: documentation.](https://muwerk.github.io/ustd/docs/index.html)

## Platform defines

Make sure to use the appropriate platform define before including from ustd.

| Platform   | define      | Comment                        |
|------------|-------------|--------------------------------|
| Mac        | `__APPLE__`   | Should be defined already      |
| Linux      | `__linux__`   | Should be defined already      |
| Arduino    | `__ATMEGA__`  | Should work with most arduinos |
| ATtiny     | `__ATTINY__`  | For very low resource ATMELs   |
| ESP8266    | `__ESP__`     | For ESP8266 and ESP32          |
| ESP32      | `__ESP__` and | ESP32 needs either `__ESP32__` |
|            | `__ESP32__`   | or `__ESP32DEV__` in addition  |

```c++
// Sample usage:
#define __ATTINY__ 1
#include "platform.h"
#include "queue.h"
```

## Related projects

* ustd is used by [muwerk](https://github.com/muwerk/muwerk) to implement a portable cooperative scheduler with MQTT-like communication queues.
