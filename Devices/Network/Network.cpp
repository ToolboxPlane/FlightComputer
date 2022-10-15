/**
 * @file Network.cpp
 * @author paul
 * @date 18.03.19
 * @brief Network @TODO
 */

#include "Network.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <utility>

device::Network::Network(const std::string &address, bool enable) {
    if (!enable) {
        return;
    }

    this->fd = socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK, PROTOCOL_ID);
    if (this->fd < 0) {
        throw std::runtime_error(std::string{"Network: "} + strerror(errno));
    }
    sockaddrIn.sin_family = AF_INET;
    if (inet_pton(AF_INET, address.c_str(), &sockaddrIn.sin_addr) != 1) {
        throw std::runtime_error(std::string{"Network: "} + strerror(errno));
    }

    this->start();
}

void device::Network::run() {
    rcLib::Package pkgOut{};

    while (!in.isClosed()) {
        if (in.get(pkgOut)) {
            auto len = pkgOut.encode();
            std::size_t written = 0;
            do {
                auto res = sendto(this->fd, pkgOut.getEncodedData() + written, len - written, 0,
                                  reinterpret_cast<sockaddr *>(&sockaddrIn), sizeof(sockaddrIn));

                if (res < 0) {
                    std::cerr << "[Network]:\t" << strerror(errno) << std::endl;
                } else {
                    written += res;
                }
            } while (written < len);
        }
    }
}

auto device::Network::getChannelIn() -> InputChannel<rcLib::Package> & {
    return in;
}
