//
// Created by paul on 14.03.18.
//

#include "RcLibSimulator.hpp"

void RcLibSimulator::run() {
    rcLib::PackageExtended pkg(256, 16);
    pkg.setMeshProperties(static_cast<uint8_t>(true));
    pkg.setDeviceId(deviceId);
    while(!in.isClosed()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for(uint8_t c=0; c < 16; c++) {
            pkg.setChannel(c, c);
        }
        out.put(pkg);
    }
}

Channel<rcLib::PackageExtended> &RcLibSimulator::getChannelIn() {
    return in;
}

MultipleOutputChannel<rcLib::PackageExtended> &RcLibSimulator::getChannelOut() {
    return out;
}

RcLibSimulator::RcLibSimulator(const uint8_t deviceId) : deviceId(deviceId){
    this->start();
}
