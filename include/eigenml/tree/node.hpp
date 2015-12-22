#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <vector>
#include <

namespace random_forest {
    template<class Matrix>
    class Node {

        Node(const int& depth, const Matrix &X, const Matrix& Y): depth(depth) X(X), Y(Y) {

            // compute the loss of this node
            criterion = [](Matrix y){return y.std();};
        }

        std::vector<Node> split(const DecisionTreeParams& params) {

             nodes;
            int nodes = X.rows();

            // if the depth is sufficient
            if (params.max_depth == this.depth) 
                return std::vector<Node>();

            if (params.min_examples >= examples) 
                return std::vector<Node>();

            // find the column that best splits the node
            double max_gain;
            std::vector<Node> best_split;

            for (int c = 0; c < X.cols(); ++c) {
                // find the optimal threshold
                std::vector<Node> childs = best_threshold(X.col(i), Y, criterion);

                if (max_gain < loss - childs[0].loss - childs[1].loss) {
                    best_split = current_split;
                    max_gain = loss - childs[0].loss - childs[1].loss;
                }
            }

            return best_split;
        }

    protected:

        template<T, F>
        std::vector<Node> best_threshold(T values, U targets, F criterion) {
            
        }

    protected:
        int depth;

        int split_factor;
        double threshold;

        double loss;

        Matrix& X;
        Matrix& Y;
    };
}

#endif
