//
// Created by paul on 01.07.18.
//

#ifndef FLIGHTCOMPUTER_STREAMDEVICE_HPP
#define FLIGHTCOMPUTER_STREAMDEVICE_HPP


#include "../../Filter.hpp"
#include "../../OutputChannel.hpp"
#include "../rcLib/PackageExtended.hpp"

class StreamDevice : public Filter {
public:
    StreamDevice(const int BUF_SIZE, const  bool ignoreErrors = false) :
            BUF_SIZE(BUF_SIZE), ignoreErrors(ignoreErrors) {};
    InputChannel<rcLib::PackageExtended> &getChannelIn();
    OutputChannel<rcLib::PackageExtended> &getChannelOut();
private:
    InputChannel<rcLib::PackageExtended> in;
    OutputChannel<rcLib::PackageExtended> out;
    void run() override;
    const int BUF_SIZE;
    const bool ignoreErrors;
protected:
    virtual int getFileDescriptor() = 0;
    virtual void invalidateFileDescriptor() = 0;
};


#endif //FLIGHTCOMPUTER_STREAMDEVICE_HPP
