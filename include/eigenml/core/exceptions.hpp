#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace eigenml { namespace core {

    class EigenMLException: public std::exception {

    public:

        EigenMLException(std::string name) : name(name) {
        }

        virtual const char* what() const throw() {
            return name.c_str();
        }
        
    protected:
        
        std::string name;
    };

}}

#endif // EXCEPTIONS_HPP