#include <fstream>
#include <iomanip>
#include <cassert>
#include "Devices/rcLib/RcLibSimulator.hpp"
#include "Filters/Fusion/Fusion.hpp"
#include "Filters/Navigation/Navigation.hpp"
#include "Filters/MeshManager/MeshManager.hpp"
#include "Utilities/Logger.hpp"
#include "Filters/OutputFilter/OutputFilter.hpp"
#include "Devices/GPS/Sim/GpsSimulator.hpp"
#include "Utilities/ChannelRecorder.hpp"
#include "Utilities/ChannelReplay.hpp"
#include "Devices/GPS/Node/Gps.hpp"
#include "OutputChannel.hpp"
#include "Filters/FeedbackControl/FeedbackControl.hpp"
#include "Devices/Network/Network.hpp"
#include "Devices/Serial/SerialPosix.hpp"

#include "Utilities/Si/Si.hpp"
#include "Utilities/Si/SiLiterals.hpp"
#include "Utilities/Si/SiExtended.hpp"

#ifdef RASPBERRY_PI
#include "Devices/LoRa/LoRa.hpp"
#endif

int main() {
    using namespace std::chrono_literals;
    using namespace si::literals;

    /*
     * I/O-Modules
     */
    device::SerialPosix serial{"/dev/ttyUSB0", 115200};
    //device::RcLibSimulator serial{23, 1000};
    //std::ifstream serialFile("logs/serial_18_06_27_19_43.csv");
    //assert(serialFile.is_open());
    //ChannelReplay<rcLib::PackageExtended> serial(serialFile);

#ifdef RASPBERRY_PI
    device::LoRa lora{};
    device::Gps gps{};
#else
    device::RcLibSimulator lora{17, 60000};
    device::GpsSimulator gps{5000};
#endif

    std::ifstream waypointFile("missions/waypoints.csv");
    assert(waypointFile.is_open());
    recording::ChannelReplay<Waypoint_t> waypointReader{waypointFile};

    device::Network network{"127.0.0.1"};

    /*
     * Filters
     */
    filter::MeshManager meshManager{};
    filter::Fusion fusion{};
    filter::Navigation navigation{};
    filter::FeedbackControl feedbackControl{};
    filter::OutputFilter outputFilter{};

    /*
     * Mesh Manager connections
     */
    serial.getChannelOut() >> meshManager.getSerialIn();
    meshManager.getSerialOut() >> serial.getChannelIn();

    lora.getChannelOut() >> meshManager.getLoraIn();
    meshManager.getLoraOut() >> lora.getChannelIn();

    meshManager.getFlightControllerOut() >> fusion.getFlightControllerIn();
    meshManager.getPdbOut() >> fusion.getPdbIn();
    meshManager.getTaranisOut() >> fusion.getTaranisIn();
    meshManager.getRemoteOut() >> fusion.getRemoteIn();
    meshManager.getBaseOut() >> fusion.getBaseIn();

    serial.getChannelOut() >> network.getChannelIn();
    lora.getChannelOut() >> network.getChannelIn();
    outputFilter.getBaseOut() >> network.getChannelIn();

    /*
     * Internal connection
     */
    gps.getChannelOut() >> fusion.getGpsIn();
    fusion.getChannelOut() >> navigation.getChannelStateIn();
    waypointReader.getChannelOut() >> navigation.getChannelWaypointIn();
    navigation.getChannelOut() >> feedbackControl.getChannelIn();
    feedbackControl.getChannelOut() >> outputFilter.getChannelIn();
    outputFilter.getBaseOut() >> meshManager.getBaseIn();
    outputFilter.getFlightControllerOut() >> meshManager.getFlightControllerIn();

    /*
     * Logging
     */
    debug::Logger<rcLib::PackageExtended> serialReceiveDebug{"Serial-Recv", true};
    debug::Logger<rcLib::PackageExtended> serialSendDebug{"Serial-Send", true};
    debug::Logger<rcLib::PackageExtended> loraReceiveDebug{"Lora-Recv", false};
    debug::Logger<rcLib::PackageExtended> loraSendDebug{"Lora-Send", false};
    debug::Logger<GpsMeasurement_t> gpsDebug{"GPS", false};
    debug::Logger<State_t> fusionDebug{"Fusion", false};
    debug::Logger<Nav_t> navDebug{"Nav", false};
    debug::Logger<Control_t> controlDebug{"Control", false};


    serial.getChannelOut() >> serialReceiveDebug.getChannelIn();
    meshManager.getSerialOut() >> serialSendDebug.getChannelIn();
    lora.getChannelOut() >> loraReceiveDebug.getChannelIn();
    meshManager.getLoraOut() >> loraSendDebug.getChannelIn();
    gps.getChannelOut() >> gpsDebug.getChannelIn();
    fusion.getChannelOut() >> fusionDebug.getChannelIn();
    navigation.getChannelOut() >> navDebug.getChannelIn();
    feedbackControl.getChannelOut() >> controlDebug.getChannelIn();

#ifdef RASPBERRY_PI
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
#endif

    std::cout << "Started all modules!" << "\n";
    while(!serial.getChannelIn().isClosed()) {
        std::this_thread::sleep_for(24h);
    }
}
