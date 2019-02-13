#pragma once

#include <string>
#include <net/if.h>
#include <boost/optional/optional.hpp>

using SocketDescriptor = int;
using InterfaceFlags = short int;

class MacAddressError : public std::exception
{
public:
    MacAddressError(std::string_view error);
    const char* what() const noexcept override;

private:
    std::string_view m_error;

};

class MacAddressFetcher
{
public:
    boost::optional<std::string> getMacAddress();

private:
    SocketDescriptor openSocket();
    ifconf getInterfaceConfig(SocketDescriptor socket);
    ifreq findInterface(SocketDescriptor socket, ifconf& config);

    std::string retrieveMacAddress(SocketDescriptor socket, ifreq& interfaceRequest);
    InterfaceFlags retrieveFlags(SocketDescriptor socket, ifreq& interfaceRequest);
    bool isNotLoopback(InterfaceFlags flags);
    bool isConnected(InterfaceFlags flags);


};
