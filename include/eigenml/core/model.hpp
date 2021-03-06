#ifndef MODEL_HPP
#define MODEL_HPP

#include <eigenml/core/eigenml.hpp>

namespace eigenml { 

    template<ModelType type, class FeatureMatrix = Matrix, class TargetMatrix = Matrix> 
    class Model {
        
        static logging::Logger logger; 

    public:

        Model() {

        }

        // method to train the model
        virtual bool fit(const FeatureMatrix &X, const TargetMatrix &Y) = 0;
        
        // method to transform the input to the output
        virtual TargetMatrix transform(const FeatureMatrix &X) = 0;

        // TODO improve the error handling?
        TargetMatrix fit_transform(const FeatureMatrix& X, const TargetMatrix& Y = nullptr) {
            bool fitted = fit(X, Y);
            if (fitted) {
                return transform(X);
            } else {
                return TargetMatrix();
            }
        }
    
    };

    template<ModelType type, class FeatureMatrix, class TargetMatrix>
    logging::Logger Model<type, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("Model");


}

#endif // MODEL_HPP