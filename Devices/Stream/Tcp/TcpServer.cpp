//
// Created by paul on 01.07.18.
//

#include "TcpServer.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <csignal>
#include <unistd.h>

TcpServer::TcpServer(uint16_t port, const int BUF_SIZE) : StreamDevice(BUF_SIZE, true), fileDescriptor(-1) {
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
        signal(SIGPIPE, SIG_IGN); // Ignore SIGPIPE
        struct sockaddr_in clientAddress{};
        socklen_t clientAddressLen = 0;
        while ((fileDescriptor =
                accept(socketFileDescriptor, (sockaddr*)&clientAddress, &clientAddressLen)) < 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        fcntl(fileDescriptor, F_SETFL, O_NONBLOCK); // Set the file descriptor non blocking
    }
    return fileDescriptor;
}

void TcpServer::invalidateFileDescriptor() {
    close(fileDescriptor);
    fileDescriptor = -1;
}
