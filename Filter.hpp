//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_THREADMODULE_HPP
#define FLIGHTCOMPUTER_THREADMODULE_HPP


#include <thread>

class Filter {
protected:
    virtual void run()= 0;
    void start() {
        std::thread t(&Filter::run, this);
        t.detach();
    }
private:
};


#endif //FLIGHTCOMPUTER_THREADMODULE_HPP
