#include "Logging.hpp"

std::shared_ptr<XiboLogger> Log::logger()
{
    static auto logger = XiboLogger::get();
    return logger;
}
