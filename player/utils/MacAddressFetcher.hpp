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
    static boost::optional<std::string> getMacAddress();

private:
    static SocketDescriptor openSocket();
    static ifconf getInterfaceConfig(SocketDescriptor socket);
    static ifreq findInterface(SocketDescriptor socket, ifconf& config);

    static std::string retrieveMacAddress(SocketDescriptor socket, ifreq& interfaceRequest);
    static InterfaceFlags retrieveFlags(SocketDescriptor socket, ifreq& interfaceRequest);
    static bool isNotLoopback(InterfaceFlags flags);
    static bool isConnected(InterfaceFlags flags);


};
