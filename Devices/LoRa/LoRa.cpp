//
// Created by paul on 15.03.18.
//

#include "LoRa.hpp"

Channel<rcLib::Package> &LoRa::getChannelOut() {
    return out;
}

Channel<rcLib::Package> &LoRa::getChannelIn() {
    return in;
}

void LoRa::run() {
    rcLib::Package pkgIn, pkgOut;
    while (true) {
        loRaHal.waitForPackage();
        for(auto c=0; c<loRaHal.getLength(); c++) {
            if(pkgOut.decode(loRaHal.getData()[c])) {
                out.put(pkgOut);
            }
        }

        if(in.get(pkgIn, false)) {
            size_t len = pkgIn.encode();
            loRaHal.send(len, pkgIn.getEncodedData());
        }
    }
}
