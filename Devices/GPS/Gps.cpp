//
// Created by paul on 15.03.18.
//

#include "Gps.hpp"

Gps::Gps(std::string port, int baud) : ThreadModule() {
}

Channel<gps_t> &Gps::getChannelOut() {
    return out;
}