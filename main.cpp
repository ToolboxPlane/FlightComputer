#include <cfenv>
#include "Devices/rcLib/RcLibSimulator.hpp"
#include "Filters/Fusion/Node/Fusion.hpp"
#include "Filters/Navigation/Node/Navigation.hpp"
#include "Filters/MeshManager/MeshManager.hpp"
#include "Utilities/Logger.hpp"
#include "Filters/OutputFilter/OutputFilter.hpp"
#include "Devices/GPS/Sim/GpsSimulator.hpp"
#include "Utilities/ChannelRecorder.hpp"
#include "Utilities/ChannelReplay.hpp"
#include "Devices/GPS/Node/Gps.hpp"
#include "Filters/FeedbackControl/Node/FeedbackControl.hpp"
#include "Devices/Serial/SerialPosix.hpp"
#include "Devices/rcLib/PackageOstream.hpp"
#include "Devices/SRF02/Node/SRF02.hpp"
#include "Devices/Network/Network.hpp"
#include "Devices/SRF02/Sim/SRF02Sim.hpp"

#ifdef RASPBERRY_PI
#include "Devices/LoRa/LoRa.hpp"
#endif

int main() {
    using namespace std::chrono_literals;
    using namespace si::literals;

#ifdef DEBUG
    feenableexcept(FE_INVALID | FE_OVERFLOW); // Floating point exceptions
#endif

    rcLib::Package::setTransmitterId(38);

    /*
     * I/O-Modules
     */

#ifdef RASPBERRY_PI
    device::SerialPosix fc{"/dev/ttyFC", 115200};
    device::SerialPosix pdb{"/dev/ttyPDB", 115200};
    device::SerialPosix lora{"/dev/ttyNav", 115200};
    //device::SRF02 srf02{"/dev/ttyUS"};
    //device::LoRa lora{};
    device::Gps gps{};
#else
    device::SerialPosix fc{"/dev/ttyACM0", 115200};
    device::RcLibSimulator lora{17, 60000};
    device::RcLibSimulator pdb{17, 60000};
    device::SRF02Sim srf02{};
    device::GpsSimulator gps{1000};
#endif

    std::ifstream waypointFile("Missions/mission.csv");
    if (!waypointFile.is_open()) {
        throw std::runtime_error{"Could not open waypoint file!"};
    }
    recording::ChannelReplay<Waypoint_t> waypointReader{waypointFile};

    device::Network network{"192.168.0.120"};

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
    fc.getChannelOut() >> meshManager.getSerialIn();
    pdb.getChannelOut() >> meshManager.getPdbIn();
    lora.getChannelOut() >> meshManager.getLoraIn();

    meshManager.getSerialOut() >> fc.getChannelIn();
    meshManager.getLoraOut() >> lora.getChannelIn();

    meshManager.getFlightControllerOut() >> fusion.getFlightControllerIn();
    meshManager.getPdbOut() >> fusion.getPdbIn();
    meshManager.getTaranisOut() >> fusion.getTaranisIn();
    meshManager.getRemoteOut() >> fusion.getRemoteIn();
    meshManager.getBaseOut() >> fusion.getBaseIn();
    meshManager.getNavOut() >> fusion.getNavIn();

    pdb.getChannelOut() >> network.getChannelIn();
    lora.getChannelOut() >> network.getChannelIn();
    outputFilter.getBaseOut() >> network.getChannelIn();
    fc.getChannelOut() >> network.getChannelIn();

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
    debug::Logger<rcLib::Package> fcReceiveDebug{"FC-Recv", false};
    debug::Logger<rcLib::Package> fcSendDebug{"FC-Send", false};
    debug::Logger<rcLib::Package> pdbReceiveDebug{"PDB-Recv", false};
    debug::Logger<rcLib::Package> loraReceiveDebug{"Lora-Recv", false};
    debug::Logger<rcLib::Package> loraSendDebug{"Lora-Send", false};
    debug::Logger<GpsMeasurement_t> gpsDebug{"GPS", false};
    debug::Logger<State_t> fusionDebug{"Fusion", true};
    debug::Logger<Nav_t> navDebug{"Nav", true};
    debug::Logger<Control_t> controlDebug{"Control", true};

    fc.getChannelOut() >> fcReceiveDebug.getChannelIn();
    pdb.getChannelOut() >> pdbReceiveDebug.getChannelIn();
    lora.getChannelOut() >> loraReceiveDebug.getChannelIn();
    meshManager.getSerialOut() >> fcSendDebug.getChannelIn();
    meshManager.getLoraOut() >> loraSendDebug.getChannelIn();
    gps.getChannelOut() >> gpsDebug.getChannelIn();
    fusion.getChannelOut() >> fusionDebug.getChannelIn();
    navigation.getChannelOut() >> navDebug.getChannelIn();
    feedbackControl.getChannelOut() >> controlDebug.getChannelIn();

#ifdef RASPBERRY_PI
    /*
     * Recorder
     */
    /*auto time = std::time(nullptr);
    auto localTime = std::localtime(&time);

    std::stringstream serialFileNameStream;
    serialFileNameStream << "logs/serial_" << std::put_time(localTime,"%y_%m_%d_%H_%M") << ".csv";
    std::ofstream fileSerialRecord(serialFileNameStream.str());
    recording::ChannelRecorder<rcLib::Package> serialRecorder(fileSerialRecord);

    std::stringstream gpsFileNameStream;
    gpsFileNameStream << "logs/gps_" << std::put_time(localTime, "%y_%m_%d_%H_%M") << ".csv";
    std::ofstream fileGpsRecord(gpsFileNameStream.str());
    recording::ChannelRecorder<GpsMeasurement_t> gpsRecorder(fileGpsRecord);

    fc.getChannelOut() >> serialRecorder.getChannelIn();
    gps.getChannelOut() >> gpsRecorder.getChannelIn();*/
#endif

    std::cout << "Started all modules!" << "\n";
    while (!fc.getChannelIn().isClosed()) {
        std::this_thread::sleep_for(24h);
    }
    return 0;
}
