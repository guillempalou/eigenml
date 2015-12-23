#include <eigenml/core/model.hpp>

namespace eigenml { namespace core {

    template<class FeatureMatrix, class TargetMatrix>
    logging::Logger Model<FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("Model");

    template<class FeatureMatrix, class TargetMatrix>
    Model<FeatureMatrix, TargetMatrix>::Model() {
    }

    template<class FeatureMatrix, class TargetMatrix>
    TargetMatrix Model<FeatureMatrix, TargetMatrix>::fit_transform(const FeatureMatrix& X, const TargetMatrix& Y) {
        bool fitted = fit(X, Y);
        if (fitted) {
            return transform(X);
        } else {
            return TargetMatrix();
        }
    }

    // specific instantiations
    template class Model<Matrix, Matrix>;
    template class Model<Matrix, Vector>;
    template class Model<Matrix, VectorI>;

}}