//
// Created by paul on 12.03.18.
//

#include <fcntl.h>
#include <bits/ios_base.h>
#include <unistd.h>
#include <iostream>
#include "../Serial.hpp"

#include "FlightController.hpp"

#define BUF_SIZE 64

FlightController::FlightController(const std::string& port, int baud)
        : serial(port, baud), in(), out(), ThreadModule() {
}

void FlightController::run() {
    rcLib::PackageExtended toSend;
    rcLib::PackageExtended received;
    uint8_t buf[BUF_SIZE];

    while(!serial.ready) ;
    while(true) {
        ssize_t readed = read(serial.fd, buf, sizeof(buf));
        for(auto c=0; c<readed; c++) {
            if(received.decode(buf[c])) {
                out.put(received);
            }
        }

        if(in.get(toSend, false)) {
            size_t length = toSend.encode();
            ssize_t written = 0;
            do {
                ssize_t result = write(serial.fd, toSend.getEncodedData(), length);
                if(result < 0) {
                    throw std::ios_base::failure("Error sending data via FlightController");
                }
                written += result;
            } while (written < length);
        }
    }
}

Channel<rcLib::PackageExtended> &FlightController::getChannelIn() {
    return this->in;
}

MultipleOutputChannel<rcLib::PackageExtended> &FlightController::getChannelOut() {
    return this->out;
}
