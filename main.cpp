#include <fstream>
#include "Devices/FlightController/FlightController.hpp"
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
#include "Filters/Navigation/WaypointReader.hpp"
#include "MultipleOutputChannel.hpp"

#ifdef RASPBERRY_PI
#include "Devices/LoRa/LoRa.hpp"
#endif

int main(void) {
    /*
     * I/O-Modules
     */
    //FlightController serial("/dev/ttyACM0", B9600);
    //LoRa lora;
    //Gps gps;

    RcLibSimulator serial(23);
    RcLibSimulator pdb(74);
    RcLibSimulator lora(17);
    GpsSimulator gps;

    std::ifstream serialFile("logs/serial180510.json");
    //ChannelReplay<rcLib::PackageExtended> serial(serialFile);

    std::ifstream waypointFile("missions/waypoints.json");
    WaypointReader waypointReader(waypointFile);

    /*
     * Internal Modules
     */
    Fusion fusion;
    Navigation navigation;
    MeshManager meshManager;
    OutputFilter outputFilter;

    /*
     * Mesh Manager
     */
    serial.getChannelOut() >> meshManager.getSerialIn();
    pdb.getChannelOut() >> meshManager.getSerialIn();
    meshManager.getSerialOut() >> serial.getChannelIn();

    lora.getChannelOut() >> meshManager.getLoraIn();
    meshManager.getLoraOut() >> lora.getChannelIn();

    meshManager.getFlightControllerOut() >> fusion.getFlightControllerIn();
    meshManager.getPdbOut() >> fusion.getPdbIn();

    /*
     * Fusion
     */
    gps.getChannelOut() >> fusion.getGpsIn();
    fusion.getChannelOut() >> navigation.getChannelStateIn();

    /*
     * Navigation
     */
    waypointReader.getChannelOut() >> navigation.getChannelWaypointIn();
    navigation.getChannelOut() >> outputFilter.getChannelIn();


    /*
     * Output Filter
     */
    outputFilter.getBaseOut() >> meshManager.getBaseIn();
    outputFilter.getFlightControllerOut() >> meshManager.getFlightControllerIn();

    /*
     * Logging
     */
    Logger<rcLib::PackageExtended> serialReceive("Serial-Recv", true);
    Logger<rcLib::PackageExtended> serialSend("Serial-Send", true);
    Logger<rcLib::PackageExtended> loraReceive("Lora-Recv", true);
    Logger<rcLib::PackageExtended> loraSend("Lora-Send", true);
    Logger<GpsMeasurement_t> gpsDebug("GPS", true);
    Logger<Nav_t> navDebug("Nav", true);
    Logger<State_t> fusionDebug("Fusion", true);

    serial.getChannelOut() >> serialReceive.getChannelIn();
    meshManager.getSerialOut() >> serialSend.getChannelIn();
    lora.getChannelOut() >> loraReceive.getChannelIn();
    meshManager.getLoraOut() >> loraSend.getChannelIn();
    gps.getChannelOut() >> gpsDebug.getChannelIn();
    fusion.getChannelOut() >> fusionDebug.getChannelIn();
    navigation.getChannelOut() >> navDebug.getChannelIn();


    /*
     * Recorder
     */
    auto time = std::time(nullptr);
    auto localTime = std::localtime(&time);

    std::stringstream serialFileNameStream;
    serialFileNameStream << "logs/serial_" << std::put_time(localTime,"%y_%m_%d_%H_%M") << ".json";
    std::ofstream fileSerialRecord(serialFileNameStream.str());
    ChannelRecorder<rcLib::PackageExtended> serialRecorder(fileSerialRecord);

    std::stringstream gpsFileNameStream;
    gpsFileNameStream << "logs/gps_" << std::put_time(localTime, "%y_%m_%d_%H_%M") << ".json";
    std::ofstream fileGpsRecord(gpsFileNameStream.str());
    ChannelRecorder<GpsMeasurement_t> gpsRecorder(fileGpsRecord);

    serial.getChannelOut() >> serialRecorder.getChannelIn();
    gps.getChannelOut() >> gpsRecorder.getChannelIn();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        std::this_thread::sleep_for(std::chrono_literals::operator""h(24));
    }
#pragma clang diagnostic pop
}
