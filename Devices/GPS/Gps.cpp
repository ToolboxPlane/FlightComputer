//
// Created by paul on 15.03.18.
//

#include "Gps.hpp"
#include <unistd.h>
#include <gps.h>

namespace device {
    Gps::Gps() {
        this->start();
    }

    void Gps::run() {
        gps_data_t gps_data{};

        if (gps_open("localhost", "2947", &gps_data) == -1) {
            throw std::runtime_error(gps_errstr(errno));
        }
        gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, nullptr);

        GpsMeasurement_t gps;
        while (true) {
            if (gps_waiting(&gps_data, 60000000)) {
                if (gps_read(&gps_data) == -1) {
                    fprintf(stderr, "error occured reading gps data, reason: %s\n", gps_errstr(errno));
                } else {
                    if ((gps_data.status == STATUS_FIX) &&
                        (gps_data.fix.mode == MODE_2D || gps_data.fix.mode == MODE_3D) &&
                        !std::isnan(gps_data.fix.latitude) &&
                        !std::isnan(gps_data.fix.longitude)) {

                        gps.fixAquired = true;

                        gps.location.lat = gps_data.fix.latitude;
                        gps.location.lon = gps_data.fix.longitude;
                        gps.timestamp = gps_data.fix.time;
                        gps.speed = gps_data.fix.speed;
                        gps.climb = gps_data.fix.climb;
                        gps.location.altitude = gps_data.fix.altitude;

                        out.put(gps);
                    } else {
                        gps.fixAquired = false;
                        out.put(gps);
                    }
                }
            }
        }
    }

    auto Gps::getChannelOut() -> OutputChannel<GpsMeasurement_t>&  {
        return out;
    }
}
