/**
 * @file DecodePackage.hpp
 * @author paul
 * @date 21.12.19
 * @brief DecodePackage @TODO
 */

#ifndef FLIGHTCOMPUTER_DECODEPACKAGE_HPP
#define FLIGHTCOMPUTER_DECODEPACKAGE_HPP

#include "../../../Devices/rcLib/RadioControlProtocolCpp/rcLib.hpp"
#include "../Type/State_t.hpp"

namespace fusion {
    auto normalizeTaranis(int input) -> si::Scalar<> {
        return (static_cast<si::default_type>(input) - 172) / (1811 - 172);
    }

    auto getSwitchPos(int input) -> SwitchPos {
        auto val = normalizeTaranis(input);
        if (val < 1 / 3.0F * si::scalar) {
            return SwitchPos::UP;
        } else if (val < 2 / 3.0F * si::scalar) {
            return SwitchPos::CENTRE;
        } else {
            return SwitchPos::DOWN;
        }
    }

    template<typename T>
    auto decodePackage(const rcLib::Package &) -> T {
        static_assert("Cannot decode package to type");
    }

    template<>
    auto decodePackage<FlightControllerPackage>(const rcLib::Package &pkg) -> FlightControllerPackage {
        FlightControllerPackage flightControllerPackage{};
        flightControllerPackage.bnoState = pkg.getChannel(0);
        flightControllerPackage.bnoError = pkg.getChannel(10);
        auto calibStat = pkg.getChannel(11);
        flightControllerPackage.sysCalibStatus = static_cast<CalibStatus>((calibStat >> 6u) & 3u);
        flightControllerPackage.gyroCalibStatus = static_cast<CalibStatus>((calibStat >> 4u) & 3u);
        flightControllerPackage.accCalibStatus = static_cast<CalibStatus>((calibStat >> 2u) & 3u);
        flightControllerPackage.magCalibStatus = static_cast<CalibStatus>((calibStat >> 6u) & 3u);
        flightControllerPackage.roll = (pkg.getChannel(1) - 500) / 2.0F;
        flightControllerPackage.pitch = (pkg.getChannel(2) - 500) / 2.0F;
        flightControllerPackage.yaw = (pkg.getChannel(3) - 500) / 2.0f;
        flightControllerPackage.rollDeriv = (pkg.getChannel(4) - 500) / 16.0F * si::hertz;
        flightControllerPackage.pitchDeriv = (pkg.getChannel(5) - 500) / 16.0F * si::hertz;
        flightControllerPackage.yawDeriv = (pkg.getChannel(6) - 500) / 16.0F * si::hertz;
        flightControllerPackage.accX = (pkg.getChannel(7) - 500) / 6.25F * si::acceleration;
        flightControllerPackage.accY = (pkg.getChannel(8) - 500) / 6.25F * si::acceleration;
        flightControllerPackage.accZ = (pkg.getChannel(9) - 500) / 6.25F * si::acceleration;

        flightControllerPackage.motor = pkg.getChannel(13) / 1000.0F;
        flightControllerPackage.elevonLeft = (pkg.getChannel(14) - 500) / 500.0f;
        flightControllerPackage.elevonRight = (pkg.getChannel(15) - 500) / 500.0F;

        return flightControllerPackage;
    }

    template<>
    auto decodePackage<PdbPackage>(const rcLib::Package &pkg) -> PdbPackage {
        PdbPackage pdbPackage{};
        pdbPackage.voltageVcc = static_cast<si::default_type>(pkg.getChannel(0)) * 128 / 1000.0f * si::volt;
        pdbPackage.currentVcc = static_cast<si::default_type>(pkg.getChannel(1)) * 256 / 1000.0f * si::ampere;
        pdbPackage.voltage5V = static_cast<si::default_type>(pkg.getChannel(2)) * 32 / 1000.0f * si::volt;
        pdbPackage.current5V = static_cast<si::default_type>(pkg.getChannel(3)) * 64 / 1000.0f * si::ampere;

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
        loraPackage.joyRightX = static_cast<si::default_type>(pkg.getChannel(0) - 127) / 127.0f;
        loraPackage.joyRightY = static_cast<si::default_type>(pkg.getChannel(1) - 127) / 127.0f;
        loraPackage.joyLeftX = static_cast<si::default_type>(pkg.getChannel(2) - 127) / 127.0f;
        loraPackage.joyLeftY = static_cast<si::default_type>(pkg.getChannel(3) - 127) / 127.0f;
        loraPackage.flightMode = static_cast<FlightMode>(pkg.getChannel(4));
        loraPackage.isArmed = static_cast<bool>(pkg.getChannel(5));
        loraPackage.rssi = -pkg.getChannel(7);

        return loraPackage;
    }

    template<>
    auto decodePackage<NavPackage>(const rcLib::Package &pkg) -> NavPackage {
        NavPackage navPackage;
        navPackage.rssi = -pkg.getChannel(0);
        navPackage.baroAltitude = static_cast<float>(pkg.getChannel(1)) * si::meter;
        navPackage.pitotVoltage = static_cast<float>(pkg.getChannel(2)) * si::volt;
        navPackage.usDistance = (pkg.getChannel(3) / 100.0F) * si::meter;

        return navPackage;
    }
} // namespace fusion

#endif // FLIGHTCOMPUTER_DECODEPACKAGE_HPP
