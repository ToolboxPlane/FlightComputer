//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPSSIMULATOR_HPP
#define FLIGHTCOMPUTER_GPSSIMULATOR_HPP


#include "../../../Framework/InputChannel.hpp"
#include "../../../Framework/Node.hpp"
#include "../../../Framework/OutputChannel.hpp"
#include "../Type/GpsMeasurement_t.hpp"

namespace device {
    class GpsSimulator : public Node {
      public:
        explicit GpsSimulator(int intervalMs = 500);

        auto getChannelOut() -> OutputChannel<GpsMeasurement_t> &;

      private:
        OutputChannel<GpsMeasurement_t> out;

        void run() override;

        const int intervalMs;
    };
} // namespace device

#endif // FLIGHTCOMPUTER_GPSSIMULATOR_HPP
