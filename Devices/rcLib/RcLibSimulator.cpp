//
// Created by paul on 14.03.18.
//

#include "RcLibSimulator.hpp"

void RcLibSimultator::run() {
    rcLib::PackageExtended pkg(256, 16);
    pkg.setMeshProperties(static_cast<uint8_t>(true));
    while(!in.isClosed() && !out.isClosed()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for(uint8_t c=0; c < 16; c++) {
            pkg.setChannel(c, c);
        }
        out.put(pkg);
    }
}

Channel<rcLib::PackageExtended> &RcLibSimultator::getChannelIn() {
    return in;
}

Channel<rcLib::PackageExtended> &RcLibSimultator::getChannelOut() {
    return out;
}
