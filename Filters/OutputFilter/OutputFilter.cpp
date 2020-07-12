//
// Created by paul on 17.03.18.
//

#include "OutputFilter.hpp"
#include "../../Devices/rcLib/PackageUtil.hpp"

#include <SiBase.hpp>

namespace filter {
    using namespace si::literals;

    OutputFilter::OutputFilter() {
        this->start();
    }

    OutputChannel<rcLib::Package> &OutputFilter::getBaseOut() {
        return baseOut;
    }

    OutputChannel<rcLib::Package> &OutputFilter::getFlightControllerOut() {
        return flightControllerOut;
    }

    OutputChannel<rcLib::Package> &OutputFilter::getNetworkOut() {
        return networkOut;
    }

    InputChannel<Control_t> &OutputFilter::getChannelIn() {
        return in;
    }

    void OutputFilter::run() {
        Control_t control{};
        rcLib::Package serialOutPkg(1024, 4);
        rcLib::Package baseOutPkg(1024, 4);
        rcLib::Package networkOutPkg(4096, 16);

        baseOutPkg.setMeshProperties(static_cast<uint8_t>(true), 2);

        while (!in.isClosed()) {
            if (in.get(control)) {
                auto angleToStart = control.state.position.angleTo(control.state.startLocation);
                auto distToStart = control.state.position.distanceTo(control.state.startLocation);


                serialOutPkg.setChannel(0, static_cast<uint16_t>(control.power * 1023));
                serialOutPkg.setChannel(1, static_cast<uint16_t>(control.pitch + 180));
                serialOutPkg.setChannel(2, static_cast<uint16_t>(control.roll + 180));
                serialOutPkg.setChannel(3, static_cast<uint16_t>(angleToStart * 2 + 500.0F * si::scalar));

                baseOutPkg.setChannel(0, -control.state.navPackage.rssi);
                baseOutPkg.setChannel(1, static_cast<uint16_t>(control.state.altitudeAboveGround));
                baseOutPkg.setChannel(2, static_cast<uint16_t>(angleToStart * 2 + 500.0F * si::scalar));
                baseOutPkg.setChannel(3, static_cast<uint16_t>(distToStart / 10));

                networkOutPkg.setChannel(0, -control.state.navPackage.rssi);
                networkOutPkg.setChannel(1, static_cast<uint16_t>(std::round(control.state.altitudeAboveGround * 40)));
                networkOutPkg.setChannel(2, static_cast<uint16_t>(
                        std::round(angleToStart * 10 + 2000 * si::scalar)));
                networkOutPkg.setChannel(3, static_cast<uint16_t>(std::round(distToStart)));
                networkOutPkg.setChannel(4, static_cast<uint16_t>(
                        std::round(control.state.roll * 10 + 2000 * si::scalar)));
                networkOutPkg.setChannel(5, static_cast<uint16_t>(
                        std::round(control.state.pitch * 10 + 2000 * si::scalar)));
                networkOutPkg.setChannel(6, static_cast<uint16_t>(
                        std::round(control.state.yaw * 10 + 2000 * si::scalar)));
                networkOutPkg.setChannel(7, static_cast<uint16_t>(std::round(control.state.speed * 40)));
                networkOutPkg.setChannel(8, static_cast<uint16_t>(std::round(control.state.position.altitude * 4)));
                networkOutPkg.setChannel(9, static_cast<uint16_t>(
                        std::round(control.state.accX * 40 + 2000_acceleration)));
                networkOutPkg.setChannel(10, static_cast<uint16_t>(
                        std::round(control.state.accY * 40 + 2000_acceleration)));
                networkOutPkg.setChannel(11, static_cast<uint16_t>(
                        std::round(control.state.accZ * 40 + 2000_acceleration)));
                networkOutPkg.setChannel(12, static_cast<uint16_t>(
                        std::round(control.state.pdbPackage.voltageVcc * 200)));

                flightControllerOut.put(serialOutPkg);
                baseOut.put(baseOutPkg);
                networkOut.put(networkOutPkg);
            }
        }
    }

}
