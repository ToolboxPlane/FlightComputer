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
#include "RadioControlProtocolCpp/rcLib.hpp"
#include "../../Utilities/Recording/Serialization.hpp"

std::ostream &operator<<(std::ostream &ostream, const rcLib::Package &package);


namespace recording {
    template <>
    auto header<rcLib::Package>() -> std::string;

    template <>
    auto getLine<rcLib::Package>(const rcLib::Package &package) -> std::string;

}
#endif //FLIGHTCOMPUTER_PACKAGEUTIL_HPP
