#ifndef DECISION_TREE_HPP
#define DECISION_TREE_HPP

#include <vector>
#include <memory>

#include <eigenml/core/model.hpp>
#include <eigenml/core/vectors/sorting.hpp>

#include <eigenml/decision_tree/decision_tree_params.hpp>
#include <eigenml/decision_tree/decision_tree_node.hpp>

#include <eigenml/logging/logging.hpp>

namespace eigenml { namespace decision_tree {

    template<class FeatureMatrix = Matrix, class TargetMatrix = Matrix> 
    class DecisionTree : public core::Model<FeatureMatrix, TargetMatrix> {

        typedef DecisionTreeNode<FeatureMatrix, TargetMatrix> NodeType;

        static logging::Logger logger;

    public:

        DecisionTree(const DecisionTreeParams& params = DecisionTreeParams())  : params(params) {
        }

        ~DecisionTree() {

        }

        bool fit(const FeatureMatrix &X, const TargetMatrix &Y) {
            // check if the tree has been fitted before
            if (root) {
                LOG_WARN << "Tree not empty, refitting";
                root.reset();
            }

            std::vector<IdxVector> sorted_indexes;
            size_t n_examples = X.rows();

            for (int i = 0; i < X.cols(); i++) {
                LOG_TRACE << "Sorting col " << i;
                std::vector<size_t> argsorted = core::argsort(X.col(i));

                // populate the table for lookups
                IdxVector idx(n_examples);
                for (size_t k = 0; k < n_examples; k++) 
                    idx[argsorted[k]] = k;

                sorted_indexes.push_back(idx);
            }

            LOG_INFO << "Creating tree";
            root = std::make_shared<NodeType>(params, 0);

            IdxVector examples(n_examples);
            std::iota( examples.begin(), examples.end(), 0 );

            // recursively split
            return root->split(X, Y, examples, sorted_indexes, true);
        }

        TargetMatrix transform(const FeatureMatrix &X) {
            X.sum();
            return TargetMatrix();
        }

    private:

        // parameters of the tree
        DecisionTreeParams params;
        
        // Root of the tree
        std::shared_ptr<NodeType> root;
    };

    template<class FeatureMatrix, class TargetMatrix>
    logging::Logger DecisionTree<FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("DecisionTree");

}}

#endif