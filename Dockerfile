FROM gcc:13 AS build

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    libboost-system-dev \
    libboost-date-time-dev \
    libboost-filesystem-dev \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

COPY CMakeLists.txt main.cpp routes.h ./
RUN cmake -B build && cmake --build build

FROM gcc:13
RUN apt-get update && apt-get install -y --no-install-recommends \
    libboost-system1.74.0 \
    libboost-date-time1.74.0 \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY --from=build /app/build/server .
EXPOSE 8080
CMD ["./server"]
