//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_FUSION_HPP
#define FLIGHTCOMPUTER_FUSION_HPP

#include <optional>

#include "../../../Devices/GPS/Type/GpsMeasurement_t.hpp"
#include "../../../Devices/rcLib/RadioControlProtocolCpp/rcLib.hpp"
#include "../../../Framework/InputChannel.hpp"
#include "../../../Framework/Node.hpp"
#include "../../../Framework/OutputChannel.hpp"
#include "../Lib/AlphaBetaTracker.hpp"
#include "../Lib/Calibration.hpp"
#include "../Lib/StateEstimateParticleFilter.hpp"
#include "../Type/State_t.hpp"

namespace filter {
    class Fusion : public Node {
      public:
        Fusion();

        void run() override;

        InputChannel<rcLib::Package> &getFlightControllerIn();

        InputChannel<rcLib::Package> &getBaseIn();

        InputChannel<rcLib::Package> &getRemoteIn();

        InputChannel<rcLib::Package> &getTaranisIn();

        InputChannel<rcLib::Package> &getPdbIn();

        InputChannel<GpsMeasurement_t> &getGpsIn();

        InputChannel<rcLib::Package> &getNavIn();

        OutputChannel<State_t> &getChannelOut();

      private:
        InputChannel<rcLib::Package> baseIn, flightControllerIn, remoteIn, pdbIn, taranisIn, navIn;
        InputChannel<GpsMeasurement_t> gpsIn;
        OutputChannel<State_t> out;

        void process();

        si::Second<long double> lastUpdate;

        rcLib::Package lastFcPackage;
        std::optional<rcLib::Package> lastPdbPackage;
        std::optional<GpsMeasurement_t> lastGpsMeasurement;
        std::optional<rcLib::Package> lastBasePackage;
        std::optional<rcLib::Package> lastTaranisPackage;
        std::optional<rcLib::Package> lastRemotePackage;
        std::optional<rcLib::Package> lastNavPackage;

        StateEstimateParticleFilter particleFilter;
        AlphaBetaTracker<si::Acceleration<>> accXFilter, accYFilter, accZFilter;
        Calibration calibration;
    };
} // namespace filter

#endif // FLIGHTCOMPUTER_FUSION_HPP
