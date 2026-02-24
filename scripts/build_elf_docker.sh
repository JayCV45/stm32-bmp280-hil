#!/usr/bin/env bash
set -euo pipefail

# Build the firmware builder image, then run it to compile firmware and export artifacts.
#
# Environment variables (optional):
#   IMAGE       Docker image name/tag to build and run (default: bmp280-build)
#   DOCKERFILE  Path to the Dockerfile used for the build (default: docker/build.Dockerfile)
#
# Output:
#   Compiled artifacts (*.elf / *.hex / *.bin) are copied into ./out on the host.

IMAGE="${IMAGE:-bmp280-build}"
DOCKERFILE="${DOCKERFILE:-docker/build.Dockerfile}"

# Build the Docker image that contains the toolchain and build dependencies
docker build -t "$IMAGE" -f "$DOCKERFILE" .

# Ensure the host output directory exists
mkdir -p out

# Run the container:
#  - Mount the repo as read-only at /src
#  - Mount ./out as /out so artifacts can be exported back to the host
#  - Execute the firmware build script inside the container
docker run --rm \
  -v "$PWD":/src:ro \
  -v "$PWD/out":/out \
  "$IMAGE" \
  /usr/local/bin/build_firmware.sh
