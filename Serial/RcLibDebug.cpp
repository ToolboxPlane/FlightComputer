//
// Created by paul on 12.03.18.
//

#include <iostream>
#include <future>
#include "RcLibDebug.hpp"


void RcLibDebug::run() {
    while(!channel.isClosed()) {
        rcLib::Package pkg;
        if(channel.get(pkg)) {
            std::cout << "[" << tag << "]\t";
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

Channel<rcLib::Package> &RcLibDebug::getChannelIn() {
    return channel;
}

RcLibDebug::RcLibDebug(const std::string &tag) : tag(tag), ThreadModule(){
}
