/**
 * @file MessageDesription.hpp
 * @author paul
 * @date 20.10.22
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_MESSAGECONVERSION_HPP
#define FLIGHTCOMPUTER_MESSAGECONVERSION_HPP

#include <FlightComputer.pb.h>
#include <typeinfo>

#include "../../Fusion/Type/State_t.hpp"
#include "../../OutputFilter/OutputTypes.hpp"

namespace lib::messages {
    template<typename T>
    struct Conversion {};

    template<>
    struct Conversion<FlightControllerPackage> {
        using ThisType = FlightControllerPackage;
        using ProtobufType = ToolboxPlaneMessages_FlightController;
        static constexpr const pb_msgdesc_s *description = ToolboxPlaneMessages_FlightController_fields;
        static constexpr uint8_t ID = 0x10;

        static auto fromProtobuf(const ProtobufType &obj) -> ThisType {
            return ThisType{
                    .bnoState = static_cast<uint8_t>(obj.bnoState),
                    .bnoError = static_cast<uint8_t>(obj.bnoError),
                    .sysCalibStatus = static_cast<CalibStatus>((obj.bnoCalibStat >> 6u) & 3u),
                    .gyroCalibStatus = static_cast<CalibStatus>((obj.bnoCalibStat >> 4u) & 3u),
                    .accCalibStatus = static_cast<CalibStatus>((obj.bnoCalibStat >> 2u) & 3u),
                    .magCalibStatus = static_cast<CalibStatus>((obj.bnoCalibStat >> 6u) & 3u),
                    .roll = (obj.roll - 500) / 2.0F,
                    .pitch = (obj.pitch - 500) / 2.0F,
                    .yaw = (obj.yaw - 500) / 2.0f,
                    .rollDeriv = (obj.dRoll - 500) / 16.0F * si::hertz,
                    .pitchDeriv = (obj.dPitch - 500) / 16.0F * si::hertz,
                    .yawDeriv = (obj.dYaw - 500) / 16.0F * si::hertz,
                    .accX = (obj.accX - 500) / 6.25F * si::acceleration,
                    .accY = (obj.accY - 500) / 6.25F * si::acceleration,
                    .accZ = (obj.accZ - 500) / 6.25F * si::acceleration,
                    .elevonLeft = (obj.servoLeft - 500) / 500.0f,
                    .elevonRight = (obj.servoRight - 500) / 500.0F,
                    .motor = obj.motor / 1000.0F,
            };
        }
    };

    template<>
    struct Conversion<PdbPackage> {
        using ThisType = PdbPackage;
        using ProtobufType = ToolboxPlaneMessages_PDB;
        static constexpr const pb_msgdesc_s *description = ToolboxPlaneMessages_PDB_fields;
        static constexpr uint8_t ID = 0x30;

        static auto fromProtobuf(const ProtobufType &obj) -> ThisType {
            return ThisType{
                    .voltageVcc = static_cast<si::default_type>(obj.v_vcc) * 128 / 1000.0f * si::volt,
                    .currentVcc = static_cast<si::default_type>(obj.i_vcc) * 256 / 1000.0f * si::ampere,
                    .voltage5V = static_cast<si::default_type>(obj.v_5v) * 32 / 1000.0f * si::volt,
                    .current5V = static_cast<si::default_type>(obj.i_5v) * 64 / 1000.0f * si::ampere,
            };
        }
    };

    template<>
    struct Conversion<TaranisPackage> {
        using ThisType = TaranisPackage;
        using ProtobufType = ToolboxPlaneMessages_Taranis;
        static constexpr const pb_msgdesc_s *description = ToolboxPlaneMessages_Taranis_fields;
        static constexpr uint8_t ID = 0x20;

        static auto fromProtobuf(const ProtobufType &obj) -> ThisType {
            return ThisType{.throttle = obj.throttleRaw,
                            .pitch = obj.pitchRaw,
                            .roll = obj.rollRaw,
                            .isArmed = obj.isArmed,
                            .manualOverrideActive = obj.overrideActive,
                            .rssi = obj.rssi};
        }
    };

    template<>
    struct Conversion<FlightControllerSetpoint> {
        using ThisType = FlightControllerSetpoint;
        using ProtobufType = ToolboxPlaneMessages_FlightControllerSetpoint;
        static constexpr const pb_msgdesc_s *description = ToolboxPlaneMessages_FlightController_fields;
        static constexpr uint8_t ID = 0x40;

        static auto toProtobuf(const ThisType &obj) -> ProtobufType {
            return ProtobufType{
                    .power = static_cast<int32_t>(static_cast<float>(obj.power) * 1023),
                    .pitch = static_cast<int32_t>(static_cast<float>(obj.pitch) + 180.0F),
                    .roll = static_cast<int32_t>(static_cast<float>(obj.roll) + 180),
            };
        }
    };

} // namespace lib::messages

#undef TYPE_MAPPING

#endif // FLIGHTCOMPUTER_MESSAGECONVERSION_HPP
