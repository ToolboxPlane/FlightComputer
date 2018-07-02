//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_OUTPUTFILTER_HPP
#define FLIGHTCOMPUTER_OUTPUTFILTER_HPP


#include "../../Filter.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../Navigation/Nav_t.hpp"
#include "../../Channel.hpp"
#include "../../MultipleOutputChannel.hpp"

class OutputFilter : Filter{
public:
    OutputFilter();

    MultipleOutputChannel<rcLib::PackageExtended> &getBaseOut();
    MultipleOutputChannel<rcLib::PackageExtended> &getFlightControllerOut();
    Channel<Nav_t> &getChannelIn();
private:
    void run() override;
    MultipleOutputChannel<rcLib::PackageExtended> flightControllerOut, baseOut;
    Channel<Nav_t> in;
};


#endif //FLIGHTCOMPUTER_OUTPUTFILTER_HPP
