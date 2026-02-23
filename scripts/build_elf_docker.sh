#!/usr/bin/env bash
set -euo pipefail

IMAGE="${IMAGE:-bmp280-build}"
DOCKERFILE="${DOCKERFILE:-docker/build.Dockerfile}"

docker build -t "$IMAGE" -f "$DOCKERFILE" .
mkdir -p out

docker run --rm \
  -v "$PWD":/src:ro \
  -v "$PWD/out":/out \
  "$IMAGE" \
  /usr/local/bin/build_firmware.sh
