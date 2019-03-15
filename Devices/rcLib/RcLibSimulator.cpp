//
// Created by paul on 14.03.18.
//

#include "RcLibSimulator.hpp"

RcLibSimulator::RcLibSimulator(const uint8_t deviceId, const int intervalMs) :
        deviceId(deviceId), intervalMs(intervalMs){
    this->start();
}

InputChannel<rcLib::PackageExtended> &RcLibSimulator::getChannelIn() {
    return in;
}

OutputChannel<rcLib::PackageExtended> &RcLibSimulator::getChannelOut() {
    return out;
}

void RcLibSimulator::run() {
    rcLib::PackageExtended pkg(256, 16);
    pkg.setMeshProperties(static_cast<uint8_t>(true));
    pkg.setDeviceId(deviceId);
    while(!in.isClosed()) {
        for(uint8_t c=0; c < 16; c++) {
            pkg.setChannel(c, c);
        }
        out.put(pkg);
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}

