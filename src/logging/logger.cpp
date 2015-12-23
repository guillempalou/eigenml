#include <eigenml/logging/logger.hpp>

namespace eigenml { namespace logging {

    Logger::Logger(std::string name) {
        this->name = name;
        this->logger.add_attribute("Name", boost::log::attributes::constant<std::string>(name));
    }

    Logger::~Logger() {

    }

    Logger setNameAttribute(const std::string& name) {
        Logger lg(name);
        return lg;
    }

}}