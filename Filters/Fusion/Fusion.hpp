//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_FUSION_HPP
#define FLIGHTCOMPUTER_FUSION_HPP


#include "../../Channel.hpp"
#include "../../Devices/RadioControlProtocol/rcLib.hpp"
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

    Channel<rcLib::Package> &getSerialIn();
    Channel<rcLib::Package> &getLoRaIn();
    Channel<Gps_t> &getGpsIn();
    Channel<State_t> &getChannelOut();
private:
    State_t process();

    rcLib::Package lastLoRaPackage;
    std::list<Gps_t> lastGpsValues;
    std::list<rcLib::Package> lastSerialPackages;

    Channel<rcLib::Package> loraIn, serialIn;
    Channel<Gps_t> gpsIn;
    Channel<State_t> out;
};


#endif //FLIGHTCOMPUTER_FUSION_HPP
