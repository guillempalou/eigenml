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

    template<ModelType modelType = ModelType::kSupervisedClassifier, class FeatureMatrix = Matrix, class TargetMatrix = Matrix> 
    class DecisionTree : public core::Model<modelType, FeatureMatrix, TargetMatrix> {

        typedef DecisionTreeNode<modelType, FeatureMatrix, TargetMatrix> NodeType;

        static logging::Logger logger;

    public:

        DecisionTree(const DecisionTreeParams& params = DecisionTreeParams())  : params_(params) {
        }

        ~DecisionTree() {

        }

        bool fit(const FeatureMatrix &X, const TargetMatrix &Y) {
            // check if the tree has been fitted before
            if (root_) {
                LOG_WARN << "Tree not empty, refitting";
                root_.reset();
            }

            std::vector<IdxVector> sorted_indexes;
            size_t n_samples = X.rows();

            if (n_samples == 0)
                throw core::InsufficientSamplesException(n_samples);

            for (int i = 0; i < X.cols(); i++) {
                LOG_TRACE << "Sorting col " << i;
                std::vector<size_t> argsorted = core::argsort(X.col(i));

                // populate the table for lookups
                IdxVector idx(n_samples);
                for (size_t k = 0; k < n_samples; k++) 
                    idx[argsorted[k]] = k;

                sorted_indexes.push_back(idx);
            }

            LOG_INFO << "Creating tree";
            root_ = std::make_shared<NodeType>(params_, 0);

            IdxVector samples(n_samples);
            std::iota( samples.begin(), samples.end(), 0 );

            // recursively split
            return root_->split(X, Y, samples, sorted_indexes, true);
        }

        TargetMatrix transform(const FeatureMatrix &X) {
            TargetMatrix R(X.rows());
            for (int i = 0; i < X.rows(); i++)
                R(i) = root_.predict(X.row(i));
            return R;
        } 

        const NodeType& root() {
            return *root_.get();
        }

    private:

        // parameters of the tree
        DecisionTreeParams params_;
        
        // Root of the tree
        std::shared_ptr<NodeType> root_;
    };

    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    logging::Logger DecisionTree<modelType, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("DecisionTree");

}}

#endif