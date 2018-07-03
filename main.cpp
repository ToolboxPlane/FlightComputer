#include <fstream>
#include <iomanip>
#include "Devices/Stream/Serial/Serial.hpp"
#include "Devices/Stream/Tcp/TcpServer.hpp"
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
#include "MultipleOutputChannel.hpp"

#ifdef RASPBERRY_PI
#include "Devices/LoRa/LoRa.hpp"
#endif

int main() {
    /*
     * I/O-Modules
     */
    Serial lora("/dev/ttyACM0", B9600);
    //LoRa lora;
    //Gps gps;

    //RcLibSimulator serial(23, 1000);
    //RcLibSimulator pdb(74, 60000);
    //RcLibSimulator lora(17, 60000);
    GpsSimulator gps(5000);

    std::ifstream serialFile("logs/piLogs/flight_23_6_18/serial_18_06_23_12_58.csv");
    ChannelReplay<rcLib::PackageExtended> serial(serialFile);

    std::ifstream waypointFile("missions/mission.csv");
    ChannelReplay<Waypoint_t> waypointReader(waypointFile);

    TcpServer tcpServer(61888);

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
    //pdb.getChannelOut() >> meshManager.getSerialIn();
    meshManager.getSerialOut() >> serial.getChannelIn();
    meshManager.getTcpOut() >> tcpServer.getChannelIn();

    lora.getChannelOut() >> meshManager.getLoraIn();
    meshManager.getLoraOut() >> lora.getChannelIn();

    meshManager.getFlightControllerOut() >> fusion.getFlightControllerIn();
    meshManager.getPdbOut() >> fusion.getPdbIn();
    meshManager.getTaranisOut() >> fusion.getTaranisIn();
    meshManager.getRemoteOut() >> fusion.getRemoteIn();
    meshManager.getBaseOut() >> fusion.getBaseIn();

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
    Logger<rcLib::PackageExtended> serialReceive("Serial-Recv", false);
    Logger<rcLib::PackageExtended> serialSend("Serial-Send", false);
    Logger<rcLib::PackageExtended> loraReceive("Lora-Recv", false);
    Logger<rcLib::PackageExtended> loraSend("Lora-Send", false);
    Logger<GpsMeasurement_t> gpsDebug("GPS", false);
    Logger<Nav_t> navDebug("Nav", false);
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
    serialFileNameStream << "logs/serial_" << std::put_time(localTime,"%y_%m_%d_%H_%M") << ".csv";
    std::ofstream fileSerialRecord(serialFileNameStream.str());
    ChannelRecorder<rcLib::PackageExtended> serialRecorder(fileSerialRecord);

    std::stringstream gpsFileNameStream;
    gpsFileNameStream << "logs/gps_" << std::put_time(localTime, "%y_%m_%d_%H_%M") << ".csv";
    std::ofstream fileGpsRecord(gpsFileNameStream.str());
    ChannelRecorder<GpsMeasurement_t> gpsRecorder(fileGpsRecord);

    serial.getChannelOut() >> serialRecorder.getChannelIn();
    gps.getChannelOut() >> gpsRecorder.getChannelIn();

    std::cout << "Started all modules!" << "\n";
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        std::this_thread::sleep_for(std::chrono_literals::operator""h(24));
    }
#pragma clang diagnostic pop
}
