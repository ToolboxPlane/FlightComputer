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

        InputChannel<FlightControllerPackage> &getFlightControllerIn();

        InputChannel<LoraPackage> &getRemoteIn();

        InputChannel<TaranisPackage> &getTaranisIn();

        InputChannel<PdbPackage> &getPdbIn();

        InputChannel<GpsMeasurement_t> &getGpsIn();

        InputChannel<NavPackage> &getNavIn();

        OutputChannel<FusionResult> &getChannelOut();

      private:
        InputChannel<FlightControllerPackage> flightControllerIn;
        InputChannel<LoraPackage> remoteIn;
        InputChannel<PdbPackage> pdbIn;
        InputChannel<TaranisPackage> taranisIn;
        InputChannel<NavPackage> navIn;
        InputChannel<GpsMeasurement_t> gpsIn;
        OutputChannel<FusionResult> out;

        void process();

        si::Second<long double> lastUpdate;

        FlightControllerPackage lastFcPackage;
        std::optional<PdbPackage> lastPdbPackage;
        std::optional<GpsMeasurement_t> lastGpsMeasurement;
        std::optional<TaranisPackage> lastTaranisPackage;
        std::optional<LoraPackage> lastRemotePackage;
        std::optional<NavPackage> lastNavPackage;

        StateEstimateParticleFilter particleFilter;
        AlphaBetaTracker<si::Acceleration<>> accXFilter, accYFilter, accZFilter;
        Calibration calibration;
    };
} // namespace filter

#endif // FLIGHTCOMPUTER_FUSION_HPP
