//
// Created by paul on 17.03.18.
//

#include "OutputFilter.hpp"

#include <SiBase.hpp>

#include "../../Devices/rcLib/PackageUtil.hpp"

namespace filter {
    using namespace si::literals;

    OutputFilter::OutputFilter() {
        this->start();
    }

    OutputChannel<FlightControllerSetpoint> &OutputFilter::getFlightControllerOut() {
        return flightControllerOut;
    }

    InputChannel<Control> &OutputFilter::getChannelIn() {
        return in;
    }

    void OutputFilter::run() {
        Control control{};
        while (!in.isClosed()) {
            if (in.get(control)) {
                FlightControllerSetpoint flightControllerSetpoint{
                        .power = control.power,
                        .pitch = control.pitch,
                        .roll = control.roll,
                };
                flightControllerOut.put(flightControllerSetpoint);
            }
        }
    }

} // namespace filter
