#include <iostream>
#include <sstream>

namespace eigenml { namespace logging {

    template<class Vector>
    std::string vector_str(const Vector& v) {
        std::ostringstream os;
        for (auto& i: v)
            os << i << ' ';
        return os.str();
    }

    template<class Map>
    std::string map_str(const Map& m) {
        std::ostringstream os;
        for (auto& i: m) {
            os << "(" << i.first << ", " << i.second << ") ";
        }
        return os.str();
    }
    
}}