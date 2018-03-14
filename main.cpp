#include <iostream>
#include <future>
#include "Serial/Serial.hpp"
#include "Serial/RcLibLogger.hpp"
#include "ChannelMultiplexer.hpp"
#include "Serial/SerialSimulator.hpp"

int main() {
    //Serial serial("/dev/ttyACM0", B9600);
    SerialSimulator serial;
    RcLibLogger rcLibLogger;

    ChannelMultiplexer<rcLib::Package> rcLibMultiplexer;
    rcLibMultiplexer.addOutput(serial.getChannelIn());
    rcLibMultiplexer.addInput(serial.getChannelOut());
    rcLibMultiplexer.addOutput(rcLibLogger.getChannelIn());

    auto rcLibLoggerHandle = std::async(std::launch::async, &RcLibLogger::run, &rcLibLogger);
    auto serialHandle = std::async(std::launch::async, &SerialSimulator::run, &serial);
    auto multiplexHandle = std::async(std::launch::async, &ChannelMultiplexer<rcLib::Package>::run, &rcLibMultiplexer);

    while(1);
}