#include <iostream>
#include <future>
#include "Serial/Serial.hpp"
#include "Serial/RcLibDebug.hpp"
#include "ChannelMultiplexer.hpp"
#include "Serial/SerialSimulator.hpp"
#include "Fusion/Fusion.hpp"
#include "Fusion/FusionDebug.hpp"

int main() {
    Serial serial("/dev/ttyACM0", B9600);
    SerialSimulator loraSimulator;
    Fusion fusion;

    RcLibDebug serialDebug("Serial");
    RcLibDebug loraDebug("Lora");
    FusionDebug fusionDebug("Fusion");

    ChannelMultiplexer<rcLib::Package> flightControllerSerialMultiplexer;
    flightControllerSerialMultiplexer.addOutput(serial.getChannelIn());
    flightControllerSerialMultiplexer.addInput(serial.getChannelOut());
    flightControllerSerialMultiplexer.addOutput(serialDebug.getChannelIn());
    flightControllerSerialMultiplexer.addOutput(fusion.getSerialIn());

    ChannelMultiplexer<rcLib::Package> loraMultiplexer;
    loraMultiplexer.addInput(loraSimulator.getChannelOut());
    loraMultiplexer.addOutput(loraDebug.getChannelIn());
    loraMultiplexer.addOutput(fusion.getLoRaIn());

    ChannelMultiplexer<state_t> fusionMultiplexer;
    fusionMultiplexer.addInput(fusion.getChannelOut());
    fusionMultiplexer.addOutput(fusionDebug.getChannelIn());

    auto serialHandle = std::async(std::launch::async, &Serial::run, &serial);
    auto loraSimHandle = std::async(std::launch::async, &SerialSimulator::run, &loraSimulator);
    auto fusionHandle = std::async(std::launch::async, &Fusion::run, &fusion);

    auto serialDebugHandle = std::async(std::launch::async, &RcLibDebug::run, &serialDebug);
    auto loraDebugHandle = std::async(std::launch::async, &RcLibDebug::run, &loraDebug);
    auto fusionDebugHandle = std::async(std::launch::async, &FusionDebug::run, &fusionDebug);

    auto serialMultiplexerHandle = std::async(std::launch::async, &ChannelMultiplexer<rcLib::Package>::run, &flightControllerSerialMultiplexer);
    auto loraMultiplexerHandle = std::async(std::launch::async, &ChannelMultiplexer<rcLib::Package>::run, &loraMultiplexer);
    auto fusionMultiplexerHandle = std::async(std::launch::async, &ChannelMultiplexer<state_t>::run, &fusionMultiplexer);

    while(1);
}