//
// Created by paul on 15.03.18.
//

#include "Gps.hpp"

#define BUF_SIZE 80

Gps::Gps(std::string port, int baud) : ThreadModule(), serial("/dev/tty?", 4800){
}

Channel<gps_t> &Gps::getChannelOut() {
    return out;
}

void Gps::run() {
    gps_t gps;
    uint8_t buf[BUF_SIZE];
    uint8_t line[BUF_SIZE];
    uint8_t head = 0;

    while(!serial.ready);
    while(!out.isClosed()) {
        size_t readed = read(serial.fd, buf, sizeof(buf));
        for(auto c=0; c<readed; c++) {
            if(c < readed - 1 && buf[c] == '\r' && buf[c+1] == '\n') {
                out.put(line, head);
            } else {
                line[head++] = buf[c];
            }
        }
    }
}

gps_t Gps::parseNmeaString(uint8_t* s, size_t n) {
    gps_t ret{};
    return ret;
}
