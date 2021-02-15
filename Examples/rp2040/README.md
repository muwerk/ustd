## Raspberry PI Pico RP2040 and USTD

### Preparations

If you haven't installed the Raspberry pico sdk, do so:

You will need `cmake`, `make`, `gcc`, `arm-none-eabi-gcc`, `arm-none-eabi-newlib` (names vary slightly).

```bash
# Decide, were to put the PICO sdk, then:
git clone https://github.com/raspberrypi/pico-sdk
cd pico-sdk
git submodule update --init
```

Make sure `PICO_SDK_PATH` environment variable exists:

```bash
export PICO_SDK_PATH="<your-path>/pic-sdk"

### Build

```bash
mkdir build
cd build
cmake ..
make
