#!/usr/bin/env bash
set -euo pipefail

# Build firmware inside the container using CMake + Ninja, and export artifacts.
#
# Example:
#   docker run --rm \
#     -v "$PWD":/src:ro \
#     -v "$PWD/out":/out \
#     <image> /usr/local/bin/build_firmware.sh
#
# Environment variables (optional):
#   SRC_DIR        Source mount point inside container (default: /src)
#   OUT_DIR        Output mount point for artifacts (default: /out)
#   BUILD_DIR      Temporary build directory (default: /tmp/build)
#   FIRMWARE_DIR   Firmware root inside SRC_DIR (default: /src/firmware)
#   TOOLCHAIN_FILE CMake toolchain file path (default: <firmware>/cmake/gcc-arm-none-eabi.cmake)

SRC_DIR="${SRC_DIR:-/src}"
OUT_DIR="${OUT_DIR:-/out}"
BUILD_DIR="${BUILD_DIR:-/tmp/build}"

FIRMWARE_DIR="${FIRMWARE_DIR:-$SRC_DIR/firmware}"
TOOLCHAIN_FILE="${TOOLCHAIN_FILE:-$FIRMWARE_DIR/cmake/gcc-arm-none-eabi.cmake}"

# Validate firmware directory exists
if [ ! -d "$FIRMWARE_DIR" ]; then
  echo "ERROR: firmware directory not found: $FIRMWARE_DIR"
  echo "Set FIRMWARE_DIR to your firmware root (inside /src)."
  exit 1
fi

# Clean and recreate build directory
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

# Configure with toolchain file if present
if [ -f "$TOOLCHAIN_FILE" ]; then
  cmake -S "$FIRMWARE_DIR" -B "$BUILD_DIR" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE"
else
  cmake -S "$FIRMWARE_DIR" -B "$BUILD_DIR" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release
fi

# Build
cmake --build "$BUILD_DIR" -j

# Export build artifacts (if OUT_DIR is mounted)
if [ -d "$OUT_DIR" ]; then
  mkdir -p "$OUT_DIR"
  cp -f "$BUILD_DIR"/*.elf "$OUT_DIR"/ 2>/dev/null || true
  cp -f "$BUILD_DIR"/*.hex "$OUT_DIR"/ 2>/dev/null || true
  cp -f "$BUILD_DIR"/*.bin "$OUT_DIR"/ 2>/dev/null || true
  echo "Exported artifacts to: $OUT_DIR"
else
  echo "NOTE: OUT_DIR not mounted ($OUT_DIR); no artifacts exported."
fi
