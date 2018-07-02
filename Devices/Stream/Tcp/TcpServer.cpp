//
// Created by paul on 01.07.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include "TcpServer.hpp"

TcpServer::TcpServer(uint16_t port, const int BUF_SIZE) : StreamDevice(BUF_SIZE), fileDescriptor(-1) {
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFileDescriptor < 0) {
        throw std::ios_base::failure("Could not create socket");
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if(bind(socketFileDescriptor, (sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::ios_base::failure("Could not bind socket");
    }

    if(listen(socketFileDescriptor, 1) < 0) {
        throw std::ios_base::failure("Could not open socket");
    }

    this->start();
}

int TcpServer::getFileDescriptor() {
    if(fileDescriptor == -1) {
        struct sockaddr_in clientAddress{};
        socklen_t clientAddressLen = 0;
        fileDescriptor = accept(socketFileDescriptor, (sockaddr*)&clientAddress, &clientAddressLen);
        if(fileDescriptor < 0) {
            throw std::ios_base::failure("Could not accept connection");
        }
    }
    return fileDescriptor;
}
