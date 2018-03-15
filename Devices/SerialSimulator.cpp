//
// Created by paul on 14.03.18.
//

#include "SerialSimulator.hpp"

void SerialSimulator::run() {
    rcLib::Package pkg(256, 16);
    pkg.setMeshProperties(true);
    while(!in.isClosed() && !out.isClosed()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for(uint16_t c=0; c < 16; c++) {
            pkg.setChannel(c, c);
        }
        out.put(pkg);
    }
}

Channel<rcLib::Package> &SerialSimulator::getChannelIn() {
    return in;
}

Channel<rcLib::Package> &SerialSimulator::getChannelOut() {
    return out;
}
