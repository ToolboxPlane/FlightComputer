//
// Created by paul on 19.03.18.
//

#ifndef FLIGHTCOMPUTER_PACKAGEEXTENDED_HPP
#define FLIGHTCOMPUTER_PACKAGEEXTENDED_HPP

#include "RadioControlProtocol/rcLib.hpp"
#include <iostream>

namespace rcLib {
    class PackageExtended : public Package{
    public:
        PackageExtended() : Package() {};
        PackageExtended(uint16_t r, uint8_t c) : Package(r,c){}

        friend std::ostream &rcLib::operator<<(std::ostream &ostream, PackageExtended package) {
            ostream << "Sender: " << (int)package.getDeviceId() << "\tChannel: " << package.getChannelCount();
            ostream << "\tResolution: " << package.getResolution() << "\tData: (";
            for(uint8_t c = 0; c<package.getChannelCount(); c++) {
                if(c > 0) {
                    ostream << ", ";
                }
                ostream << package.getChannel(c);
            }
            ostream << ")";
            return ostream;
        }
    };
}


#endif //FLIGHTCOMPUTER_PACKAGEEXTENDED_HPP
