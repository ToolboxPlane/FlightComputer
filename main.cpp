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

    while(1);
}