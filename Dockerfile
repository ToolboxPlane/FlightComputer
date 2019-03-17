FROM ubuntu:18.04

# Install dependencies
RUN apt-get update -y && apt-get install -y cmake gcc-8 g++-8 libasan5 libgps-dev gpsd setserial git
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8

# Install wiring pi
RUN git clone git://git.drogon.net/wiringPi
WORKDIR wiringPi
RUN ./build

RUN ldconfig

RUN mkdir /src
COPY . /src/
RUN rm -rf /src/build
RUN mkdir -p /src/build

WORKDIR /src/build

RUN mkdir logs
RUN cmake -DCMAKE_BUILD_TYPE=Release .. && make -j$(nproc) FlightComputer

CMD ["Tests/Tests", "--gtest_repeat=10", "--gtest_shuffle", "--gtest_color=yes"]