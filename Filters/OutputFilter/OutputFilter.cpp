//
// Created by paul on 17.03.18.
//

#include "OutputFilter.hpp"
#include "../../Devices/rcLib/PackageOstream.hpp"

namespace filter {
    OutputFilter::OutputFilter() {
        this->start();
    }

    OutputChannel<rcLib::Package> &OutputFilter::getBaseOut() {
        return baseOut;
    }

    OutputChannel<rcLib::Package> &OutputFilter::getFlightControllerOut() {
        return flightControllerOut;
    }

    InputChannel<Control_t> &OutputFilter::getChannelIn() {
        return in;
    }

    void OutputFilter::run() {
        Control_t control{};
        rcLib::Package serialOutPkg(1024, 4);
        rcLib::Package baseOutPkg(1024, 4);

        baseOutPkg.setMeshProperties(static_cast<uint8_t>(true), 2);

        while (!in.isClosed()) {
            if (in.get(control)) {
                serialOutPkg.setChannel(0, static_cast<uint16_t>(control.power * 1023));
                serialOutPkg.setChannel(1, static_cast<uint16_t>(control.pitch + 180));
                serialOutPkg.setChannel(2, static_cast<uint16_t>(control.roll + 180));
                serialOutPkg.setChannel(3, 0);

                baseOutPkg.setChannel(0, -control.state.navPackage.rssi);
                baseOutPkg.setChannel(1, static_cast<uint16_t>(control.state.altitudeAboveGround));
                baseOutPkg.setChannel(2, static_cast<uint16_t>(control.state.lat * 10));
                baseOutPkg.setChannel(3, static_cast<uint16_t>(control.state.lon * 10));

                flightControllerOut.put(serialOutPkg);
                baseOut.put(baseOutPkg);
            }
        }
    }
}
