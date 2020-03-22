/**
 * @file Network.cpp
 * @author paul
 * @date 18.03.19
 * @brief Network @TODO
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <utility>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "Network.hpp"

device::Network::Network(std::string address) : address{std::move(address)} {
    this->fd = socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK, PROTOCOL_ID);
    if (this->fd < 0) {
        throw std::runtime_error(std::string{"Network: "} + strerror(errno));
    }

    this->start();
}

void device::Network::run() {
    std::array<uint8_t, 512> buffer{};
    rcLib::Package pkgIn{};
    rcLib::Package pkgOut{};
    sockaddr_in sockaddrIn{};
    sockaddrIn.sin_family = AF_INET;
    if (inet_pton(AF_INET, this->address.c_str(), &sockaddrIn.sin_addr) != 1) {
        throw std::runtime_error(std::string{"Network: "} + strerror(errno));
    }

    while (!in.isClosed()) {
        auto received = recv(this->fd, buffer.data(), buffer.size(), 0);
        if (received > 0) {
            auto *ipHeader = reinterpret_cast<iphdr*>(buffer.data());
            for (long c = ipHeader->ihl*4u; c < received; c++) {
                if (pkgIn.decode(buffer[c])) {
                    out.put(pkgIn);
                }
            }
        }

        while (in.get(pkgOut, false)) {
            auto len = pkgOut.encode();
            sendto(this->fd, pkgOut.getEncodedData(), len, 0,
                   reinterpret_cast<sockaddr*>(&sockaddrIn), sizeof(sockaddrIn));
        }

        std::this_thread::yield();
    }
}

auto device::Network::getChannelIn() -> InputChannel<rcLib::Package> & {
    return in;
}

auto device::Network::getChannelOut() -> OutputChannel<rcLib::Package> & {
    return out;
}
