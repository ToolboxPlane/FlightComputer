#include <iostream>
#include <future>
#include "Serial/Serial.hpp"

int main() {
    Channel<rcLib::Package> serialIn, serialOut;
    Serial serial("/dev/ttyACM0", B9600, serialIn, serialOut);

    auto serialHandle = std::async(std::launch::async, &Serial::run, serial);

    rcLib::Package in;
    while(!serialIn.isClosed() && !serialOut.isClosed()) {
        if(serialOut.get(in)) {
            std::cout << "Received package Sender: " << (int)in.getDeviceId()
                    << "\t(";
            for(uint8_t c=0; c<in.getChannelCount(); c++) {
                std::cout << in.getChannel(c);
                if(c + 1 < in.getChannelCount()) {
                    std::cout  << ", ";
                }
            }
            std::cout << ")" << std::endl;
        }
    }

    return 0;
}