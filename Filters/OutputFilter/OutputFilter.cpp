//
// Created by paul on 17.03.18.
//

#include "OutputFilter.hpp"

OutputFilter::OutputFilter() {
    this->start();
}

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
    rcLib::PackageExtended serialOutPkg(1024, 4), loraOutPkg(1024, 8);
    rcLib::Package::transmitterId = 38;

    loraOutPkg.setMeshProperties(static_cast<uint8_t>(true), 2);

    while(!in.isClosed()) {
        if(in.get(nav)) {
            serialOutPkg.setChannel(0, (uint16_t)(nav.power*1023));
            serialOutPkg.setChannel(1, (uint16_t)(nav.pitch+180));
            serialOutPkg.setChannel(2, (uint16_t)(nav.roll+180));
            serialOutPkg.setChannel(3, 0);

            loraOutPkg.setChannel(0, (uint16_t)(nav.power*1023));
            loraOutPkg.setChannel(1, (uint16_t)(nav.pitch+180));
            loraOutPkg.setChannel(2, (uint16_t)(nav.roll+180));
            loraOutPkg.setChannel(3, nav.stateMajor);
            loraOutPkg.setChannel(4, nav.stateMinor);
            loraOutPkg.setChannel(5, 0);
            loraOutPkg.setChannel(6, 0);
            loraOutPkg.setChannel(7, 0);

            flightControllerOut.put(serialOutPkg);
            baseOut.put(loraOutPkg);
        }
    }
}

