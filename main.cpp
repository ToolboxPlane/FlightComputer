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
    //FlightController flightController("/dev/ttyACM0", B9600);

    SerialSimulator flightController;
    SerialSimulator lora;
    GpsSimulator gps;

    Fusion fusion;
    Navigation navigation;
    MeshManager meshManager;

    RcLibDebug serialReceive("FlightController-Received");
    RcLibDebug loraReceive("Lora-Received");
    RcLibDebug serialSend("FlightController-Send");
    RcLibDebug loraSend("Lora-Send");
    FusionDebug fusionDebug("Fusion");
    GpsDebug gpsDebug("GPS");

    /*
     * (Flightcontroller) -> (Fusion, Mesh, Debug)
     */
    ChannelMultiplexer<rcLib::Package> flightControllerSerialMultiplexer;
    flightControllerSerialMultiplexer.addInput(flightController.getChannelOut());
    flightControllerSerialMultiplexer.addOutput(fusion.getSerialIn());
    flightControllerSerialMultiplexer.addOutput(meshManager.getSerialIn());
    flightControllerSerialMultiplexer.addOutput(serialReceive.getChannelIn());

    /*
     * (Mesh) -> (Serial, Debug)
     */
    ChannelMultiplexer<rcLib::Package> flightControllerOutMultiplexer;
    flightControllerOutMultiplexer.addInput(meshManager.getSerialOut());
    flightControllerOutMultiplexer.addOutput(flightController.getChannelIn());
    flightControllerOutMultiplexer.addOutput(serialSend.getChannelIn());

    /*
     * (LoRa) -> (Fusion, Mesh, Debug)
     */
    ChannelMultiplexer<rcLib::Package> loraMultiplexer;
    loraMultiplexer.addInput(lora.getChannelOut());
    loraMultiplexer.addOutput(fusion.getLoRaIn());
    loraMultiplexer.addOutput(meshManager.getLoraIn());
    loraMultiplexer.addOutput(loraReceive.getChannelIn());

    /*
     * (Mesh) -> (LoRa, Debug)
     */
    ChannelMultiplexer<rcLib::Package> loraOutMultiplexer;
    loraOutMultiplexer.addInput(meshManager.getLoraOut());
    loraOutMultiplexer.addOutput(lora.getChannelIn());
    loraOutMultiplexer.addOutput(loraSend.getChannelIn());

    /*
     * (GPS) -> (Fusion, Debug)
     */
    ChannelMultiplexer<gps_t> gpsMultiplexer;
    gpsMultiplexer.addOutput(fusion.getGpsIn());
    gpsMultiplexer.addInput(gps.getChannelOut());
    gpsMultiplexer.addOutput(gpsDebug.getChannelIn());

    /*
     * (Fusion) -> (Navigation, Fusion)
     */
    ChannelMultiplexer<state_t> fusionMultiplexer;
    fusionMultiplexer.addInput(fusion.getChannelOut());
    fusionMultiplexer.addOutput(fusionDebug.getChannelIn());
    fusionMultiplexer.addOutput(navigation.getChannelIn());


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        std::this_thread::sleep_for(std::chrono_literals::operator""h(24));
    }
#pragma clang diagnostic pop
}