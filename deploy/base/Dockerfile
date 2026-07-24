# syntax=docker/dockerfile:1.7
#
# grpc-base image
# Build once per gRPC version, push to ECR, then have your app Dockerfile
# `FROM <your-ecr-repo>/grpc-base:<tag>` instead of FetchContent-ing grpc.
#
# Build:
#   docker build -t <your-ecr-repo>/grpc-base:v1.82.0 -f Dockerfile.grpc-base .
# Push:
#   docker push <your-ecr-repo>/grpc-base:v1.82.0

# ---------- Stage 1: compile gRPC + protobuf + abseil ----------
FROM debian:bookworm-slim AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    git \
    python3-pip \
    ca-certificates \
    ninja-build \
    ccache \
    libssl-dev \
    zlib1g-dev \
    pkg-config \
    && pip install --break-system-packages "cmake>=4.1.1" \
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
    -DgRPC_ZLIB_PROVIDER=package \
    -DgRPC_SSL_PROVIDER=package \
    -DgRPC_CARES_PROVIDER=module \
    && cmake --build build -j$(nproc) --target install \
    && find /opt/grpc -name '*.a' -exec strip --strip-debug {} \; \
    && rm -rf /grpc

# ---------- Stage 2: runtime-ready base your app images will FROM ----------
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    ninja-build \
    pkg-config \
    git \
    python3-pip \
    ca-certificates \
    libcurl4-openssl-dev \
    libssl-dev \
    zlib1g-dev \
    && pip install --break-system-packages "cmake>=4.1.1" \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /opt/grpc /opt/grpc
ENV PATH="/opt/grpc/bin:${PATH}"
ENV CMAKE_PREFIX_PATH="/opt/grpc"