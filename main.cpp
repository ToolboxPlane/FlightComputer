#include <fstream>
#include <iomanip>
#include <cassert>
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
#include "OutputChannel.hpp"

#ifdef RASPBERRY_PI
#include "Devices/LoRa/LoRa.hpp"
#endif

int main() {
    /*
     * I/O-Modules
     */
    //Serial serial{"/dev/ttyACM0", B9600};
    //LoRa lora;
    //Gps gps;

    device::RcLibSimulator serial{23, 50};
    device::RcLibSimulator lora{17, 60000};
    device::GpsSimulator gps{5000};

    //std::ifstream serialFile("logs/serial_18_06_27_19_43.csv");
    //assert(serialFile.is_open());
    //ChannelReplay<rcLib::PackageExtended> serial(serialFile);

    std::ifstream waypointFile("missions/waypoints.csv");
    assert(waypointFile.is_open());
    recording::ChannelReplay<Waypoint_t> waypointReader{waypointFile};

    device::TcpServer tcpServer{61888};

    /*
     * Internal Modules
     */
    filter::Fusion fusion{};
    filter::Navigation navigation{};
    filter::MeshManager meshManager{};
    filter::OutputFilter outputFilter{};

    /*
     * Mesh Manager
     */
    serial.getChannelOut() >> meshManager.getSerialIn();
    meshManager.getSerialOut() >> serial.getChannelIn();
    meshManager.getTcpOut() >> tcpServer.getChannelIn();

    //lora.getChannelOut() >> meshManager.getLoraIn();
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
     * Output Node
     */
    outputFilter.getBaseOut() >> meshManager.getBaseIn();
    outputFilter.getFlightControllerOut() >> meshManager.getFlightControllerIn();

    /*
     * Logging
     */
    debug::Logger<rcLib::PackageExtended> serialReceiveDebug{"Serial-Recv", true};
    debug::Logger<rcLib::PackageExtended> serialSendDebug{"Serial-Send", false};
    debug::Logger<rcLib::PackageExtended> loraReceiveDebug{"Lora-Recv", false};
    debug::Logger<rcLib::PackageExtended> loraSendDebug{"Lora-Send", false};
    debug::Logger<GpsMeasurement_t> gpsDebug{"GPS", false};
    debug::Logger<Nav_t> navDebug{"Nav", false};
    debug::Logger<State_t> fusionDebug{"Fusion", false};


    serial.getChannelOut() >> serialReceiveDebug.getChannelIn();
    meshManager.getSerialOut() >> serialSendDebug.getChannelIn();
    lora.getChannelOut() >> loraReceiveDebug.getChannelIn();
    meshManager.getLoraOut() >> loraSendDebug.getChannelIn();
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
    recording::ChannelRecorder<rcLib::PackageExtended> serialRecorder(fileSerialRecord);

    std::stringstream gpsFileNameStream;
    gpsFileNameStream << "logs/gps_" << std::put_time(localTime, "%y_%m_%d_%H_%M") << ".csv";
    std::ofstream fileGpsRecord(gpsFileNameStream.str());
    recording::ChannelRecorder<GpsMeasurement_t> gpsRecorder(fileGpsRecord);

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
