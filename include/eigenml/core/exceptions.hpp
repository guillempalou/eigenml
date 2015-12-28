#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace eigenml { namespace core {

    // enums defining exception arguments
    struct ExceptionMessage {
        static const std::string kGeneralException;
        static const std::string kWrongParametersException;
        static const std::string kInsufficientSamplesException;
        static const std::string kWrongSplitCriterionException;
    };

    class EigenMLException: public std::exception {

    public:

        EigenMLException(std::string name) : name_(ExceptionMessage::kGeneralException + ": " + name) {
        }

        virtual const char* what() const throw() {
            return name_.c_str();
        }
        
    protected:
        
        std::string name_;
    };

    class WrongParametersException : public EigenMLException {

    public:

        WrongParametersException(std::string msg) : 
            EigenMLException(msg + " " + ExceptionMessage::kWrongSplitCriterionException) {
        }
    };


    class InsufficientSamplesException : public EigenMLException {

    public:

        InsufficientSamplesException(size_t samples) : 
            EigenMLException(ExceptionMessage::kInsufficientSamplesException + " " + std::to_string(samples)) {
        }

    };

    
}}

#endif // EXCEPTIONS_HPP