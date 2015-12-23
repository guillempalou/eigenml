#include <eigenml/core/model.hpp>

namespace eigenml {

    Model::logger = logging::setNameAttribute("Model");

    template<class FeatureMatrix, class TargetMatrix>
    Model::Model() {
    }


    template<class FeatureMatrix, class TargetMatrix>
    TargetMatrix fit_transform(const FeatureMatrix& X, const TargetMatrix& Y = nullptr) {
        bool fitted = fit(X, Y);
        if (fitted) {
            return transform(X);
        } else {
            return TargetMatrix();
        }
    }

    // specific instantiations
    template class Model<Matrix, Matrix>
    template class Model<Matrix, Vector>
    template class Model<Matrix, VectorI>
}