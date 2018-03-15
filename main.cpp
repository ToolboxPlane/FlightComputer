#include "Devices/FlightController/FlightController.hpp"
#include "Devices/RcLibDebug.hpp"
#include "ChannelMultiplexer.hpp"
#include "Devices/SerialSimulator.hpp"
#include "Fusion/Fusion.hpp"
#include "Fusion/FusionDebug.hpp"
#include "Navigation/Navigation.hpp"
#include "MeshManager/MeshManager.hpp"
#include "Devices/GPS/GpsDebug.hpp"

#ifdef _ON_PI_
#include "Devices/GPS/GPS.hpp"
#include "Devices/LoRa/LoRa.hpp"
#else
#include "Devices/GPS/GpsSimulator.hpp"
#endif

int main() {
    FlightController flightController("/dev/ttyACM0", B9600);
    SerialSimulator loraSimulator;
    GpsSimulator gpsSimulator;

    Fusion fusion;
    Navigation navigation;
    MeshManager meshManager;

    RcLibDebug serialDebug("FlightController");
    RcLibDebug loraDebug("Lora");
    FusionDebug fusionDebug("Fusion");
    GpsDebug gpsDebug("GPS");

    ChannelMultiplexer<rcLib::Package> flightControllerSerialMultiplexer;
    flightControllerSerialMultiplexer.addInput(flightController.getChannelOut());
    flightControllerSerialMultiplexer.addOutput(fusion.getSerialIn());
    flightControllerSerialMultiplexer.addOutput(meshManager.getSerialIn());
    //flightControllerSerialMultiplexer.addOutput(serialDebug.getChannelIn());

    ChannelMultiplexer<rcLib::Package> flightControllerOutMultiplexer;
    flightControllerOutMultiplexer.addInput(meshManager.getSerialOut());
    flightControllerOutMultiplexer.addOutput(flightController.getChannelIn());

    ChannelMultiplexer<rcLib::Package> loraMultiplexer;
    loraMultiplexer.addInput(loraSimulator.getChannelOut());
    loraMultiplexer.addOutput(fusion.getLoRaIn());
    loraMultiplexer.addOutput(meshManager.getLoraIn());

    ChannelMultiplexer<rcLib::Package> loraOutMultiplexer;
    loraOutMultiplexer.addInput(meshManager.getLoraOut());
    loraOutMultiplexer.addOutput(loraSimulator.getChannelIn());
    //loraOutMultiplexer.addOutput(loraDebug.getChannelIn());

    ChannelMultiplexer<gps_t> gpsMultiplexer;
    gpsMultiplexer.addOutput(fusion.getGpsIn());
    gpsMultiplexer.addInput(gpsSimulator.getChannelOut());
    gpsMultiplexer.addOutput(gpsDebug.getChannelIn());

    ChannelMultiplexer<state_t> fusionMultiplexer;
    fusionMultiplexer.addInput(fusion.getChannelOut());
    fusionMultiplexer.addOutput(fusionDebug.getChannelIn());

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        std::this_thread::sleep_for(std::chrono_literals::operator""h(24));
    }
#pragma clang diagnostic pop
}