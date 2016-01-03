#include <eigenml/core/types.hpp> 

namespace eigenml {
    
    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    struct model_traits {

        // scalar types
        typedef typename FeatureMatrix::Scalar FeatureScalarType;
        typedef typename TargetMatrix::Scalar TargetScalarType;
        
        // sample types
        typedef typename FeatureMatrix::ConstRowXpr  ConstSampleType;
        typedef typename FeatureMatrix::RowXpr  SampleType;
    }; 

}