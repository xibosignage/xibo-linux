#include "SOAPManager.hpp"

const int DEFAULT_CONCURRENT_REQUESTS = 2;

SOAPManager::SOAPManager(const std::string& host) :
    m_work{m_ioc}, m_host(host)
{
    for(int i = 0; i != DEFAULT_CONCURRENT_REQUESTS; ++i)
    {
        m_workerThreads.push_back(std::make_unique<JoinableThread>([=](){
            Log::trace("SOAP Thread {}", std::this_thread::get_id());

            m_ioc.run();
        }));
    }
}

SOAPManager::~SOAPManager()
{
    m_ioc.stop();
}
