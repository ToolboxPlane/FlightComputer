//
// Created by paul on 01.07.18.
//

#ifndef FLIGHTCOMPUTER_TCPSERVER_HPP
#define FLIGHTCOMPUTER_TCPSERVER_HPP


#include "../StreamDevice.hpp"

class TcpServer : public StreamDevice {
public:
    explicit TcpServer(uint16_t port, int BUF_SIZE = 64);

private:
    int socketFileDescriptor;
    int fileDescriptor;

protected:
    int getFileDescriptor() override;
};


#endif //FLIGHTCOMPUTER_TCPSERVER_HPP
