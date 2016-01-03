#include <eigenml/decision_tree/nodes/base_tree_node.hpp>

namespace eigenml { namespace decision_tree {

    template<class FeatureMatrix, class TargetMatrix>
    class RegressionTreeNode : public BaseTreeNode<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix> {
    public:
    protected:
    };

}}