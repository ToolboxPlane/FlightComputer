//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_NAVIGATION_HPP
#define FLIGHTCOMPUTER_NAVIGATION_HPP


#include "../ThreadModule.hpp"
#include "../Fusion/Fusion.hpp"

struct nav_t {
    double pitch, heading, power;
};

class Navigation : public ThreadModule{
public:
    Navigation() : ThreadModule() {}

private:
    void run() override;
    Channel<state_t> in;
    Channel<nav_t> out;
};


#endif //FLIGHTCOMPUTER_NAVIGATION_HPP
