/**
 * @file NameProvider.cpp
 * @author paul
 * @date 03.04.20
 * Description here TODO
 */
#include "NameProvider.hpp"

#include <ctime>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <filesystem>

namespace recording {
    NameProvider::NameProvider() {
        auto time = std::time(nullptr);
        auto localTime = std::localtime(&time);

        std::stringstream dirNameStream;
        dirNameStream << "Logs/Recording" << std::put_time(localTime,"%Y-%m-%d_%H-%M-%S");

        recordingName = dirNameStream.str();

        if (!std::filesystem::exists(recordingName)) {
            std::filesystem::create_directory(recordingName);
        }
    }

    NameProvider::NameProvider(std::string recordingName) : recordingName{std::move(recordingName)} {
        if (!std::filesystem::exists(recordingName)) {
            throw std::runtime_error{"[NameProvider]:\tRecording does not exist!"};
        }
    }

    auto NameProvider::getStream(const std::string &device) -> std::iostream& {
        if (openStreams.find(device) == openStreams.end()) {
            std::fstream fstream{recordingName + "/" + device + ".csv"};
            openStreams.emplace(std::make_pair(device, std::move(fstream)));
        }
        return openStreams.at(device);

    }
}
