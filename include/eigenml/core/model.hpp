#ifndef MODEL_HPP
#define MODEL_HPP

#include <eigenml/core/types.hpp>
#include <eigenml/logging/logging.hpp>

namespace eigenml { namespace core {

    template<class FeatureMatrix, class TargetMatrix> 
    class Model {
        
        static logging::Logger logger; 

    public:

        Model();

        // method to train the model
        virtual bool fit(const FeatureMatrix &X, const TargetMatrix &Y) = 0;
        
        // method to transform the input to the output
        virtual TargetMatrix transform(const FeatureMatrix &X) = 0;
        
        // TODO improve the error handling?
        TargetMatrix fit_transform(const FeatureMatrix& X, const TargetMatrix& Y = nullptr);
    };
    
}}

#endif // MODEL_HPP