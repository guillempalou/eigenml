#include <iostream>

namespace eigenml { namespace logging {

    template<class Vector>
    void print_vector(const Vector& v) {
        for (auto& i: v)
            std::cout << i << ' ';
        std::cout << std::endl;
    }

    template<class Map>
    void print_map(const Map& m) {
        for (auto& i: m) {
            std::cout << "(" << i.first << ", " << i.second << ") ";
        }
        std::cout << std::endl;
    }
    
}}