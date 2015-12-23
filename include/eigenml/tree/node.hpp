#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <functional>

namespace eigenml { namespace tree {

    template<class FeatureMatrix, class TargetMatrix> 
    class Node {

        // type of this class
        typedef Node<FeatureMatrix, TargetMatrix> NodeType;

        // typedef for histogram types
        typedef std::map<double, double> Histogram;

        // value that the split criterion should return
        typedef std::pair<double, double> ValueAndWeight;

        // function for criteria
        typedef std::function<ValueAndWeight(Histogram)> Criterion;

        // sorted X columns by values
        typedef std::vector<size_t> SortedCol;
        typedef std::vector<SortedCol> SortedCols;

    public:

        Node(const int& depth, const FeatureMatrix &X, const TargetMatrix& Y, const SortedCols& indexes) : 
            depth(depth), X(X), Y(Y), indexes(indexes) {

            // initialize the node with the number of samples on it
            n_examples = indexes.size();
        }

        std::vector<NodeType> split(const DecisionTreeParams& params) {

            // if the depth is sufficient
            if (params.max_depth == depth) 
                return std::vector<NodeType>();

            if (params.min_examples >= n_examples) 
                return std::vector<NodeType>();

            // find the column that best splits the node
            Split best_split;

            // use functions depending on the output of the tree

            for (int c = 0; c < X.cols(); ++c) {
                // find the optimal threshold
                auto criterion = std::bind(&NodeType::entropy, *this, std::placeholders::_1);
                Split split = find_optimal_threshold(X.col(c), Y, indexes[c], criterion);
                if (best_split < split) {
                    best_split = split;
                }
            }
            // split the node
            
            return std::vector<Node>();
        }

    protected:

        // structure that the split function returns
        struct Split {

            size_t index = -1;
            double threshold = 0;

            // assign extremely low gain
            double gain = -1e10;
        
            Split(size_t index, double threshold, double gain) : 
                    index(index), threshold(threshold), gain(gain) {
            }

            Split() {
            }

            bool operator<(const Split& other) {
                return gain < other.gain;
            }
        };

        template<class FeatureVector, class TargetVector, typename Function>
        Split find_optimal_threshold(const FeatureVector& features, 
                                     const TargetVector& target, 
                                     const std::vector<size_t>& indexes, 
                                     Function& criterion) {
            
            // order the targets
            TargetVector ordered_target(indexes.size());

            // order the targets and compute a global histogram 
            Histogram histogram;
            for (size_t i = 0; i < indexes.size(); ++i) {
                ordered_target(i) = target(indexes[i]);
                histogram[ordered_target(i)]++;
            }

            // we are only interested in the value in the root
            double root_cost = entropy(histogram).first;

            Histogram left_histogram;
            
            size_t best_index = 0;
            double best_gain = -1;
            double best_threshold = 0;

            for (size_t i = 0; i < indexes.size(); ++i) {
                double threshold = features(indexes[i]);
                left_histogram[ordered_target(i)]++;
                histogram[ordered_target(i)]--;

                auto left_cost = criterion(left_histogram);
                auto right_cost = criterion(histogram);
                
                // get the weight for each child and for the root
                double w = left_cost.second + right_cost.second;
                double w_left = left_cost.second;
                double w_right = right_cost.second;

                //compute the gain
                double gain = w * root_cost - w_left * left_cost.first - w_right * right_cost.first;

                if (best_gain < gain) {
                    best_index = i;
                    best_threshold = threshold;
                    best_gain = gain;
                }
            }

            Split split = {best_index, best_threshold, best_gain};
            return split;
        }
        
        /*
            Entropy criterion for node splitting
         */
        ValueAndWeight entropy(const Histogram& histogram) {
            size_t n = 0;
            // count elements on the histogram
            for (auto& e: histogram) 
                n+=e.second;
            if (n == 0) 
                return ValueAndWeight(0, 0);
            double ent = 0;
            for (auto& e: histogram) {
                double p = e.second*1.0/n;
                ent += (p != 0) ? -p*std::log2(p) : 0;
            }
            return ValueAndWeight(ent, n);
        }

    protected:

        size_t depth;
        size_t n_examples;

        size_t feature_split;
        double threshold;

        const FeatureMatrix& X;
        const TargetMatrix& Y;
        const SortedCols& indexes;
    };

}}

#endif
