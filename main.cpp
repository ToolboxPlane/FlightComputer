#include <SiPrinterExtended.hpp>
#include <cfenv>
#include <iomanip>

#include "Devices/GPS/Node/Gps.hpp"
#include "Devices/GPS/Sim/GpsSimulator.hpp"
#include "Devices/Network/Network.hpp"
#include "Devices/Serial/SerialPosix.hpp"
#include "Devices/rcLib/PackageUtil.hpp"
#include "Devices/rcLib/RcLibSimulator.hpp"
#include "Filters/FeedbackControl/Node/FeedbackControl.hpp"
#include "Filters/Fusion/Node/Fusion.hpp"
#include "Filters/MeshManager/MeshManager.hpp"
#include "Filters/Navigation/Node/Navigation.hpp"
#include "Filters/OutputFilter/OutputFilter.hpp"
#include "MessageConversion.hpp"
#include "Utilities/Logger.hpp"
#include "Utilities/Recording/ChannelRecorder.hpp"
#include "Utilities/Recording/ChannelReplay.hpp"
#include "Utilities/Recording/NameProvider.hpp"

int main() {
    using namespace std::chrono_literals;
    using namespace si::literals;

    ToolboxPlaneMessages::FlightController message;
    message.set_pitch(17);
    message.set_roll(37);
    message.set_accx(98);
    message.set_accy(99);
    message.set_accz(101);
    std::cout << message.ByteSize() << std::endl;
    return 0;

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
    // device::SerialPosix lora{"/dev/ttyNav", 115200};
    device::Gps gps{};
#else
    // recording::NameProvider replayNameProvider{"2020-04-07_15-16-45"};
    // recording::NameProvider replayNameProvider{"2020-04-07_15-28-46"};
    recording::NameProvider replayNameProvider{"2020-04-07_15-35-41"};
    auto fcStream = replayNameProvider.getInputStream("fc");
    auto pdbStream = replayNameProvider.getInputStream("pdb");
    auto loraStream = replayNameProvider.getInputStream("lora");
    auto gpsStream = replayNameProvider.getInputStream("gps");
    recording::ChannelReplay<rcLib::Package> fc{std::move(fcStream)};
    recording::ChannelReplay<rcLib::Package> pdb{std::move(pdbStream)};
    recording::ChannelReplay<rcLib::Package> lora{std::move(loraStream)};
    recording::ChannelReplay<GpsMeasurement_t> gps{std::move(gpsStream)};
#endif

    std::ifstream waypointFile("Missions/mission.csv");
    if (!waypointFile.is_open()) {
        throw std::runtime_error{"Could not open waypoint file!"};
    }
    recording::ChannelReplay<Waypoint_t> waypointReader{std::move(waypointFile)};

    device::Network network{"172.23.27.35", true};

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
    // lora.getChannelOut() >> meshManager.getLoraIn();

    meshManager.getSerialOut() >> fc.getChannelIn();
    // meshManager.getLoraOut() >> lora.getChannelIn();

    meshManager.getFlightControllerOut() >> fusion.getFlightControllerIn();
    meshManager.getPdbOut() >> fusion.getPdbIn();
    meshManager.getTaranisOut() >> fusion.getTaranisIn();
    meshManager.getRemoteOut() >> fusion.getRemoteIn();
    meshManager.getBaseOut() >> fusion.getBaseIn();
    meshManager.getNavOut() >> fusion.getNavIn();

    pdb.getChannelOut() >> network.getChannelIn();
    // lora.getChannelOut() >> network.getChannelIn();
    fc.getChannelOut() >> network.getChannelIn();
    outputFilter.getNetworkOut() >> network.getChannelIn();

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
    // lora.getChannelOut() >> loraReceiveDebug.getChannelIn();
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
    // lora.getChannelOut() >> loraRecorder.getChannelIn();
    gps.getChannelOut() >> gpsRecorder.getChannelIn();
#endif

    std::cout << "Started all modules!"
              << "\n";
    while (!fc.getChannelIn().isClosed()) {
        std::this_thread::sleep_for(24h);
    }
    return 0;
}
