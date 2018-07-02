//
// Created by paul on 01.07.18.
//

#include "StreamDevice.hpp"
#include <unistd.h>

Channel<rcLib::PackageExtended> &StreamDevice::getChannelIn() {
    return in;
}

MultipleOutputChannel<rcLib::PackageExtended> &StreamDevice::getChannelOut() {
    return out;
}

void StreamDevice::run() {
    rcLib::PackageExtended toSend;
    rcLib::PackageExtended received;
    uint8_t buf[BUF_SIZE];

    while(true) {
        ssize_t readed = read(this->getFileDescriptor(), buf, sizeof(buf));
        for(auto c=0; c<readed; c++) {
            if(received.decode(buf[c])) {
                out.put(received);
            }
        }

        if(in.get(toSend, false)) {
            size_t length = toSend.encode();
            ssize_t written = 0;
            do {
                ssize_t result = write(this->getFileDescriptor(), toSend.getEncodedData(), length);
                if(result < 0) {
                    throw std::ios_base::failure("Error sending data!");
                }
                written += result;
            } while (written < length);
        }
    }
}
