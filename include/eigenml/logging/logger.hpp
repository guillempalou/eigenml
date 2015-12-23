#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>

#include <eigenml/logging/init_logging.hpp>

#define LOG_TRACE(lg) BOOST_LOG_SEV(lg.logger, boost::log::trivial::trace)
#define LOG_DEBUG(lg) BOOST_LOG_SEV(lg.logger, boost::log::trivial::debug)
#define LOG_INFO(lg) BOOST_LOG_SEV(lg.logger, boost::log::trivial::info)
#define LOG_WARN(lg) BOOST_LOG_SEV(lg.logger, boost::log::trivial::warn)
#define LOG_ERROR(lg) BOOST_LOG_SEV(lg.logger, boost::log::trivial::error)

namespace eigenml { namespace logging {

    struct Logger {

        typedef std::ostream LogStream;

        Logger(std::string name);
        ~Logger();

        std::string name;
        BoostLogger logger;
 
    };

    Logger setNameAttribute(const std::string& name);

}}

#endif // LOGGER_HPP
