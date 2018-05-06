#include <fstream>
#include "Devices/FlightController/FlightController.hpp"
#include "ChannelMultiplexer.hpp"
#include "Devices/rcLib/RcLibSimulator.hpp"
#include "Filters/Fusion/Fusion.hpp"
#include "Filters/Navigation/Navigation.hpp"
#include "Filters/MeshManager/MeshManager.hpp"
#include "Utilities/Logger.hpp"
#include "Filters/OutputFilter/OutputFilter.hpp"
#include "Devices/GPS/GpsSimulator.hpp"
#include "Utilities/ChannelRecorder.hpp"
#include "Utilities/ChannelReplay.hpp"

#ifdef RASPBERRY_PI
#include "Devices/GPS/Gps.hpp"
#include "Devices/LoRa/LoRa.hpp"
#endif

int main(int argc, char** argv) {
    //FlightController serial("/dev/ttyACM0", B9600);
    //LoRa lora;
//    Gps gps("/dev/ttyS0", B9600);

    RcLibSimultator serial(23);
    RcLibSimultator lora(17);
    GpsSimulator gps;

    Fusion fusion;
    Navigation navigation;
    MeshManager meshManager;
    OutputFilter outputFilter;

    Logger<rcLib::PackageExtended> serialReceive("FC-Recv", false);
    Logger<rcLib::PackageExtended> serialSend("FC-Send", false);
    Logger<rcLib::PackageExtended> loraReceive("Lora-Recv", false);
    Logger<rcLib::PackageExtended> loraSend("Lora-Send", false);
    Logger<Gps_t> gpsDebug("GPS", false);
    Logger<Nav_t> navDebug("Nav", false);
    Logger<State_t> fusionDebug("Fusion", false);

    /*
     * (Flightcontroller) -> (Mesh, Debug)
     */
    ChannelMultiplexer<rcLib::PackageExtended> fcInMux;
    fcInMux.addInput(serial.getChannelOut());
    fcInMux.addOutput(meshManager.getSerialIn());
    fcInMux.addOutput(serialReceive.getChannelIn());

    /*
     * (Mesh) -> (Serial, Debug)
     */
    ChannelMultiplexer<rcLib::PackageExtended> fcOutMux;
    fcOutMux.addInput(meshManager.getSerialOut());
    fcOutMux.addOutput(serial.getChannelIn());
    fcOutMux.addOutput(serialSend.getChannelIn());

    /*
     * (LoRa) -> (Mesh, Debug)
     */
    ChannelMultiplexer<rcLib::PackageExtended> loraInMux;
    loraInMux.addInput(lora.getChannelOut());
    loraInMux.addOutput(meshManager.getLoraIn());
    loraInMux.addOutput(loraReceive.getChannelIn());

    /*
     * (Mesh) -> (LoRa, Debug)
     */
    ChannelMultiplexer<rcLib::PackageExtended> loraOutMux;
    loraOutMux.addInput(meshManager.getLoraOut());
    loraOutMux.addOutput(lora.getChannelIn());
    loraOutMux.addOutput(loraSend.getChannelIn());

    /*
     * (Mesh) -> (Fusion)
     */
    ChannelMultiplexer<rcLib::PackageExtended> meshFlightControllerOutMux;
    meshFlightControllerOutMux.addInput(meshManager.getFlightControllerOut());
    meshFlightControllerOutMux.addOutput(fusion.getFlightControllerIn());

    /*
     * (GPS) -> (Fusion, Debug)
     */
    ChannelMultiplexer<Gps_t> gpsInMux;
    gpsInMux.addOutput(fusion.getGpsIn());
    gpsInMux.addInput(gps.getChannelOut());
    gpsInMux.addOutput(gpsDebug.getChannelIn());

    /*
     * (Fusion) -> (Navigation, Debug)
     */
    ChannelMultiplexer<State_t> fusionOutMux;
    fusionOutMux.addInput(fusion.getChannelOut());
    fusionOutMux.addOutput(fusionDebug.getChannelIn());
    fusionOutMux.addOutput(navigation.getChannelIn());

    /*
     * (Nav) -> (Output, Debug)
     */
    ChannelMultiplexer<Nav_t> navOutMux;
    navOutMux.addInput(navigation.getChannelOut());
    navOutMux.addOutput(navDebug.getChannelIn());
    navOutMux.addOutput(outputFilter.getChannelIn());

    /*
     * (Output Filter) -> (Mesh)
     */
    ChannelMultiplexer<rcLib::PackageExtended> loraFilterMux;
    loraFilterMux.addInput(outputFilter.getBaseOut());
    loraFilterMux.addOutput(meshManager.getBaseIn());

    /*
     * (Output Filter) -> (Flightcontroller)
     */
    ChannelMultiplexer<rcLib::PackageExtended> serialFilterMux;
    serialFilterMux.addInput(outputFilter.getFlightControllerOut());
    serialFilterMux.addOutput(meshManager.getFlightControllerIn());


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        std::this_thread::sleep_for(std::chrono_literals::operator""h(24));
    }
#pragma clang diagnostic pop
}
