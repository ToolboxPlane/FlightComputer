//
// Created by paul on 01.07.18.
//

#include "StreamDevice.hpp"
#include <unistd.h>

InputChannel<rcLib::PackageExtended> &StreamDevice::getChannelIn() {
    return in;
}

OutputChannel<rcLib::PackageExtended> &StreamDevice::getChannelOut() {
    return out;
}

void StreamDevice::run() {
    rcLib::PackageExtended toSend;
    rcLib::PackageExtended received;
    uint8_t buf[BUF_SIZE];

    while(true) {
        ssize_t readed = read(this->getFileDescriptor(), buf, sizeof(buf));
        if(readed > 0) {
            for (auto c = 0; c < readed; c++) {
                if (received.decode(buf[c])) {
                    out.put(received);
                }
            }
        } else if (readed == 0) {
            std::this_thread::yield();
        } else {
            if(errno == EPIPE) {
                this->invalidateFileDescriptor();
            }
        }

        if(in.get(toSend, false)) {
            size_t length = toSend.encode();
            ssize_t written = 0;
            do {
                ssize_t result = write(this->getFileDescriptor(), toSend.getEncodedData() + written, length);
                if(result < 0) {
                    if(ignoreErrors) {
                        if(errno == EPIPE) {
                            this->invalidateFileDescriptor();
                        }
                        break;
                    } else {
                        throw std::ios_base::failure("Error sending data!");
                    }
                } else if(result == 0) {
                    std::this_thread::yield();
                } else {
                    written += result;
                }
            } while (written < length);
        }
    }
}
