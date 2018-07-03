//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
#define FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP

#include "../../../Channel.hpp"
#include "../../rcLib/PackageExtended.hpp"
#include "../../../Filter.hpp"
#include "../../../MultipleOutputChannel.hpp"
#include "../StreamDevice.hpp"

class Serial : public StreamDevice {
public:
    Serial(const std::string& port, int baud, int BUF_SIZE = 20);
private:
    void setBlocking(bool isBlocking);
    void setAttributes(int baud, int parity, int timeoutMs = 500);
    int fileDescriptor;

protected:
    int getFileDescriptor() override;
    void invalidateFileDescriptor() override;
};


#endif //FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
