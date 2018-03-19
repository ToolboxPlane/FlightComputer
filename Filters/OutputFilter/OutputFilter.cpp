//
// Created by paul on 17.03.18.
//

#include "OutputFilter.hpp"

Channel<rcLib::PackageExtended> &OutputFilter::getLoraOut() {
    return loraOut;
}

Channel<rcLib::PackageExtended> &OutputFilter::getSerialOut() {
    return serialOut;
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

            serialOut.put(serialOutPkg);
            loraOut.put(loraOutPkg);
        }
    }
}
