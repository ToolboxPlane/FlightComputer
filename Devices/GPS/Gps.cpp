//
// Created by paul on 15.03.18.
//

#include "Gps.hpp"
#include <unistd.h>
#include <gps.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

Gps::Gps() {
    this->start();
}

MultipleOutputChannel<GpsMeasurement_t> &Gps::getChannelOut() {
    return out;
}

void Gps::run() {
    if (gps_open("localhost", "3000", &gps_data) == -1) {
        fprintf(stderr, "reason: %s\n",gps_errstr(errno));
        exit(1);
    }
    gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, nullptr);

    GpsMeasurement_t gps;
    while(true) {
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

#pragma clang diagnostic pop
