FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y build-essential cmake libsdl2-dev

WORKDIR /app

CMD rm -rf build \
    && mkdir build \
    && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build

