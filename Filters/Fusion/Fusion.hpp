//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_FUSION_HPP
#define FLIGHTCOMPUTER_FUSION_HPP


#include "../../InputChannel.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../../Node.hpp"
#include "../../Devices/GPS/GpsMeasurement_t.hpp"
#include "State_t.hpp"
#include "../../OutputChannel.hpp"
#include <experimental/optional>

namespace filter {
    enum ProcessingStatus {
        NOT_RECEIVED,
        READY,
        PROCESSED
    };

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
        rcLib::PackageExtended lastPdbPackage;
        GpsMeasurement_t lastGpsMeasurement;
        rcLib::PackageExtended lastBasePackage;
        rcLib::PackageExtended lastTaranisPackage;
        rcLib::PackageExtended lastRemotePackage;
        bool pdbRecv = false, gpsRecv = false, baseRecv = false, taranisRecv = false, remoteRecv = false;

        State_t process();

        int normalizeTaranis(int input);

        InputChannel<rcLib::PackageExtended> baseIn, flightControllerIn, remoteIn, pdbIn, taranisIn;
        InputChannel<GpsMeasurement_t> gpsIn;
        OutputChannel<State_t> out;
    };
}

#endif //FLIGHTCOMPUTER_FUSION_HPP
