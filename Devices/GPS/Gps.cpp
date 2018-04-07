//
// Created by paul on 15.03.18.
//

#include "Gps.hpp"
#include <unistd.h>
#include <gps.h>

#define BUF_SIZE 80

Gps::Gps() : ThreadModule(){
    gps_data_t gps_data;
    if ((rc = gps_open("localhost", "2947", &gps_data)) == -1) {
        fprintf(stderr, "code: %d, reason: %s\n", rc, gps_errstr(rc));
        return -1;
    }
    gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);
}

Channel<Gps_t> &Gps::getChannelOut() {
    return out;
}

void Gps::run() {
    Gps_t gps;
    gps_data_t gps_data;
    while(!out.isClosed()) {
        /* wait for 60 seconds to receive data */
        if (gps_waiting (&gps_data, 60000000)) {
            /* read data */
            if ((rc = gps_read(&gps_data)) == -1) {
                fprintf(stderr, "error occured reading gps data. code: %d, reason: %s\n", rc, gps_errstr(rc));
            } else {
                /* Display data from the GPS receiver. */
                if ((gps_data.status == STATUS_FIX) &&
                    (gps_data.fix.mode == MODE_2D || gps_data.fix.mode == MODE_3D) &&
                    !isnan(gps_data.fix.latitude) &&
                    !isnan(gps_data.fix.longitude)) {

                    gps.lat = gps_data.fix.latitude;
                    gps.lon = gps_data.fix.longitude;
                    gps.timestamp = gps_data.fix.timestamp;
                    gps.speed = gps_data.fix.speed;
                }
            }
        }

    }
}
