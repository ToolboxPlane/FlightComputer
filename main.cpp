#include <cfenv>
#include <iomanip>
#include "Devices/rcLib/RcLibSimulator.hpp"
#include "Filters/Fusion/Node/Fusion.hpp"
#include "Filters/Navigation/Node/Navigation.hpp"
#include "Filters/MeshManager/MeshManager.hpp"
#include "Utilities/Logger.hpp"
#include "Filters/OutputFilter/OutputFilter.hpp"
#include "Devices/GPS/Sim/GpsSimulator.hpp"
#include "Devices/GPS/Node/Gps.hpp"
#include "Filters/FeedbackControl/Node/FeedbackControl.hpp"
#include "Devices/Serial/SerialPosix.hpp"
#include "Devices/rcLib/PackageUtil.hpp"
#include "Devices/Network/Network.hpp"
#include "Utilities/Recording/NameProvider.hpp"
#include "Utilities/Recording/ChannelRecorder.hpp"
#include "Utilities/Recording/ChannelReplay.hpp"

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
    device::Gps gps{};
#else
    recording::NameProvider replayNameProvider{"2020-04-03_17-32-44_Short"};
    recording::ChannelReplay<rcLib::Package> fc{replayNameProvider.getInputStream("fc")};
    recording::ChannelReplay<rcLib::Package> pdb{replayNameProvider.getInputStream("pdb")};
    recording::ChannelReplay<rcLib::Package> lora{replayNameProvider.getInputStream("lora")};
    recording::ChannelReplay<GpsMeasurement_t> gps{replayNameProvider.getInputStream("gps")};
#endif

    std::ifstream waypointFile("Missions/mission.csv");
    if (!waypointFile.is_open()) {
        throw std::runtime_error{"Could not open waypoint file!"};
    }
    recording::ChannelReplay<Waypoint_t> waypointReader{std::move(waypointFile)};

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
    debug::Logger<State_t> fusionDebug{"Fusion", false};
    debug::Logger<Nav_t> navDebug{"Nav", true};
    debug::Logger<Control_t> controlDebug{"Control", false};

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
    recording::NameProvider recordingNameProvider{};
    recording::ChannelRecorder<rcLib::Package> fcRecorder(recordingNameProvider.getOutputStream("fc"));
    recording::ChannelRecorder<rcLib::Package> pdbRecorder(recordingNameProvider.getOutputStream("pdb"));
    recording::ChannelRecorder<rcLib::Package> loraRecorder(recordingNameProvider.getOutputStream("lora"));
    recording::ChannelRecorder<GpsMeasurement_t> gpsRecorder(recordingNameProvider.getOutputStream("gps"));

    fc.getChannelOut() >> fcRecorder.getChannelIn();
    pdb.getChannelOut() >> pdbRecorder.getChannelIn();
    lora.getChannelOut() >> loraRecorder.getChannelIn();
    gps.getChannelOut() >> gpsRecorder.getChannelIn();
#endif

    std::cout << "Started all modules!" << "\n";
    while (!fc.getChannelIn().isClosed()) {
        std::this_thread::sleep_for(24h);
    }
    return 0;
}
