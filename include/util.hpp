#ifndef UTIL_HPP
#define UTIL_HPP

/**
 * @file util.hpp
 * @brief Kis segédfüggvények.
 */

#include <string>

/**
 * @brief Leszedi a sorvégi carriage return karaktert, ha van.
 */
inline void trimCR(std::string& str) {
    if (!str.empty() && str.back() == '\r') {
        str.pop_back();
    }
}

#endif // UTIL_HPP