#include <iostream>
#include <vector>
#include <map>
#include <numeric>
#include <Eigen/Dense>
//#include <random_forest/tree/decision_tree.hpp>
#include <algorithm>
#include <cmath>

using namespace std;

template <typename T>
std::vector<size_t> argsort(const T& v) {

  // initialize original index locations
  std::vector<size_t> idx(v.size());

  // fills idx[i] = i
  std::iota( idx.begin(), idx.end(), 0 );

  // sort indexes based on comparing values in v
  std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) -> bool {return v[i1] < v[i2];});

  return idx;
}

template<class T>
std::pair<double, double> entropy(const T& histogram) {
    size_t n = 0;
    // count elements on the histogram
    for (auto& e: histogram) 
        n+=e.second;
    if (n == 0) 
        return std::pair<double, double>(0, 0);
    double ent = 0;
    for (auto& e: histogram) {
        double p = e.second*1.0/n;
        ent += (p != 0) ? -p*std::log2(p) : 0;
    }
    return std::pair<double, double>(ent, n);
}

template<class FeatureVector, class TargetVector, typename Function>
std::pair<size_t, double> find_optimal_threshold(const FeatureVector& features, 
                                              const std::vector<size_t>& indexes, 
                                              const TargetVector& target, 
                                              Function criterion) {
    
    // order the targets
    TargetVector ordered_target(indexes.size());

    // order the targets and compute a global histogram 
    std::map<int, int> histogram;
    for (size_t i = 0; i < indexes.size(); ++i) {
        ordered_target(i) = target(indexes[i]);
        histogram[ordered_target(i)]++;
    }

    // we are only interested in the value
    double root_cost = entropy(histogram).first;

    std::map<int, int> left_histogram;
    std::map<int, int> right_histogram(histogram);
    
    size_t best_index = 0;
    double best_gain = -1;
    double best_threshold = 0;

    for (size_t i = 0; i < indexes.size(); ++i) {
        double threshold = features(indexes[i]);
        left_histogram[ordered_target(i)]++;
        right_histogram[ordered_target(i)]--;

        auto left_cost = criterion(left_histogram);
        auto right_cost = criterion(right_histogram);
        
        // get the weight for each child
        double w_left = left_cost.second / (left_cost.second + right_cost.second);
        double w_right = right_cost.second / (left_cost.second + right_cost.second);

        //compute the gain
        double gain = root_cost - w_left * left_cost.first - w_right * right_cost.first;

        if (best_gain < gain) {
            best_index = i;
            best_threshold = threshold;
            best_gain = gain;
        }
    }

    return std::pair<size_t, double>(best_index, best_threshold);
}

int main() {
    
    // DecisionTreeParams params;
    // params.criterion = "mse";
    
    // DecisionTree<Eigen::MatrixXd> clf(params);
    
    size_t N = 10;
    Eigen::MatrixXd X = Eigen::MatrixXd::Random(N, 1);
    Eigen::VectorXi Y(N, 1);

    X << 0, 2, 4, 5, 8, 1, 9, 7, 3, 6;
    Y << 0, 0, 0, 1, 1, 0, 1, 1, 0, 1;

    // init tree
    const auto& c = X.col(0);
    std::vector<size_t> idx = argsort(c);

    typedef map<int, int> Histogram;
    std::pair<size_t, double> result = find_optimal_threshold(c, idx, Y, &entropy<Histogram>);

    std::cout << "Index " << result.first << std::endl;
    std::cout << "Threshold " << result.second << std::endl;

    // std::cout << "Classifier " << clf.fit(X, Y) << std::endl;
}
