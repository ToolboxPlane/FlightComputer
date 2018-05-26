//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_FUSION_HPP
#define FLIGHTCOMPUTER_FUSION_HPP


#include "../../Channel.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../../ThreadModule.hpp"
#include "../../Devices/GPS/GpsMeasurement_t.hpp"
#include "State_t.hpp"
#include "../../MultipleOutputChannel.hpp"
#include <experimental/optional>

enum ProcessingStatus {
    NOT_RECEIVED,
    READY,
    PROCESSED
};

class Fusion : public ThreadModule {
public:
    Fusion();
    void run() override;

    Channel<rcLib::PackageExtended> &getFlightControllerIn();
    Channel<rcLib::PackageExtended> &getBaseIn();
    Channel<rcLib::PackageExtended> &getRemoteIn();
    Channel<rcLib::PackageExtended> &getTaranisIn();
    Channel<rcLib::PackageExtended> &getPdbIn();
    Channel<GpsMeasurement_t> &getGpsIn();

    MultipleOutputChannel<State_t> &getChannelOut();
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

    Channel<rcLib::PackageExtended> baseIn, flightControllerIn, remoteIn, pdbIn, taranisIn;
    Channel<GpsMeasurement_t> gpsIn;
    MultipleOutputChannel<State_t> out;
};


#endif //FLIGHTCOMPUTER_FUSION_HPP
