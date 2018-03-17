//
// Created by paul on 17.03.18.
//

#include "OutputFilter.hpp"

Channel<rcLib::Package> &OutputFilter::getLoraOut() {
    return loraOut;
}

Channel<rcLib::Package> &OutputFilter::getSerialOut() {
    return serialOut;
}

Channel<Nav_t> &OutputFilter::getChannelIn() {
    return in;
}

void OutputFilter::run() {
    Nav_t nav{};
    rcLib::Package serialOutPkg(1024, 8), loraOutPkg(1024, 8);
    while(!in.isClosed()) {
        if(in.get(nav)) {
            serialOutPkg.setChannel(0, (uint16_t)nav.heading);
            serialOutPkg.setChannel(1, (uint16_t)nav.pitch);
            serialOutPkg.setChannel(2, (uint16_t)nav.power);

            serialOut.put(serialOutPkg);
            loraOut.put(loraOutPkg);
        }
    }
}
