//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_THREADMODULE_HPP
#define FLIGHTCOMPUTER_THREADMODULE_HPP


#include <thread>
#include <future>

class ThreadModule {
public:
    ThreadModule() : handle(std::async(std::launch::async, &ThreadModule::run, this)) {}
    virtual void run()= 0;
private:
    std::future<void> handle;
};


#endif //FLIGHTCOMPUTER_THREADMODULE_HPP
