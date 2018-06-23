//
// Created by paul on 19.03.18.
//

#ifndef FLIGHTCOMPUTER_PACKAGEEXTENDED_HPP
#define FLIGHTCOMPUTER_PACKAGEEXTENDED_HPP

#include "RadioControlProtocol/rcLib.hpp"
#include "../../Utilities/json.hpp"
#include <iostream>

using json = nlohmann::json;

namespace rcLib {
    class PackageExtended : public Package{
    public:
        PackageExtended() : Package() {};
        PackageExtended(uint16_t r, uint8_t c) : Package(r,c){}

        explicit PackageExtended(const std::vector<std::string> &items) {
            this->tid = static_cast<uint8_t>(std::stoi(items[0]));
            this->channelCount = static_cast<uint16_t>(items.size() - 1);

            for(auto c=0; c<items.size(); c++) {
                this->channelData[c-1] = static_cast<uint16_t>(std::stoi(items[c]));
            }
        }

        static std::string header() {
            std::stringstream stringstream;
            stringstream << "Transmitter-ID";
            for(auto c=0; c<16; c++) {
                stringstream << "; " << "Channel " << c;
            }
            return stringstream.str();
        }

        std::string getLine() {
            std::stringstream stringstream;
            stringstream << (int)this->tid;

            for(auto c=0; c<this->channelCount; c++) {
                stringstream << "; " << this->getChannel(static_cast<uint8_t>(c));
            }
            return stringstream.str();
        }

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
