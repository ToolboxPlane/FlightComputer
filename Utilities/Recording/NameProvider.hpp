/**
 * @file NameProvider.hpp
 * @author paul
 * @date 03.04.20
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_NAMEPROVIDER_HPP
#define FLIGHTCOMPUTER_NAMEPROVIDER_HPP

#include <fstream>
#include <string>

namespace recording {
    class NameProvider {
      public:
        NameProvider();
        explicit NameProvider(const std::string &recordingName);

        [[nodiscard]] auto getOutputStream(const std::string &device) const -> std::ofstream;

        [[nodiscard]] auto getInputStream(const std::string &device) const -> std::ifstream;

      private:
        std::string recordingPath;
    };
} // namespace recording

#endif // FLIGHTCOMPUTER_NAMEPROVIDER_HPP
