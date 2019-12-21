/**
 * @file PackageOstream.cpp
 * @author paul
 * @date 21.12.19
 * @brief PackageOstream @TODO
 */

#include "PackageOstream.hpp"

std::ostream &operator<<(std::ostream &ostream, const rcLib::Package &package) {
    ostream << "Sender: " << static_cast<int>(package.getDeviceId()) << "\tChannel: " << package.getChannelCount();
    ostream << "\tResolution: " << package.getResolution() << "\tData: (";
    for(auto c = 0; c<package.getChannelCount(); c++) {
        if(c > 0) {
            ostream << ", ";
        }
        ostream << package.getChannel(c);
    }
    ostream << ")";
    return ostream;
}
