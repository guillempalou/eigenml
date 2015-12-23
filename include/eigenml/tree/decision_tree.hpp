#ifndef DECISION_TREE_HPP
#define DECISION_TREE_HPP

#include <vector>

#include <eigenml/core/model.hpp>
#include <eigenml/core/vectors/sorting.hpp>

#include <eigenml/tree/decision_tree_params.hpp>
#include <eigenml/tree/node.hpp>

#include <eigenml/logging/printing.hpp>

namespace eigenml { namespace tree {

    template<class FeatureMatrix, class TargetMatrix> 
    class DecisionTree : public core::Model<FeatureMatrix, TargetMatrix> {

        typedef Node<FeatureMatrix, TargetMatrix> NodeType;

    public:

        DecisionTree(const DecisionTreeParams& params = DecisionTreeParams()) {
            this->params = params;
            // sanity check the parameters
        }

        ~DecisionTree() {
        }

        bool fit(const FeatureMatrix &X, const TargetMatrix &Y) {
            // first some assertions
            
            if (!nodes.empty()) {
                nodes.clear();
            }

            std::vector<std::vector<size_t> > sorted_cols;

            for (int i = 0; i < X.cols(); i++) {
                std::vector<size_t> sorted_idx = core::argsort(X.col(i));
                sorted_cols.push_back(sorted_idx);
            }

            NodeType root(0, X, Y, sorted_cols);
            nodes.push_back(root);

            int head = 1;
            int tail = 0;

            while (tail < head) {
                NodeType& n = nodes[tail];

                auto splitted = n.split(params);
                for (auto child = splitted.begin(); child != splitted.end(); ++child) {
                    nodes.push_back(*child);
                }
            }

            return true;
        }

        TargetMatrix transform(const FeatureMatrix &X) {
            X.sum();
            return TargetMatrix();
        }

    private:

        // parameters of the tree
        DecisionTreeParams params;
        
        // nodes of the tree
        std::vector<NodeType> nodes;

    };

}}

#endif