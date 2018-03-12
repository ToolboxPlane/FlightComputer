//
// Created by paul on 12.03.18.
//

#include <iostream>
#include "RcLibLogger.hpp"

RcLibLogger::RcLibLogger(Channel<rcLib::Package> &channel)
        : channel(channel){
}

void RcLibLogger::run() {
    while(!channel.isClosed()) {
        rcLib::Package pkg;
        if(channel.get(pkg)) {
            std::cout << "Received package Sender: " << (int)pkg.getDeviceId() << "\t(";

            for(uint8_t c=0; c<pkg.getChannelCount(); c++) {
                std::cout << pkg.getChannel(c);
                if(c + 1 < pkg.getChannelCount()) {
                    std::cout << ", ";
                }
            }
            std::cout << ")" << std::endl;
        }
    }
}
