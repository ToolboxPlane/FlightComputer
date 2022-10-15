/**
 * @file PackageOstream.hpp
 * @author paul
 * @date 21.12.19
 * @brief PackageOstream @TODO
 */

#ifndef FLIGHTCOMPUTER_PACKAGEUTIL_HPP
#define FLIGHTCOMPUTER_PACKAGEUTIL_HPP

#include <ostream>
#include <sstream>

#include "../../Utilities/Recording/Serialization.hpp"
#include "RadioControlProtocolCpp/rcLib.hpp"

std::ostream &operator<<(std::ostream &ostream, const rcLib::Package &package);


namespace recording {
    template<>
    auto header<rcLib::Package>() -> std::string;

    template<>
    auto getLine<rcLib::Package>(const rcLib::Package &package) -> std::string;

    template<>
    auto getItem<rcLib::Package>(const std::vector<std::string> &line) -> rcLib::Package;

} // namespace recording
#endif // FLIGHTCOMPUTER_PACKAGEUTIL_HPP
