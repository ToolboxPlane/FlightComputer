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
#include "Filters/MessageCoding/Node/MessageDecoding.hpp"
#include "Filters/MessageCoding/Node/MessageEncoding.hpp"
#include "Filters/Navigation/Node/Navigation.hpp"
#include "Filters/OutputFilter/OutputFilter.hpp"
#include "Utilities/Logger.hpp"
#include "Utilities/Recording/ChannelRecorder.hpp"
#include "Utilities/Recording/ChannelReplay.hpp"
#include "Utilities/Recording/NameProvider.hpp"
#include "Filters/OutputFilter/OutputTypes.hpp"

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

#ifndef RASPBERRY_PI
    device::SerialPosix fc{"/dev/ttyFC", 115200};
    //device::SerialPosix pdb{"/dev/ttyPDB", 115200};
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
    // recording::ChannelReplay<rcLib::Package> fc{std::move(fcStream)};
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
    filter::MessageDecoding<FlightControllerPackage> fcDecoding;
    filter::MessageDecoding<TaranisPackage> taranisDecoding;
    filter::MessageDecoding<PdbPackage> pdbDecoding;

    filter::Fusion fusion{};
    filter::Navigation navigation{};
    filter::FeedbackControl feedbackControl{};
    filter::OutputFilter outputFilter{};

    filter::MessageEncoding<FlightControllerSetpoint> setpointEncoding;

    fc.getChannelOut() >> fcDecoding.getChannelIn();
    fc.getChannelOut() >> taranisDecoding.getChannelIn();
    //pdb.getChannelOut() >> pdbDecoding.getChannelIn();
    //lora >> fusion.getRemoteIn();
    //nav>> fusion.getNavIn();

    fcDecoding.getChannelOut() >> fusion.getFlightControllerIn();
    pdbDecoding.getChannelOut() >> fusion.getPdbIn();
    taranisDecoding.getChannelOut() >> fusion.getTaranisIn();
    gps.getChannelOut() >> fusion.getGpsIn();

    fusion.getChannelOut() >> navigation.getChannelStateIn();
    waypointReader.getChannelOut() >> navigation.getChannelWaypointIn();

    navigation.getChannelOut() >> feedbackControl.getChannelIn();

    feedbackControl.getChannelOut() >> outputFilter.getChannelIn();

    outputFilter.getFlightControllerOut() >> setpointEncoding.getChannelIn();

    setpointEncoding.getChannelOut() >> fc.getChannelIn();

    std::cout << "Started all modules!"
              << "\n";
    while (!fc.getChannelIn().isClosed()) {
        std::this_thread::sleep_for(24h);
    }
    return 0;
}
