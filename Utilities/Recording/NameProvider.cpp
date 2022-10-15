/**
 * @file NameProvider.cpp
 * @author paul
 * @date 03.04.20
 * Description here TODO
 */
#include "NameProvider.hpp"

#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace recording {
    NameProvider::NameProvider() {
        auto time = std::time(nullptr);
        auto localTime = std::localtime(&time);

        std::stringstream dirNameStream;
        dirNameStream << "Logs/Recording-" << std::put_time(localTime, "%Y-%m-%d_%H-%M-%S");

        recordingPath = dirNameStream.str();

        if (!std::filesystem::exists("Logs")) {
            std::filesystem::create_directory("Logs");
        }

        if (!std::filesystem::exists(recordingPath)) {
            std::filesystem::create_directory(recordingPath);
        }
    }

    NameProvider::NameProvider(const std::string &recordingName) {
        std::stringstream dirNameStream;
        dirNameStream << "Logs/Recording-" << recordingName;
        recordingPath = dirNameStream.str();

        if (!std::filesystem::exists(recordingPath)) {
            throw std::runtime_error{"[NameProvider]:\tRecording does not exist!"};
        }
    }

    auto NameProvider::getOutputStream(const std::string &device) const -> std::ofstream {
        return std::ofstream{recordingPath + "/" + device + ".csv"};
    }

    auto NameProvider::getInputStream(const std::string &device) const -> std::ifstream {
        if (!std::filesystem::exists(recordingPath + "/" + device + ".csv")) {
            throw std::runtime_error("[NameProvider]:\t" + device + " does not exist!");
        }
        return std::ifstream{recordingPath + "/" + device + ".csv"};
    }
} // namespace recording
