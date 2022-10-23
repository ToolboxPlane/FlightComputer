//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_OUTPUTFILTER_HPP
#define FLIGHTCOMPUTER_OUTPUTFILTER_HPP


#include "../../Devices/rcLib/RadioControlProtocolCpp/rcLib.hpp"
#include "../../Framework/InputChannel.hpp"
#include "../../Framework/Node.hpp"
#include "../../Framework/OutputChannel.hpp"
#include "../FeedbackControl/Type/Control_t.hpp"
#include "OutputTypes.hpp"

namespace filter {
    class OutputFilter : public Node {
      public:
        OutputFilter();

        OutputChannel<FlightControllerSetpoint> &getFlightControllerOut();

        //OutputChannel<> &getDebugOut();

        InputChannel<Control> &getChannelIn();

      private:
        void run() override;

        OutputChannel<FlightControllerSetpoint> flightControllerOut;
        InputChannel<Control> in;
    };
} // namespace filter

#endif // FLIGHTCOMPUTER_OUTPUTFILTER_HPP
