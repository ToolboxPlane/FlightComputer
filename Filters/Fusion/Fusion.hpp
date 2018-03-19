//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_FUSION_HPP
#define FLIGHTCOMPUTER_FUSION_HPP


#include "../../Channel.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../../ThreadModule.hpp"
#include "../../Devices/GPS/Gps_t.hpp"
#include "State_t.hpp"

enum ProcessingStatus {
    NOT_RECEIVED,
    READY,
    PROCESSED
};

class Fusion : public ThreadModule {
public:
    Fusion() : ThreadModule() {};
    void run() override;

    Channel<rcLib::PackageExtended> &getSerialIn();
    Channel<rcLib::PackageExtended> &getLoRaIn();
    Channel<Gps_t> &getGpsIn();
    Channel<State_t> &getChannelOut();
private:
    State_t process();

    rcLib::PackageExtended lastLoRaPackage;
    std::list<Gps_t> lastGpsValues;
    std::list<rcLib::PackageExtended> lastSerialPackages;

    Channel<rcLib::PackageExtended> loraIn, serialIn;
    Channel<Gps_t> gpsIn;
    Channel<State_t> out;
};


#endif //FLIGHTCOMPUTER_FUSION_HPP
