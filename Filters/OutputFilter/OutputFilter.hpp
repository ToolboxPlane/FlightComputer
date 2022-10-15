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

namespace filter {
    class OutputFilter : public Node {
      public:
        OutputFilter();

        OutputChannel<rcLib::Package> &getBaseOut();

        OutputChannel<rcLib::Package> &getFlightControllerOut();

        OutputChannel<rcLib::Package> &getNetworkOut();

        InputChannel<Control_t> &getChannelIn();

      private:
        void run() override;

        OutputChannel<rcLib::Package> flightControllerOut, baseOut, networkOut;
        InputChannel<Control_t> in;
    };
} // namespace filter

#endif // FLIGHTCOMPUTER_OUTPUTFILTER_HPP
