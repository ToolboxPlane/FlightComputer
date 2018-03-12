#include <iostream>
#include <future>
#include "Serial/Serial.hpp"
#include "Serial/RcLibLogger.hpp"
#include "ChannelMultiplexer.hpp"

int main() {
    Channel<rcLib::Package> serialIn, serialOut;
    Serial serial("/dev/ttyACM0", B9600, serialIn, serialOut);

    Channel<rcLib::Package> rcLibDebug;
    RcLibLogger rcLibLogger(rcLibDebug);

    ChannelMultiplexer<rcLib::Package> rcLibMultiplexer;
    rcLibMultiplexer.addOutput(serialIn);
    rcLibMultiplexer.addInput(serialOut);
    rcLibMultiplexer.addOutput(rcLibDebug);

    auto rcLibLoggerHandle = std::async(std::launch::async, &RcLibLogger::run, rcLibLogger);
    auto serialHandle = std::async(std::launch::async, &Serial::run, serial);
    auto multiplexHandle = std::async(std::launch::async, &ChannelMultiplexer<rcLib::Package>::run, rcLibMultiplexer);

    while(1);
}