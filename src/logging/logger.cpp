#include <eigenml/logging/logger.hpp>

namespace eigenml { namespace logging {

    Logger::Logger(std::string name) {
        this->name = name;
        this->logger.add_attribute("Name", boost::log::attributes::constant<std::string>(name));
    }

    Logger::~Logger() {

    }

    BoostLogger setNameAttribute(const std::string& name) {
        BoostLogger lg;
        lg.add_attribute("Name", boost::log::attributes::constant<std::string>(name));
        return lg;
    }

}}