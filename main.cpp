#include "Devices/FlightController/FlightController.hpp"
#include "ChannelMultiplexer.hpp"
#include "Devices/RcLibSimulator.hpp"
#include "Filters/Fusion/Fusion.hpp"
#include "Filters/Navigation/Navigation.hpp"
#include "Filters/MeshManager/MeshManager.hpp"

#ifdef _ON_PI_
#include "Devices/GPS/GPS.hpp"
#include "Devices/LoRa/LoRa.hpp"
#else
#include "Devices/GPS/GpsSimulator.hpp"
#include "Logger.hpp"

#endif

int main() {
    //FlightController flightController("/dev/ttyACM0", B9600);
    RcLibSimultator flightController;

#ifdef _ON_PI_
    Gps gps("/dev/tty?", B4800);
    LoRa lora;
#else
    RcLibSimultator lora;
    GpsSimulator gps;
#endif

    Fusion fusion;
    Navigation navigation;
    MeshManager meshManager;

    Logger<rcLib::Package> serialReceive("FC-Recv");
    Logger<rcLib::Package> serialSend("FC-Send");
    Logger<rcLib::Package> loraReceive("Lora-Recv");
    Logger<rcLib::Package> loraSend("Lora-Send");
    Logger<Gps_t> gpsDebug("GPS");
    Logger<Nav_t> navDebug("Nav");
    Logger<State_t> fusionDebug("Fusion");

    /*
     * (Flightcontroller) -> (Fusion, Mesh, Debug)
     */
    ChannelMultiplexer<rcLib::Package> fcInMux;
    fcInMux.addInput(flightController.getChannelOut());
    fcInMux.addOutput(fusion.getSerialIn());
    fcInMux.addOutput(meshManager.getSerialIn());
    fcInMux.addOutput(serialReceive.getChannelIn());

    /*
     * (Mesh) -> (Serial, Debug)
     */
    ChannelMultiplexer<rcLib::Package> fcOutMux;
    fcOutMux.addInput(meshManager.getSerialOut());
    fcOutMux.addOutput(flightController.getChannelIn());
    fcOutMux.addOutput(serialSend.getChannelIn());

    /*
     * (LoRa) -> (Fusion, Mesh, Debug)
     */
    ChannelMultiplexer<rcLib::Package> loraInMux;
    loraInMux.addInput(lora.getChannelOut());
    loraInMux.addOutput(fusion.getLoRaIn());
    loraInMux.addOutput(meshManager.getLoraIn());
    loraInMux.addOutput(loraReceive.getChannelIn());

    /*
     * (Mesh) -> (LoRa, Debug)
     */
    ChannelMultiplexer<rcLib::Package> loraOutMux;
    loraOutMux.addInput(meshManager.getLoraOut());
    loraOutMux.addOutput(lora.getChannelIn());
    loraOutMux.addOutput(loraSend.getChannelIn());

    /*
     * (GPS) -> (Fusion, Debug)
     */
    ChannelMultiplexer<Gps_t> gpsInMux;
    gpsInMux.addOutput(fusion.getGpsIn());
    gpsInMux.addInput(gps.getChannelOut());
    gpsInMux.addOutput(gpsDebug.getChannelIn());

    /*
     * (Fusion) -> (Navigation, Fusion)
     */
    ChannelMultiplexer<State_t> fusionOutMux;
    fusionOutMux.addInput(fusion.getChannelOut());
    fusionOutMux.addOutput(fusionDebug.getChannelIn());
    fusionOutMux.addOutput(navigation.getChannelIn());


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        std::this_thread::sleep_for(std::chrono_literals::operator""h(24));
    }
#pragma clang diagnostic pop
}