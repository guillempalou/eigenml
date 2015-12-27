#ifndef INIT_LOGGING_HPP
#define INIT_LOGGING_HPP

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>

namespace eigenml { namespace logging {

    // define a type for severity
    typedef boost::log::trivial::severity_level severity_level;

    // define a type for the logger
    typedef boost::log::sources::severity_logger<severity_level> BoostLogger;
    
    void init_std_log(std::basic_ostream<char>& stream, severity_level severity);

    void init_cerr_log(severity_level severity);

    void init_cout_log(severity_level severity);

    void init_file_log(std::string filename, size_t max_size, severity_level severity); 

}}

#endif // INIT_LOGGING_HPP