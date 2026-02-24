# STM32 BMP280 HIL Project

A production-style embedded project for reading temperature from a Bosch **BMP280** sensor on an **STM32F446** target, with layered quality gates:

- host-side unit/integration tests (mocked HAL),
- firmware cross-compilation,
- hardware-in-the-loop (HIL) flash-and-verify automation.

This repository is designed to be approachable for firmware developers while still reflecting CI/CD practices used in professional embedded teams.

---

## Table of Contents

- [Project Goals](#project-goals)
- [Architecture Overview](#architecture-overview)
- [Repository Layout](#repository-layout)
- [Build and Test Workflows](#build-and-test-workflows)
  - [1) Host Unit + Integration Tests](#1-host-unit--integration-tests)
  - [2) Firmware Build (ARM GCC)](#2-firmware-build-arm-gcc)
  - [3) Hardware-in-the-Loop (HIL) Test](#3-hardware-in-the-loop-hil-test)
- [How It Works (Runtime Flow)](#how-it-works-runtime-flow)
- [CI Pipeline](#ci-pipeline)
- [Troubleshooting](#troubleshooting)
- [Development Notes](#development-notes)
- [Roadmap Ideas](#roadmap-ideas)

---

## Project Goals

1. Build a reliable BMP280 temperature path on STM32 over I2C.
2. Keep sensor logic testable off-target with mock HAL shims.
3. Validate the full system on real hardware using automated HIL checks.
4. Provide a clean baseline for extending toward pressure/altitude and richer firmware architecture.

---

## Architecture Overview

### Firmware layer

- `firmware/Core/Src/main.c` initializes clocks/peripherals and runs the main loop.
- Temperature is read from BMP280 and streamed via UART (`115200` baud).

### Driver layer

- `firmware/Drivers/BMP280/src/BMP280.cpp` implements BMP280 init/read logic in C++.
- `firmware/Drivers/BMP280/src/BMP280_Wrapper.cpp` provides a C ABI wrapper so C firmware can call C++ driver code.

### Test layer

- `tests/` builds the same driver code with `UNIT_TEST` enabled.
- A mock HAL (`tests/mocks/stm32_hal_mock.h`) emulates I2C register reads/writes in memory.
- GTest covers:
  - compensation math correctness,
  - wrapper/init flow correctness.

### Automation layer

- Docker images for:
  - host-side test execution (`docker/ci.Dockerfile`),
  - firmware toolchain builds (`docker/build.Dockerfile`).
- HIL script (`scripts/flash_and_test.py`) flashes ELF via `pyocd`, then checks UART output for `Temperature:`.

---

## Repository Layout

```text
.
├── .github/workflows/hil_test.yaml    # CI pipeline: tests + build + HIL
├── docker/
│   ├── build.Dockerfile               # ARM GCC firmware build image
│   ├── ci.Dockerfile                  # Host test image (GTest)
│   └── build_firmware.sh              # Artifact export helper
├── firmware/
│   ├── Core/                          # CubeMX-generated app and startup glue
│   ├── Drivers/
│   │   ├── BMP280/                    # Custom BMP280 C++ driver + C wrapper
│   │   ├── CMSIS/
│   │   └── STM32F4xx_HAL_Driver/
│   ├── Makefile                       # Cross-compile to ELF/HEX/BIN
│   └── STM32F446XX_FLASH.ld           # Linker script
├── scripts/
│   ├── build_elf_docker.sh            # Build firmware artifacts in Docker
│   └── flash_and_test.py              # Flash + UART validation (HIL)
├── tests/
│   ├── unit/                          # Formula-oriented tests
│   ├── integration/                   # Wrapper/flow tests
│   └── mocks/                         # HAL mock implementation
└── requirements.txt                   # Python deps for HIL tooling
```

---

## Build and Test Workflows

### 1) Host Unit + Integration Tests

Run in container (recommended):

```bash
docker build -t bmp280-ci -f docker/ci.Dockerfile .
docker run --rm bmp280-ci
```

This compiles `tests/` with `-DUNIT_TEST=ON` and executes both test binaries.

---

### 2) Firmware Build (ARM GCC)

#### Option A — Docker Compose (recommended)

```bash
docker compose run --rm --user "$(id -u):$(id -g)" builder make -C firmware clean all
```

#### Option B — Local toolchain

Install `arm-none-eabi-*` toolchain and run:

```bash
make -C firmware clean all
```

Build outputs:

- `firmware/build/firmware.elf`
- `firmware/build/firmware.hex`
- `firmware/build/firmware.bin`

---

### 3) Hardware-in-the-Loop (HIL) Test

Prerequisites:

- STM32 board connected with ST-LINK/debug access.
- BMP280 wired on I2C.
- UART exposed at `/dev/ttyACM0` (or update script constant).
- Python environment with `requirements.txt` installed.

Run:

```bash
python3 scripts/flash_and_test.py
```

The script:

1. flashes `firmware/build/firmware.elf` via `pyocd`,
2. resets target,
3. listens on UART and passes when `Temperature:` appears.

---

## How It Works (Runtime Flow)

On boot, firmware:

1. Initializes HAL, system clock, GPIO, I2C1, USART2.
2. Scans I2C bus (prints detected device addresses).
3. Creates BMP280 instance through wrapper.
4. Calls sensor init:
   - soft reset,
   - chip-ID verification (`0x58`),
   - calibration read,
   - measurement config.
5. Enters loop:
   - read compensated temperature,
   - print `Temperature: XX.XX C` over UART every 500 ms.

---

## CI Pipeline

GitHub Actions workflow (`.github/workflows/hil_test.yaml`) runs on a self-hosted STM32 runner and performs:

1. cleanup old build artifacts,
2. host tests in Docker (`docker/ci.Dockerfile`),
3. firmware build in Docker Compose,
4. HIL flash + UART verification.

This gives confidence from algorithm-level behavior up to real-board execution.

---

## Troubleshooting

### No `Temperature:` on UART

- Confirm UART port and permissions (`/dev/ttyACM0`).
- Verify baud rate is `115200` in terminal and firmware.
- Check board reset and USB serial reconnect timing.

### HIL flash errors (`pyocd`)

- Confirm probe is connected and visible.
- Ensure target override matches (`stm32f446re` in script).
- Rebuild firmware so `firmware/build/firmware.elf` exists.

### I2C scan finds nothing

- Verify wiring (SCL/SDA/GND/VCC), pull-ups, and sensor address (`0x76` default).
- Confirm sensor power rail and board ground reference.

### Docker permission issues

- Use `--user "$(id -u):$(id -g)"` in compose run to avoid root-owned build outputs.

---

## Development Notes

- Driver is intentionally split into:
  - C++ implementation (`BMP280` class),
  - C wrapper for firmware interoperability.
- Testability hinges on the compile-time gate in `BMP280.hpp` (`UNIT_TEST` switches to mock HAL).
- Current implementation focuses on temperature path; pressure path can be added using same calibration/compensation pattern.

---

## Roadmap Ideas

- Add pressure and altitude APIs + tests.
- Add error codes instead of sentinel temperature values.
- Introduce non-blocking sampling/state machine instead of fixed `HAL_Delay` loop.
- Add UART command interface for runtime diagnostics.
- Add coverage publishing from host tests.

---

## License

This project includes STM32 HAL/CMSIS components and generated startup files from ST ecosystems.
Confirm licensing requirements for redistribution in your target environment.
