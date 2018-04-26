//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_OUTPUTFILTER_HPP
#define FLIGHTCOMPUTER_OUTPUTFILTER_HPP


#include "../../ThreadModule.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../Navigation/Nav_t.hpp"
#include "../../Channel.hpp"

class OutputFilter : ThreadModule{
public:
    OutputFilter() : ThreadModule() {}

    Channel<rcLib::PackageExtended> &getBaseOut();
    Channel<rcLib::PackageExtended> &getFlightControllerOut();
    Channel<Nav_t> &getChannelIn();
private:
    void run() override;
    Channel<rcLib::PackageExtended> flightControllerOut, baseOut;
    Channel<Nav_t> in;
};


#endif //FLIGHTCOMPUTER_OUTPUTFILTER_HPP
