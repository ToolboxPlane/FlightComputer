//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPS_T_HPP
#define FLIGHTCOMPUTER_GPS_T_HPP

class Gps_t {
public:
    double lat, lon;
    unsigned long timestamp;

    friend std::ostream &operator<<(std::ostream &ostream, Gps_t gps) {
        ostream << "Lat: " << gps.lat << "\tLon: " << gps.lon << "\tAt: " << gps.timestamp;
        return ostream;
    }
};

#endif //FLIGHTCOMPUTER_GPS_T_HPP
