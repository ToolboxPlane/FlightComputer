//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_FUSION_HPP
#define FLIGHTCOMPUTER_FUSION_HPP

#include "../../../InputChannel.hpp"
#include "../../../Devices/rcLib/PackageExtended.hpp"
#include "../../../Node.hpp"
#include "../../../Devices/GPS/Type/GpsMeasurement_t.hpp"
#include "../Type/State_t.hpp"
#include "../../../OutputChannel.hpp"
#include <optional>

namespace filter {
    class Fusion : public Node {
    public:
        Fusion();

        void run() override;

        InputChannel<rcLib::PackageExtended> &getFlightControllerIn();

        InputChannel<rcLib::PackageExtended> &getBaseIn();

        InputChannel<rcLib::PackageExtended> &getRemoteIn();

        InputChannel<rcLib::PackageExtended> &getTaranisIn();

        InputChannel<rcLib::PackageExtended> &getPdbIn();

        InputChannel<GpsMeasurement_t> &getGpsIn();

        OutputChannel<State_t> &getChannelOut();

    private:
        rcLib::PackageExtended lastFcPackage;
        std::optional<rcLib::PackageExtended> lastPdbPackage;
        std::optional<GpsMeasurement_t> lastGpsMeasurement;
        std::optional<rcLib::PackageExtended> lastBasePackage;
        std::optional<rcLib::PackageExtended> lastTaranisPackage;
        std::optional<rcLib::PackageExtended> lastRemotePackage;

        State_t process();

        static int normalizeTaranis(int input);

        InputChannel<rcLib::PackageExtended> baseIn, flightControllerIn, remoteIn, pdbIn, taranisIn;
        InputChannel<GpsMeasurement_t> gpsIn;
        OutputChannel<State_t> out;
    };
}

#endif //FLIGHTCOMPUTER_FUSION_HPP
