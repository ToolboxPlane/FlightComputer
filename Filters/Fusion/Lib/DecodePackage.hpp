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

using namespace si::base;
using namespace si::extended;

namespace fusion {
    auto normalizeTaranis(int input) -> float {
        return (static_cast<float>(input) - 172) / (1811 - 172);
    }

    auto getSwitchPos(int input) -> SwitchPos {
        auto val = normalizeTaranis(input);
        if (val < 1.0/3.0) {
            return SwitchPos::UP;
        } else if (val < 2.0/3.0) {
            return SwitchPos::CENTRE;
        } else {
            return SwitchPos::DOWN;
        }
    }

    template<typename T>
    auto decodePackage(const rcLib::Package&) -> T {
        static_assert("Cannot decode package to type");
    }

    template<>
    auto decodePackage<FlightControllerPackage>(const rcLib::Package &pkg) -> FlightControllerPackage {
        FlightControllerPackage flightControllerPackage{};
        flightControllerPackage.bnoState = pkg.getChannel(0);
        flightControllerPackage.roll = (static_cast<float>(pkg.getChannel(1)) - 500) / 2.0f;
        flightControllerPackage.pitch = (static_cast<float>(pkg.getChannel(2)) -500) / 2.0f;
        flightControllerPackage.yaw = (static_cast<float>(pkg.getChannel(3)) - 500) / 2.0f;
        flightControllerPackage.rollDeriv = (static_cast<float>(pkg.getChannel(4)) - 500) * hertz;
        flightControllerPackage.pitchDeriv = (static_cast<float>(pkg.getChannel(5)) -500) * hertz;
        flightControllerPackage.yawDeriv = (static_cast<float>(pkg.getChannel(6)) - 500) * hertz;

        // @TODO fix scaling
        flightControllerPackage.motor = pkg.getChannel(13);
        flightControllerPackage.elevonLeft = static_cast<float>(pkg.getChannel(14)) -  500;
        flightControllerPackage.elevonRight = static_cast<float>(pkg.getChannel(15)) - 500;

        return flightControllerPackage;
    }

    template<>
    auto decodePackage<PdbPackage>(const rcLib::Package &pkg) -> PdbPackage {
        PdbPackage pdbPackage{};
        pdbPackage.status = pkg.getChannel(0);
        pdbPackage.voltageVcc = static_cast<float>(pkg.getChannel(1)) * 128 / 1000.0f * volt;
        pdbPackage.currentVcc = static_cast<float>(pkg.getChannel(2)) * 256 / 1000.0f * ampere;
        pdbPackage.voltage5V = static_cast<float>(pkg.getChannel(3)) * 32 / 1000.0f * volt;
        pdbPackage.current5V = static_cast<float>(pkg.getChannel(4)) * 64 / 1000.0f * ampere;

        return pdbPackage;
    }

    template<>
    auto decodePackage<TaranisPackage>(const rcLib::Package &pkg) -> TaranisPackage {
        TaranisPackage taranisPackage{};
        taranisPackage.throttle = normalizeTaranis(pkg.getChannel(3));
        taranisPackage.pitch = normalizeTaranis(pkg.getChannel(4));
        taranisPackage.roll = normalizeTaranis(pkg.getChannel(5));
        taranisPackage.isArmed = getSwitchPos(pkg.getChannel(6)) == SwitchPos::DOWN;
        taranisPackage.manualOverrideActive = getSwitchPos(pkg.getChannel(7)) != SwitchPos::DOWN;
        taranisPackage.rssi = normalizeTaranis(pkg.getChannel(15));

        return taranisPackage;
    }

    template<>
    auto decodePackage<LoraPackage>(const rcLib::Package &pkg) -> LoraPackage {
        LoraPackage loraPackage{};
        loraPackage.joyRightX = static_cast<float>(pkg.getChannel(0) - 127) / 127.0f;
        loraPackage.joyRightY = static_cast<float>(pkg.getChannel(1) - 127) / 127.0f;
        loraPackage.joyLeftX = static_cast<float>(pkg.getChannel(2) - 127) / 127.0f;
        loraPackage.joyLeftY = static_cast<float>(pkg.getChannel(3) - 127) / 127.0f;
        loraPackage.flightMode = static_cast<FlightMode>(pkg.getChannel(4));
        loraPackage.isArmed = static_cast<bool>(pkg.getChannel(5));

        return loraPackage;
    }
}

#endif //FLIGHTCOMPUTER_DECODEPACKAGE_HPP
