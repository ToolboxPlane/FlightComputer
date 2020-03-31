//
// Created by paul on 15.03.18.
//

#include "Gps.hpp"
#include "../../../../../../../../usr/include/gps.h"
#include <unistd.h>
#include <gps.h>

namespace device {
    Gps::Gps() {
        this->start();
    }

    void Gps::run() {
        gps_data_t gps_data{};

        if (gps_open("localhost", "2947", &gps_data) == -1) {
            throw std::runtime_error(std::string{"GPS: "} + gps_errstr(errno));
        }
        gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, nullptr);

        GpsMeasurement_t gps;
        while (true) {
            if (gps_waiting(&gps_data, 60000000)) {
                if (gps_read(&gps_data) == -1) {
                    std::cerr << std::string{"[GPS]:\tError:"} + gps_errstr(errno) << std::endl;
                } else {
                    if ((gps_data.status == STATUS_FIX || gps_data.status == STATUS_DGPS_FIX) &&
                        (gps_data.fix.mode == MODE_2D || gps_data.fix.mode == MODE_3D) &&
                        !std::isnan(gps_data.fix.latitude) &&
                        !std::isnan(gps_data.fix.longitude)) {

                        gps.fixAquired = true;

                        gps.location.lat = gps_data.fix.latitude;
                        gps.location.lon = gps_data.fix.longitude;
                        gps.timestamp = static_cast<si::default_type>(gps_data.fix.time) * si::base::second;
                        gps.speed = static_cast<si::default_type>(gps_data.fix.speed) * si::extended::speed;
                        gps.climb = static_cast<si::default_type>(gps_data.fix.climb) * si::extended::speed;
                        gps.location.altitude = static_cast<si::default_type>(gps_data.fix.altitude) * si::base::meter;
                        gps.epLat = static_cast<si::default_type>(gps_data.fix.epy) * si::base::meter;
                        gps.epLon = static_cast<si::default_type>(gps_data.fix.epx) * si::base::meter;
                        gps.epVert = static_cast<si::default_type>(gps_data.fix.epv) * si::base::meter;
                        gps.epSpeed = static_cast<si::default_type>(gps_data.fix.eps) * si::extended::speed;
                        gps.epClimb = static_cast<si::default_type>(gps_data.fix.epc) * si::extended::speed;

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
