//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_THREADMODULE_HPP
#define FLIGHTCOMPUTER_THREADMODULE_HPP


#include <thread>

class ThreadModule {
public:
    ThreadModule() {
        std::thread t(&ThreadModule::run, this);
        t.detach();
    }
protected:
    virtual void run()= 0;
private:
};


#endif //FLIGHTCOMPUTER_THREADMODULE_HPP
