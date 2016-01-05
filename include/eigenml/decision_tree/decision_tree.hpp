#ifndef DECISION_TREE_HPP
#define DECISION_TREE_HPP

#include <vector>
#include <memory>

#include <eigenml/core/model.hpp>
#include <eigenml/core/vectors/sorting.hpp>
#include <eigenml/logging/logging.hpp>

#include <eigenml/decision_tree/decision_tree_params.hpp>
#include <eigenml/decision_tree/decision_tree_traits.hpp>

namespace eigenml { namespace decision_tree {

    template<ModelType modelType = ModelType::kSupervisedClassifier, class FeatureMatrix = Matrix, class TargetMatrix = Matrix> 
    class DecisionTree : public Model<modelType, FeatureMatrix, TargetMatrix> {

        // traits type
        typedef tree_traits<modelType, FeatureMatrix, TargetMatrix> tree_traits_type;

        // node of the trees
        typedef typename tree_traits_type::NodeType NodeType;

        // criterion function
        typedef typename tree_traits_type::CriterionType CriterionType;

        // distribution type
        typedef typename tree_traits_type::DistributionType DistributionType;

        // criterion creator
        typedef CriterionCreator<modelType, DistributionType, CriterionType, FeatureMatrix, TargetMatrix> CriterionCreatorType;

        // splitter for the nodes
        typedef ThresholdFinder<DistributionType, CriterionType, FeatureMatrix, TargetMatrix> ThresholdFinderType;

        // logger
        static logging::Logger logger;

    public:

        DecisionTree(const DecisionTreeParams& params = DecisionTreeParams())  : params_(params) {
            LOG_DEBUG << "Criterion for creating the tree " << params.criterion;
            criterion_ = CriterionCreatorType::create_criterion_function(params.criterion);
            threshold_finder_ = ThresholdFinderType();
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
            root_ = std::make_shared<NodeType>(params_, 0, threshold_finder_, criterion_);

            IdxVector samples(n_samples);
            std::iota( samples.begin(), samples.end(), 0 );

            // recursively split
            return root_->split(X, Y, samples, sorted_indexes, true);
        }

        TargetMatrix transform(const FeatureMatrix &X) {
            TargetMatrix R(X.rows(), 1);
            for (int i = 0; i < X.rows(); i++)
                R(i) = root_.get()->predict(X.row(i));
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

        // threshold finder
        ThresholdFinderType threshold_finder_;

        // Criterion 
        CriterionType criterion_;
    };

    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    logging::Logger DecisionTree<modelType, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("DecisionTree");

}}

#endif