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

enum ProcessingStatus {
    NOT_RECEIVED,
    READY,
    PROCESSED
};

class Fusion : public ThreadModule {
public:
    Fusion() : ThreadModule(), gpsValue(0,0) {};
    void run() override;

    Channel<rcLib::PackageExtended> &getFlightControllerIn();
    Channel<rcLib::PackageExtended> &getBaseIn();
    Channel<rcLib::PackageExtended> &getRemoteIn();
    Channel<rcLib::PackageExtended> &getPdbIn();
    Channel<GpsMeasurement_t> &getGpsIn();
    MultipleOutputChannel<State_t> &getChannelOut();

private:
    rcLib::PackageExtended pdbPackage, fcPackage;
    GpsMeasurement_t gpsValue;

    State_t process();

    Channel<rcLib::PackageExtended> baseIn, flightControllerIn, remoteIn, pdbIn;
    Channel<GpsMeasurement_t> gpsIn;
    MultipleOutputChannel<State_t> out;
};


#endif //FLIGHTCOMPUTER_FUSION_HPP
