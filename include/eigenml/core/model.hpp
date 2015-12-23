#ifndef MODEL_HPP
#define MODEL_HPP

namespace eigenml { namespace core {

    template<class FeatureMatrix, class TargetMatrix> 
    class Model {
    public:
        // method to train the model
        virtual bool fit(const FeatureMatrix &X, const TargetMatrix &Y) = 0;
        
        // method to transform the input to the output
        virtual TargetMatrix transform(const FeatureMatrix &X) = 0;
        
        // TODO improve the error handling?
        virtual TargetMatrix fit_transform(const FeatureMatrix& X, const TargetMatrix& Y = nullptr) {
            bool fitted = fit(X, Y);
            if (fitted) {
                return transform(X);
            } else {
                return TargetMatrix();
            }
        }
    };
    
}}

#endif // MODEL_HPP