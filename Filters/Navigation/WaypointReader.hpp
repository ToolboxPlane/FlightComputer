//
// Created by paul on 14.05.18.
//

#ifndef FLIGHTCOMPUTER_WAYPOINTREADER_HPP
#define FLIGHTCOMPUTER_WAYPOINTREADER_HPP


#include "../../ThreadModule.hpp"
#include "../../Utilities/json.hpp"
#include "Waypoint_t.hpp"
#include "../../Channel.hpp"
#include "../../MultipleOutputChannel.hpp"

#include <iostream>

using json = nlohmann::json;

class WaypointReader : public ThreadModule{
public:
    explicit WaypointReader(std::istream &istream) : ThreadModule(), istream(istream) {}
    MultipleOutputChannel<Waypoint_t> &getChannelOut();
private:
    void run() override;
    MultipleOutputChannel<Waypoint_t> out;
    std::istream &istream;
};


#endif //FLIGHTCOMPUTER_WAYPOINTREADER_HPP
