#include <vector>
#include <numeric>
#include <algorithm>

namespace eigenml { namespace core {

    template <typename T>
    std::vector<size_t> argsort(const T& v) {

      // initialize original index locations
      std::vector<size_t> idx(v.size());

      // fills idx[i] = i
      std::iota( idx.begin(), idx.end(), 0 );

      // sort indexes based on comparing values in v
      std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) -> bool {return v(i1) < v(i2);});

      return idx;
    }

}}