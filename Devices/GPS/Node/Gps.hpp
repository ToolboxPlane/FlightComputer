//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPS_HPP
#define FLIGHTCOMPUTER_GPS_HPP

#include <string>
#include <gps.h>

#include "../Type/GpsMeasurement_t.hpp"
#include "../../../Node.hpp"
#include "../../../InputChannel.hpp"
#include "../../../OutputChannel.hpp"

namespace device {
    class Gps : public Node {
        public:
            Gps();

            auto getChannelOut() -> OutputChannel<GpsMeasurement_t> &;

        private:
            void run() override;

            OutputChannel<GpsMeasurement_t> out;
    };
}

#endif //FLIGHTCOMPUTER_GPS_HPP
