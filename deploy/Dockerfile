FROM 387555629933.dkr.ecr.us-east-1.amazonaws.com/grpc-cpp-builder:v1.82.0

COPY . /app
WORKDIR /app

RUN cmake -B build -DUSE_SYSTEM_GRPC=ON
RUN cmake --build build -j$(nproc)