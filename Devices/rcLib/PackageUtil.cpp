/**
 * @file PackageOstream.cpp
 * @author paul
 * @date 21.12.19
 * @brief PackageOstream @TODO
 */

#include <cassert>
#include <iostream>
#include "PackageUtil.hpp"

std::ostream &operator<<(std::ostream &ostream, const rcLib::Package &package) {
    ostream << "Sender: " << static_cast<int>(package.getDeviceId()) << "\tChannel: " << package.getChannelCount();
    ostream << "\tResolution: " << package.getResolution() << "\tData: (";
    for (auto c = 0; c < package.getChannelCount(); c++) {
        if (c > 0) {
            ostream << ", ";
        }
        ostream << package.getChannel(c);
    }
    ostream << ")";
    return ostream;
}

namespace recording {
    template <>
    auto header<rcLib::Package>() -> std::string {
        return "Transmitter-ID; Resolution; Channel-Count; Channel-Data";
    }

    template <>
    auto getLine<rcLib::Package>(const rcLib::Package &package) -> std::string {
        std::stringstream stringstream;
        stringstream << static_cast<int>(package.getDeviceId()) << "; ";
        stringstream << package.getResolution() << "; ";
        stringstream << package.getChannelCount();

        for(auto c=0; c<package.getChannelCount(); c++) {
            stringstream << "; " << package.getChannel(static_cast<uint8_t>(c));
        }
        return stringstream.str();
    }

    template <>
    auto getItem<rcLib::Package>(const std::vector<std::string> &line) -> rcLib::Package {
        assert(line.size() > 3);
        auto deviceId = static_cast<uint8_t>(std::stoi(line[0]));
        auto resolution = static_cast<uint16_t>(std::stoi(line[1]));
        auto channelCount = static_cast<uint8_t>(std::stoi(line[2]));

        assert(line.size() == (3u + channelCount));

        rcLib::Package pkg{resolution, channelCount};
        pkg.setDeviceId(deviceId);

        for (auto c=0; c<channelCount; ++c) {
            pkg.setChannel(c, std::stoi(line[c+3]));
        }

        return pkg;
    }
}
