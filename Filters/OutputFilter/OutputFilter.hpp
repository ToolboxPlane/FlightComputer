//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_OUTPUTFILTER_HPP
#define FLIGHTCOMPUTER_OUTPUTFILTER_HPP


#include "../../Filter.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../Navigation/Nav_t.hpp"
#include "../../InputChannel.hpp"
#include "../../OutputChannel.hpp"

class OutputFilter : public Filter{
public:
    OutputFilter();

    OutputChannel<rcLib::PackageExtended> &getBaseOut();
    OutputChannel<rcLib::PackageExtended> &getFlightControllerOut();
    InputChannel<Nav_t> &getChannelIn();
private:
    void run() override;
    OutputChannel<rcLib::PackageExtended> flightControllerOut, baseOut;
    InputChannel<Nav_t> in;
};


#endif //FLIGHTCOMPUTER_OUTPUTFILTER_HPP
