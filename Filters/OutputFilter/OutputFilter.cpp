//
// Created by paul on 17.03.18.
//

#include "OutputFilter.hpp"

MultipleOutputChannel<rcLib::PackageExtended> & OutputFilter::getBaseOut() {
    return baseOut;
}

MultipleOutputChannel<rcLib::PackageExtended> & OutputFilter::getFlightControllerOut() {
    return flightControllerOut;
}

Channel<Nav_t> &OutputFilter::getChannelIn() {
    return in;
}

void OutputFilter::run() {
    Nav_t nav{};
    rcLib::PackageExtended serialOutPkg(1024, 8), loraOutPkg(1024, 8);
    rcLib::Package::transmitterId = 38;

    loraOutPkg.setMeshProperties(static_cast<uint8_t>(true), 2);

    while(!in.isClosed()) {
        if(in.get(nav)) {
            serialOutPkg.setChannel(0, (uint16_t)nav.heading);
            serialOutPkg.setChannel(1, (uint16_t)nav.pitch);
            serialOutPkg.setChannel(2, (uint16_t)(nav.power*1023));

            loraOutPkg.setChannel(0, (uint16_t)nav.heading);
            loraOutPkg.setChannel(1, (uint16_t)nav.pitch);
            loraOutPkg.setChannel(2, (uint16_t)(nav.power*1023));

            flightControllerOut.put(serialOutPkg);
            baseOut.put(loraOutPkg);
        }
    }
}

OutputFilter::OutputFilter() {
    this->start();
}
