#include "SOAPManager.hpp"

const int DEFAULT_PORT = 80;

SOAPManager::SOAPManager(const std::string& host) :
    m_work{m_ioc}, m_host(host), m_port(DEFAULT_PORT)
{
    m_logger = spdlog::get(LOGGER);
    m_work_thread.reset(new std::thread([=](){
        m_ioc.run();
    }));
}

SOAPManager::~SOAPManager()
{
    m_ioc.stop();
    m_work_thread->join();
}
