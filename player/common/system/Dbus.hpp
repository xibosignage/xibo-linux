#pragma once

#include "common/PlayerRuntimeError.hpp"

#include <dbus/dbus.h>
#include <stdint.h>
#include <unistd.h>

class Dbus
{
public:
    struct Error : PlayerRuntimeError
    {
        Error(DBusError error);

        const char* what() const noexcept override;

    private:
        DBusError error_;
    };

    void preventSleep();

private:
    DBusConnection* connectToDbus();
    DBusMessage* createInhibitCall(const char* pid, const char* reason);

    uint32_t sendAndWaitForResponse(DBusConnection* connection, DBusMessage* message);

private:
    DBusError error_;
};
