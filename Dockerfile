FROM gcc:13 AS build

WORKDIR /app

RUN apt-get update && apt-get install -y cmake libboost-all-dev libasio-dev

COPY CMakeLists.txt main.cpp ./
RUN cmake -B build && cmake --build build

FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y libboost-system1.74.0 && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY --from=build /app/build/server .
EXPOSE 8080
CMD ["./server"]
