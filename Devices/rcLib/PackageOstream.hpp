/**
 * @file PackageOstream.hpp
 * @author paul
 * @date 21.12.19
 * @brief PackageOstream @TODO
 */

#ifndef FLIGHTCOMPUTER_PACKAGEOSTREAM_HPP
#define FLIGHTCOMPUTER_PACKAGEOSTREAM_HPP

#include <ostream>
#include "RadioControlProtocolCpp/rcLib.hpp"

std::ostream &operator<<(std::ostream &ostream, const rcLib::Package &package);

#endif //FLIGHTCOMPUTER_PACKAGEOSTREAM_HPP
