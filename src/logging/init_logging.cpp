#include <eigenml/logging/init_logging.hpp>

namespace boostlog = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

namespace eigenml { namespace logging {
    
    void init_cout_log(boost::log::trivial::severity_level severity) {
        init_std_log(std::cout, severity);
    }

    void init_cerr_log(boost::log::trivial::severity_level severity) {
        init_std_log(std::cerr, severity);
    }

    void init_std_log(std::basic_ostream<char>& stream, boost::log::trivial::severity_level severity) {
        boostlog::add_console_log
        (
            stream,
            keywords::format = (expr::stream
                << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << ": [" << boostlog::trivial::severity << "] "
                << expr::attr<std::string>("Name") << " | "
                << expr::smessage)
        );

        boostlog::core::get()->set_filter
        (
            boostlog::trivial::severity >= severity
        );

        boostlog::add_common_attributes();
    }

    // got it from boost.org
    void init_file_log(std::string filename, size_t max_size, boost::log::trivial::severity_level severity)
    {
        boostlog::add_file_log
        (
            keywords::file_name = filename,
            keywords::rotation_size = max_size,
            keywords::format = (expr::stream
                << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << ": [" << boostlog::trivial::severity << "] "
                << expr::attr<std::string>("Name") << " | "
                << expr::smessage)
        );

        boostlog::core::get()->set_filter
        (
            boostlog::trivial::severity >= severity
        );

        boostlog::add_common_attributes();
    }

}}