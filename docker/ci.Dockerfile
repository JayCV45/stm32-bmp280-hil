FROM ubuntu:22.04

# Install build tools and GoogleTest dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libgtest-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Pre-build GTest libraries
RUN cd /usr/src/googletest && \
    cmake . && \
    make && \
    cp lib/*.a /usr/lib

WORKDIR /app
CMD ["/bin/bash"]
