//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_FUSION_HPP
#define FLIGHTCOMPUTER_FUSION_HPP


#include "../Channel.hpp"
#include "../Serial/RadioControlProtocol/rcLib.hpp"
#include "../ThreadModule.hpp"

struct state_t {
    double lat, lon;
    double heightAboveGround, heightAboveSeaLevel;
    double pitch, roll, heading;
    double airspeed, groundSpeed;
    int flightmode, armed;
};

enum ProcessingStatus {
    NOT_RECEIVED,
    READY,
    PROCESSED
};

class Fusion : ThreadModule {
public:
    Fusion() : ThreadModule() {};
    void run() override;

    Channel<rcLib::Package> &getSerialIn();
    Channel<rcLib::Package> &getLoRaIn();
    Channel<state_t> &getChannelOut();
private:
    state_t process(rcLib::Package loraPackage, rcLib::Package serialPackage);

    Channel<rcLib::Package> loraIn, serialIn;
    //@TODO GPS Package
    Channel<state_t> out;
};


#endif //FLIGHTCOMPUTER_FUSION_HPP
