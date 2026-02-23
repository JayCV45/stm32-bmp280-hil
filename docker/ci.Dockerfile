FROM ubuntu:22.04

# 1. Install build tools, GTest, and lcov for code coverage reports
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libgtest-dev \
    git \
    lcov \
    && rm -rf /var/lib/apt/lists/*

# 2. Build GTest (Ubuntu 22.04 keeps source in /usr/src/googletest)
RUN cd /usr/src/googletest && \
    cmake . && \
    make && \
    cp lib/libgtest*.a /usr/lib

WORKDIR /app

# 3. Copy only necessary files first to leverage Docker cache
COPY . .

# 4. Create build directory and compile with UNIT_TEST flag
# We define -DUNIT_TEST=ON so your code knows to use the mock HAL
RUN mkdir -p build && cd build && \
    cmake -DUNIT_TEST=ON ../tests && \  
    make

# 5. Set the default command to run tests and output results
# This ensures the container fails if any test fails
CMD ["./build/tests/unit/unit_tests", "&&", "./build/tests/integration/integration_tests"]
