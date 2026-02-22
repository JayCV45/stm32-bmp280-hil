# Sử dụng image ubuntu làm gốc
FROM ubuntu:22.04

# Tránh các câu hỏi tương tác khi cài đặt
ENV DEBIAN_FRONTEND=noninteractive

# Cài đặt các công cụ cần thiết: Arm Toolchain, Make, CMake, Python
RUN apt-get update && apt-get install -y \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    make \
    cmake \
    git \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Thiết lập thư mục làm việc trong container
WORKDIR /app

# Lệnh mặc định khi container chạy
CMD ["make", "-j$(nproc)"]
