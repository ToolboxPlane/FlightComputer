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
    /*
     * I/O-Modules
     */
    //FlightController serial("/dev/ttyACM0", B9600);
    //LoRa lora;
    //Gps gps;

    //RcLibSimulator serial(23);
    RcLibSimulator lora(17);
    GpsSimulator gps;

    std::ifstream serialFile;
    serialFile.open("logs/serial180510.json");
    ChannelReplay<rcLib::PackageExtended> serial(serialFile);

    /*
     * Internal Modules
     */
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
    ChannelMultiplexer<GpsMeasurement_t> gpsInMux;
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
    Logger<rcLib::PackageExtended> serialReceive("FC-Recv", false);
    Logger<rcLib::PackageExtended> serialSend("FC-Send", false);
    Logger<rcLib::PackageExtended> loraReceive("Lora-Recv", false);
    Logger<rcLib::PackageExtended> loraSend("Lora-Send", false);
    Logger<GpsMeasurement_t> gpsDebug("GPS", false);
    Logger<Nav_t> navDebug("Nav", false);
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
    auto time = std::time(nullptr);
    auto localTime = std::localtime(&time);

    std::ofstream fileSerialRecord;
    std::stringstream serialFileNameStream;
    serialFileNameStream << "logs/serial_" << std::put_time(localTime,"%y_%m_%d_%H_%M") << ".json";
    fileSerialRecord.open(serialFileNameStream.str());
    ChannelRecorder<rcLib::PackageExtended> serialRecorder(fileSerialRecord);

    std::ofstream fileGpsRecord;
    std::stringstream gpsFileNameStream;
    gpsFileNameStream << "logs/gps_" << std::put_time(localTime, "%y_%m_%d_%H_%M") << ".json";
    fileGpsRecord.open(gpsFileNameStream.str());
    ChannelRecorder<GpsMeasurement_t> gpsRecorder(fileGpsRecord);

    serialInMux.addOutput(serialRecorder.getChannelIn());
    gpsInMux.addOutput(gpsRecorder.getChannelIn());



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        std::this_thread::sleep_for(std::chrono_literals::operator""h(24));
    }
#pragma clang diagnostic pop
}
