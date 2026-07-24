FROM 387555629933.dkr.ecr.us-east-1.amazonaws.com/grpc-cpp-builder:latest


COPY . /app
WORKDIR /app

RUN rm -rf build && cmake -B build -DUSE_SYSTEM_GRPC=ON
RUN cmake --build build -j$(nproc)