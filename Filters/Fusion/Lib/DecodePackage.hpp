/**
 * @file DecodePackage.hpp
 * @author paul
 * @date 21.12.19
 * @brief DecodePackage @TODO
 */

#ifndef FLIGHTCOMPUTER_DECODEPACKAGE_HPP
#define FLIGHTCOMPUTER_DECODEPACKAGE_HPP

#include "../Type/State_t.hpp"
#include "../../../Devices/rcLib/RadioControlProtocolCpp/rcLib.hpp"

namespace fusion {
    template<typename T>
    auto decodePackage(const rcLib::Package&) -> T {
        static_assert("Cannot decode package to type");
    }

    template<>
    auto decodePackage<FlightControllerPackage>(const rcLib::Package &pkg) -> FlightControllerPackage {
        FlightControllerPackage flightControllerPackage{};
        flightControllerPackage.bnoState = pkg.getChannel(0);
        flightControllerPackage.roll = pkg.getChannel(1) - 500;
        flightControllerPackage.pitch = pkg.getChannel(2) -500;
        flightControllerPackage.yaw = pkg.getChannel(3) - 500;
        flightControllerPackage.rollDeriv = pkg.getChannel(4) - 500;
        flightControllerPackage.pitchDeriv = pkg.getChannel(5) -500;
        flightControllerPackage.yawDeriv = pkg.getChannel(6) - 500;

        flightControllerPackage.aileronRight = pkg.getChannel(11) -  500;
        flightControllerPackage.vtailRight = pkg.getChannel(12) - 500;
        flightControllerPackage.motor = pkg.getChannel(13);
        flightControllerPackage.vtailLeft = pkg.getChannel(14) - 500;
        flightControllerPackage.aileronLeft = pkg.getChannel(15) - 500;

        return flightControllerPackage;
    }
}

#endif //FLIGHTCOMPUTER_DECODEPACKAGE_HPP
