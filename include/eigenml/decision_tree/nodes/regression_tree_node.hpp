#include <eigenml/decision_tree/nodes/base_tree_node.hpp>

namespace eigenml { namespace decision_tree {

    template<class DistributionType, class CriterionType, class FeatureMatrix, class TargetMatrix>
    class RegressionTreeNode : public BaseTreeNode<ModelType::kSupervisedRegressor, DistributionType, CriterionType, FeatureMatrix, TargetMatrix> {

        typedef BaseTreeNode<ModelType::kSupervisedRegressor, DistributionType, CriterionType, FeatureMatrix, TargetMatrix> BaseType;

    public:

        using BaseType::BaseType;

    protected:

    };

}}