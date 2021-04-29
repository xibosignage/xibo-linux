#include "MacAddressFetcher.hpp"

#include "common/logger/Logging.hpp"

#include <boost/format.hpp>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

const std::size_t ConfigBufferSize = 1024;
const std::size_t MacAddressBuffer = 100;
const MacAddress UndefinedMacAddress{"00:00:00:00:00:00"};
const int InvalidSocket = -1;

MacAddress MacAddressFetcher::fetch()
{
    try
    {
        char buffer[ConfigBufferSize];

        auto socket = openSocket();
        auto interfaceConfig = getInterfaceConfig(socket, buffer);
        auto interfaceRequest = findInterface(socket, interfaceConfig);

        return MacAddress{retrieveMacAddress(socket, interfaceRequest)};
    }
    catch (std::exception& e)
    {
        Log::info("Failed to get MAC address: {}", e.what());
        return UndefinedMacAddress;
    }
}

SocketDescriptor MacAddressFetcher::openSocket()
{
    SocketDescriptor sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == InvalidSocket)
    {
        throw MacAddressFetcher::Error{"MAC", "Socket was not opened"};
    }
    return sock;
}

ifconf MacAddressFetcher::getInterfaceConfig(SocketDescriptor socket, char* buffer)
{
    ifconf interfaceConfig;
    interfaceConfig.ifc_len = ConfigBufferSize;
    interfaceConfig.ifc_buf = buffer;

    if (!ioctl(socket, SIOCGIFCONF, &interfaceConfig))
    {
        return interfaceConfig;
    }

    throw MacAddressFetcher::Error{"MAC", "Interface Config was not recieved"};
}

ifreq MacAddressFetcher::findInterface(SocketDescriptor socket, ifconf& interfaceConfig)
{
    const std::size_t configSize = static_cast<std::size_t>(interfaceConfig.ifc_len) / sizeof(ifreq);

    ifreq* it = interfaceConfig.ifc_req;
    const ifreq* const end = it + configSize;

    for (; it != end; ++it)
    {
        auto flags = retrieveFlags(socket, *it);
        if (isNotLoopback(flags))
        {
            ifreq interfaceRequest;
            std::strcpy(interfaceRequest.ifr_name, it->ifr_name);
            return interfaceRequest;
        }
    }

    throw MacAddressFetcher::Error{"MAC", "Interface Request was not found"};
}

std::string MacAddressFetcher::retrieveMacAddress(SocketDescriptor socket, ifreq& interfaceRequest)
{
    if (!ioctl(socket, SIOCGIFHWADDR, &interfaceRequest))
    {
        char macAddress[MacAddressBuffer] = {0};
        unsigned char* mac = reinterpret_cast<unsigned char*>(interfaceRequest.ifr_hwaddr.sa_data);

        std::sprintf(macAddress, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        return macAddress;
    }

    throw MacAddressFetcher::Error{"MAC", "MAC Address was not retrived from request"};
}

InterfaceFlags MacAddressFetcher::retrieveFlags(SocketDescriptor socket, ifreq& interfaceRequest)
{
    if (!ioctl(socket, SIOCGIFFLAGS, &interfaceRequest))
    {
        return interfaceRequest.ifr_flags;
    }

    throw MacAddressFetcher::Error{"MAC", "Flags were not retrived from frequest"};
}

bool MacAddressFetcher::isNotLoopback(InterfaceFlags flags)
{
    return !(flags & IFF_LOOPBACK);
}
