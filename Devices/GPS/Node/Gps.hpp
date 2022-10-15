//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPS_HPP
#define FLIGHTCOMPUTER_GPS_HPP

#include "../../../Framework/InputChannel.hpp"
#include "../../../Framework/Node.hpp"
#include "../../../Framework/OutputChannel.hpp"
#include "../Type/GpsMeasurement_t.hpp"

namespace device {
    class Gps : public Node {
      public:
        Gps();

        auto getChannelOut() -> OutputChannel<GpsMeasurement_t> &;

      private:
        void run() override;

        OutputChannel<GpsMeasurement_t> out;
    };
} // namespace device

#endif // FLIGHTCOMPUTER_GPS_HPP
