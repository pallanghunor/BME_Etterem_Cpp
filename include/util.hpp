#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

// JPorta védelem: Fájlok beolvasásakor a Windows-os \r karaktert levágja
inline void trimCR(std::string& str) {
    if (!str.empty() && str.back() == '\r') {
        str.pop_back();
    }
}

#endif // UTIL_HPP