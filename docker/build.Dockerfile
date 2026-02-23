# Sử dụng image ubuntu làm gốc
FROM ubuntu:22.04

# Tránh các câu hỏi tương tác khi cài đặt
ENV DEBIAN_FRONTEND=noninteractive

# Cài đặt các công cụ cần thiết: Arm Toolchain, Make, CMake, Python
RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    make \
    cmake \
    ninja-build \
    git \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Thiết lập thư mục làm việc trong container
WORKDIR /app

# Add export-build script (only used when explicitly called)
COPY docker/build_firmware.sh /usr/local/bin/build_firmware.sh
RUN chmod +x /usr/local/bin/build_firmware.sh

# Lệnh mặc định khi container chạy
CMD ["make", "-j$(nproc)"]
