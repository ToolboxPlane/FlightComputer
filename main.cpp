#include <iostream>
#include <future>
#include "Devices/Serial/Serial.hpp"
#include "Devices/RcLibDebug.hpp"
#include "ChannelMultiplexer.hpp"
#include "Devices/SerialSimulator.hpp"
#include "Fusion/Fusion.hpp"
#include "Fusion/FusionDebug.hpp"
#include "Navigation/Navigation.hpp"
#include "MeshManager/MeshManager.hpp"

#ifdef _ON_PI_
#include "Devices/GPS/GPS.hpp"
#include "Devices/LoRa/LoRa.hpp"
#endif

int main() {
    Serial serial("/dev/ttyACM0", B9600);
    SerialSimulator loraSimulator;
    Fusion fusion;
    Navigation navigation;
    MeshManager meshManager;

    RcLibDebug serialDebug("Serial");
    RcLibDebug loraDebug("Lora");
    FusionDebug fusionDebug("Fusion");

    ChannelMultiplexer<rcLib::Package> flightControllerSerialMultiplexer;
    flightControllerSerialMultiplexer.addInput(serial.getChannelOut());
    flightControllerSerialMultiplexer.addOutput(fusion.getSerialIn());
    flightControllerSerialMultiplexer.addOutput(meshManager.getSerialIn());

    ChannelMultiplexer<rcLib::Package> flightControllerOutMultiplexer;
    flightControllerOutMultiplexer.addInput(meshManager.getSerialOut());
    flightControllerOutMultiplexer.addOutput(serial.getChannelIn());
    flightControllerOutMultiplexer.addOutput(serialDebug.getChannelIn());

    ChannelMultiplexer<rcLib::Package> loraMultiplexer;
    loraMultiplexer.addInput(loraSimulator.getChannelOut());
    loraMultiplexer.addOutput(fusion.getLoRaIn());
    loraMultiplexer.addOutput(meshManager.getLoraIn());

    ChannelMultiplexer<rcLib::Package> loraOutMultiplexer;
    loraOutMultiplexer.addInput(meshManager.getLoraOut());
    loraOutMultiplexer.addOutput(loraSimulator.getChannelIn());
    loraOutMultiplexer.addOutput(loraDebug.getChannelIn());

    ChannelMultiplexer<state_t> fusionMultiplexer;
    fusionMultiplexer.addInput(fusion.getChannelOut());
    fusionMultiplexer.addOutput(fusionDebug.getChannelIn());

    while(1) {
        std::this_thread::sleep_for(std::chrono_literals::operator""h(24));
    }
}