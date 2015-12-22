#ifndef __RF_DECISION_TREE_CLASSIFIER_HPP__
#define __RF_DECISION_TREE_CLASSIFIER_HPP__

#include <random_forest/model.hpp>
#include <random_forest/tree/decision_tree_params.hpp>

namespace random_forest {

    template<class Matrix>
    class DecisionTree : public Model<Matrix> {
    public:

        DecisionTree(const DecisionTreeParams& params = DecisionTreeParams()) {
            this->params = params;
            // sanity check the parameters
        }

        ~DecisionTree() {
        }

        bool fit(const Matrix &X, const Matrix &Y) {
            // first some assertions
            
            // add the initial node
            int n_examples = X.rows()

            Node root(0, X, Y);
            nodes.push_back(root);

            int head = 1;
            int tail = 0;

            while (tail < head) {
                Node& n = nodes[tail];

                auto splitted = n.split(params);
                for (auto child = splitted.begin(); child != splitted.end(); ++child) {
                    nodes.push_back(*child);
                }
            }

            return true;
        }

        Matrix transform(const Matrix &X) {
            return Matrix();
        }

    private:

        // parameters of the tree
        DecisionTreeParams params;
        
        // nodes of the tree
        std::vector<Node> nodes;

    };
}

#endif