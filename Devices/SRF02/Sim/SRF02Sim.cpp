/**
 * @file SRF02Sim.cpp
 * @author paul
 * @date 26.03.20
 * Description here TODO
 */
#include "SRF02Sim.hpp"

namespace device {
    SRF02Sim::SRF02Sim() {
        start();
    }


    auto SRF02Sim::getChannelOut() -> OutputChannel<si::Meter<>> & {
        return out;
    }

    void SRF02Sim::run() {
        using namespace std::chrono_literals;
        while (true) {
            std::this_thread::sleep_for(66ms);

            uint16_t distance = 70;

            out.put(distance/100.0F * si::meter);
        }
    }

}
