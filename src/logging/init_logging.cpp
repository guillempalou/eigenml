#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace boostlog = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

namespace eigenml { namespace logging {
    
    // got it from boost.org
    void init_file_log(std::string filename, size_t max_size)
    {
        boostlog::add_file_log
        (
            keywords::file_name = filename,
            keywords::rotation_size = max_size,
            keywords::format = "[%TimeStamp%]: %Message%"
        );

        boostlog::core::get()->set_filter
        (
            boostlog::trivial::severity >= boostlog::trivial::info
        );
    }

}}