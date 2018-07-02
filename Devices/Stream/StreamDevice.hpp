//
// Created by paul on 01.07.18.
//

#ifndef FLIGHTCOMPUTER_STREAMDEVICE_HPP
#define FLIGHTCOMPUTER_STREAMDEVICE_HPP


#include "../../Filter.hpp"
#include "../../MultipleOutputChannel.hpp"
#include "../rcLib/PackageExtended.hpp"

class StreamDevice : public Filter {
public:
    explicit StreamDevice(const int BUF_SIZE) : BUF_SIZE(BUF_SIZE) {};
    Channel<rcLib::PackageExtended> &getChannelIn();
    MultipleOutputChannel<rcLib::PackageExtended> &getChannelOut();
private:
    Channel<rcLib::PackageExtended> in;
    MultipleOutputChannel<rcLib::PackageExtended> out;
    void run() override;
    const int BUF_SIZE;
protected:
    virtual int getFileDescriptor() = 0;
};


#endif //FLIGHTCOMPUTER_STREAMDEVICE_HPP
