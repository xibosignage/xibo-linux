#include "SOAPManager.hpp"

SOAPManager::SOAPManager(const std::string& host, int port) :
    m_work{m_ioc}, m_host(host), m_port(port)
{
    m_spdlog = spdlog::get(LOGGER);
    m_work_thread.reset(new std::thread([=](){
        m_ioc.run();
    }));
}

SOAPManager::~SOAPManager()
{
    m_ioc.stop();
    m_work_thread->join();
}
