//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_OUTPUTFILTER_HPP
#define FLIGHTCOMPUTER_OUTPUTFILTER_HPP


#include "../../Node.hpp"
#include "../../Devices/rcLib/RadioControlProtocolCpp/rcLib.hpp"
#include "../FeedbackControl/Type/Control_t.hpp"
#include "../../InputChannel.hpp"
#include "../../OutputChannel.hpp"

namespace filter {
    class OutputFilter : public Node {
    public:
        OutputFilter();

        OutputChannel<rcLib::Package> &getBaseOut();

        OutputChannel<rcLib::Package> &getFlightControllerOut();

        InputChannel<Control_t> &getChannelIn();

    private:
        void run() override;

        OutputChannel<rcLib::Package> flightControllerOut, baseOut;
        InputChannel<Control_t> in;
    };
}

#endif //FLIGHTCOMPUTER_OUTPUTFILTER_HPP
