#include "Logging.hpp"

std::shared_ptr<Logger> Log::logger()
{
    static auto logger = Logger::get();
    return logger;
}
