#include <eigenml/core/exceptions.hpp>

namespace eigenml { namespace core { 
    
    const std::string ExceptionMessage::kGeneralException = "EigenML Exception";
    const std::string ExceptionMessage::kWrongParametersException = "Wrong parameters";
    const std::string ExceptionMessage::kInsufficientSamplesException = "Insufficient Samples";

    // decision tree specific
    const std::string ExceptionMessage::kWrongSplitCriterionException = "Wrong split Criterion";

}}