#include <vector>
#include <numeric>
#include <algorithm>

#include <eigenml/core/types.hpp>

namespace eigenml { namespace core {

    template <typename T>
    std::vector<size_t> argsort(const T& v);

}}