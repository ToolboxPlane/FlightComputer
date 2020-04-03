/**
 * @file NameProvider.hpp
 * @author paul
 * @date 03.04.20
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_NAMEPROVIDER_HPP
#define FLIGHTCOMPUTER_NAMEPROVIDER_HPP

#include <iostream>
#include <map>

namespace recording {
    class NameProvider {
        public:
            NameProvider();
            explicit NameProvider(std::string recordingName);

            auto getStream(const std::string &device) -> std::iostream&;

        private:
            std::string recordingName;
            std::map<std::string, std::fstream> openStreams;

    };
}

#endif //FLIGHTCOMPUTER_NAMEPROVIDER_HPP
