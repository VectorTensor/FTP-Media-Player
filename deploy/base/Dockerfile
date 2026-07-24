# syntax=docker/dockerfile:1.7

# ---------- Stage 1: compile gRPC + protobuf + abseil ----------
FROM ubuntu:24.04 AS build

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    gcc \
    make \
    git \
    python3-pip \
    ca-certificates \
    ninja-build \
    ccache \
    libssl-dev \
    zlib1g-dev \
    pkg-config \
    && pip3 install --break-system-packages "cmake>=4.1.1" \
    && rm -rf /var/lib/apt/lists/*

ENV CCACHE_DIR=/ccache
ENV CMAKE_CXX_COMPILER_LAUNCHER=ccache
ENV CMAKE_C_COMPILER_LAUNCHER=ccache

ARG GRPC_VERSION=v1.82.0

RUN git clone --recurse-submodules --depth 1 -b ${GRPC_VERSION} \
    https://github.com/grpc/grpc /grpc

WORKDIR /grpc

RUN --mount=type=cache,target=/ccache \
    cmake -B build -G Ninja \
    -DgRPC_INSTALL=ON \
    -DgRPC_BUILD_TESTS=OFF \
    -DgRPC_BUILD_GRPCPP_REFLECTION=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/opt/grpc \
    -DCMAKE_CXX_STANDARD=20 \
    -DABSL_ENABLE_INSTALL=ON \
    -Dprotobuf_INSTALL=ON \
    -Dutf8_range_ENABLE_INSTALL=ON \
    -DgRPC_ZLIB_PROVIDER=package \
    -DgRPC_SSL_PROVIDER=package \
    -DgRPC_CARES_PROVIDER=module \
    && cmake --build build -j"$(nproc)" --target install \
    && find /opt/grpc -name '*.a' -exec strip --strip-debug {} \; \
    && rm -rf /grpc

# ---------- Stage 2: base image for application builds ----------
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    gcc \
    make \
    ninja-build \
    pkg-config \
    git \
    python3-pip \
    ca-certificates \
    libcurl4-openssl-dev \
    libssl-dev \
    zlib1g-dev \
    && pip3 install --break-system-packages "cmake>=4.1.1" \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /opt/grpc /opt/grpc

ENV PATH="/opt/grpc/bin:${PATH}"
ENV CMAKE_PREFIX_PATH="/opt/grpc"