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
#include "Devices/GPS/Gps.hpp"

#ifdef RASPBERRY_PI
#include "Devices/LoRa/LoRa.hpp"
#endif

int main(int argc, char** argv) {
    //FlightController serial("/dev/ttyACM0", B9600);
    //LoRa lora;
    //Gps gps;

    RcLibSimulator serial(23);
    RcLibSimulator lora(17);
    GpsSimulator gps;

    Fusion fusion;
    Navigation navigation;
    MeshManager meshManager;
    OutputFilter outputFilter;

    /*
     * (Flightcontroller) -> (Mesh)
     */
    ChannelMultiplexer<rcLib::PackageExtended> serialInMux;
    serialInMux.addInput(serial.getChannelOut());
    serialInMux.addOutput(meshManager.getSerialIn());

    /*
     * (Mesh) -> (Serial)
     */
    ChannelMultiplexer<rcLib::PackageExtended> serialOutMux;
    serialOutMux.addInput(meshManager.getSerialOut());
    serialOutMux.addOutput(serial.getChannelIn());

    /*
     * (LoRa) -> (Mesh)
     */
    ChannelMultiplexer<rcLib::PackageExtended> loraInMux;
    loraInMux.addInput(lora.getChannelOut());
    loraInMux.addOutput(meshManager.getLoraIn());

    /*
     * (Mesh) -> (LoRa)
     */
    ChannelMultiplexer<rcLib::PackageExtended> loraOutMux;
    loraOutMux.addInput(meshManager.getLoraOut());
    loraOutMux.addOutput(lora.getChannelIn());

    /*
     * (Mesh) -> (Fusion)
     */
    ChannelMultiplexer<rcLib::PackageExtended> meshFlightControllerOutMux;
    meshFlightControllerOutMux.addInput(meshManager.getFlightControllerOut());
    meshFlightControllerOutMux.addOutput(fusion.getFlightControllerIn());

    /*
     * (GPS) -> (Fusion)
     */
    ChannelMultiplexer<Gps_t> gpsInMux;
    gpsInMux.addInput(gps.getChannelOut());
    gpsInMux.addOutput(fusion.getGpsIn());

    /*
     * (Fusion) -> (Navigation)
     */
    ChannelMultiplexer<State_t> fusionOutMux;
    fusionOutMux.addInput(fusion.getChannelOut());
    fusionOutMux.addOutput(navigation.getChannelIn());

    /*
     * (Nav) -> (Output)
     */
    ChannelMultiplexer<Nav_t> navOutMux;
    navOutMux.addInput(navigation.getChannelOut());
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







    /*
     * Logging
     */
    Logger<rcLib::PackageExtended> serialReceive("FC-Recv", true);
    Logger<rcLib::PackageExtended> serialSend("FC-Send", true);
    Logger<rcLib::PackageExtended> loraReceive("Lora-Recv", true);
    Logger<rcLib::PackageExtended> loraSend("Lora-Send", true);
    Logger<Gps_t> gpsDebug("GPS", true);
    Logger<Nav_t> navDebug("Nav", true);
    Logger<State_t> fusionDebug("Fusion", true);

    serialInMux.addOutput(serialReceive.getChannelIn());
    serialOutMux.addOutput(serialSend.getChannelIn());
    loraInMux.addOutput(loraReceive.getChannelIn());
    loraOutMux.addOutput(loraSend.getChannelIn());
    gpsInMux.addOutput(gpsDebug.getChannelIn());
    fusionOutMux.addOutput(fusionDebug.getChannelIn());
    navOutMux.addOutput(navDebug.getChannelIn());





    /*
     * Recorder
     */
    std::ofstream fileSerialRecord;
    fileSerialRecord.open("logs/serial.json");
    ChannelRecorder<rcLib::PackageExtended> serialRecorder(fileSerialRecord);

    std::ofstream fileGpsRecord;
    fileSerialRecord.open("logs/gps.json");
    ChannelRecorder<Gps_t> gpsRecorder(fileGpsRecord);

    serialInMux.addOutput(serialRecorder.getChannelIn());
    gpsInMux.addOutput(gpsRecorder.getChannelIn());



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        std::this_thread::sleep_for(std::chrono_literals::operator""h(24));
    }
#pragma clang diagnostic pop
}
