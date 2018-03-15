//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_FUSION_HPP
#define FLIGHTCOMPUTER_FUSION_HPP


#include "../Channel.hpp"
#include "../Devices/RadioControlProtocol/rcLib.hpp"
#include "../ThreadModule.hpp"
#include "../Devices/GPS/gps_t.hpp"

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

class Fusion : public ThreadModule {
public:
    Fusion() : ThreadModule() {};
    void run() override;

    Channel<rcLib::Package> &getSerialIn();
    Channel<rcLib::Package> &getLoRaIn();
    Channel<gps_t> &getGpsIn();
    Channel<state_t> &getChannelOut();
private:
    state_t process(rcLib::Package loraPackage, rcLib::Package serialPackage, gps_t gpsPackage);

    Channel<rcLib::Package> loraIn, serialIn;
    Channel<gps_t> gpsIn;
    Channel<state_t> out;
};


#endif //FLIGHTCOMPUTER_FUSION_HPP
