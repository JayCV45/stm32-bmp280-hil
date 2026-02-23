#!/usr/bin/env bash
set -euo pipefail

# Usage:
# docker run --rm -v "$PWD":/src:ro -v "$PWD/out":/out <image> /usr/local/bin/build_firmware.sh

SRC_DIR="${SRC_DIR:-/src}"
OUT_DIR="${OUT_DIR:-/out}"
BUILD_DIR="${BUILD_DIR:-/tmp/build}"

# Adjust if your CMake firmware root differs
FIRMWARE_DIR="${FIRMWARE_DIR:-$SRC_DIR/firmware}"

# Optional: if you use a toolchain file in repo, set it here; otherwise script will omit it.
TOOLCHAIN_FILE="${TOOLCHAIN_FILE:-$FIRMWARE_DIR/cmake/gcc-arm-none-eabi.cmake}"

if [ ! -d "$FIRMWARE_DIR" ]; then
  echo "ERROR: firmware dir not found: $FIRMWARE_DIR"
  echo "Set FIRMWARE_DIR env var to your firmware root (inside /src)."
  exit 1
fi

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

if [ -f "$TOOLCHAIN_FILE" ]; then
  cmake -S "$FIRMWARE_DIR" -B "$BUILD_DIR" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE"
else
  cmake -S "$FIRMWARE_DIR" -B "$BUILD_DIR" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release
fi

cmake --build "$BUILD_DIR" -j

if [ -d "$OUT_DIR" ]; then
  mkdir -p "$OUT_DIR"
  cp -f "$BUILD_DIR"/*.elf "$OUT_DIR"/ 2>/dev/null || true
  cp -f "$BUILD_DIR"/*.hex "$OUT_DIR"/ 2>/dev/null || true
  cp -f "$BUILD_DIR"/*.bin "$OUT_DIR"/ 2>/dev/null || true
  echo "Exported artifacts to: $OUT_DIR"
else
  echo "NOTE: $OUT_DIR not mounted; no artifacts exported."
fi
