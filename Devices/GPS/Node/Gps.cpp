//
// Created by paul on 15.03.18.
//

#include "Gps.hpp"
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
            if (gps_waiting(&gps_data, std::numeric_limits<int>::max())) {
                if (gps_read(&gps_data) == -1) {
                    std::cerr << std::string{"[GPS]:\tError:"} + gps_errstr(errno) << std::endl;
                } else {
                    if ((gps_data.status == STATUS_FIX || gps_data.status == STATUS_DGPS_FIX) &&
                            gps_data.fix.mode == MODE_3D &&
                            !std::isnan(gps_data.fix.latitude) && !std::isnan(gps_data.fix.longitude)
                            && !std::isnan(gps_data.fix.altitude) && !std::isnan(gps_data.fix.epy)
                            && !std::isnan(gps_data.fix.epx) && !std::isnan(gps_data.fix.epv)) {
                        gps.fixAquired = true;

                        gps.timestamp = si::Second<long double>{gps_data.fix.time};

                        gps.location.lat = gps_data.fix.latitude;
                        gps.location.lon = gps_data.fix.longitude;
                        gps.location.altitude = static_cast<si::default_type>(gps_data.fix.altitude) * si::meter;

                        gps.speed = static_cast<si::default_type>(gps_data.fix.speed) * si::speed;
                        gps.climb = static_cast<si::default_type>(gps_data.fix.climb) * si::speed;

                        gps.epLat = static_cast<si::default_type>(gps_data.fix.epy) * si::meter;
                        gps.epLon = static_cast<si::default_type>(gps_data.fix.epx) * si::meter;
                        gps.epVert = static_cast<si::default_type>(gps_data.fix.epv) * si::meter;
                        gps.epSpeed = static_cast<si::default_type>(gps_data.fix.eps) * si::speed;
                        gps.epClimb = static_cast<si::default_type>(gps_data.fix.epc) * si::speed;

                        out.put(gps);
                    } else {
                        gps.fixAquired = false;

                        gps.timestamp = si::Second<long double>(0);

                        gps.location.lat = NAN;
                        gps.location.lon = NAN;
                        gps.location.altitude = NAN * si::meter;

                        gps.speed = NAN * si::speed;
                        gps.climb = NAN * si::speed;

                        gps.epLat = std::numeric_limits<si::default_type>::infinity() * si::meter;
                        gps.epLon = std::numeric_limits<si::default_type>::infinity() * si::meter;
                        gps.epVert = std::numeric_limits<si::default_type>::infinity() * si::meter;
                        gps.epSpeed = std::numeric_limits<si::default_type>::infinity() * si::speed;
                        gps.epClimb = std::numeric_limits<si::default_type>::infinity() * si::speed;
                        out.put(gps);
                    }
                }
            }
        }
    }

    auto Gps::getChannelOut() -> OutputChannel<GpsMeasurement_t> & {
        return out;
    }
}
