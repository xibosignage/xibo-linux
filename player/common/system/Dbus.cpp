#include "Dbus.hpp"

#include "common/logger/Logging.hpp"

#include <cstdio>

Dbus::Error::Error(DBusError error) : PlayerRuntimeError("DBus", ""), error_(error) {}

// TODO managed objects

const char* Dbus::Error::what() const noexcept
{
    return error_.name;
}

void Dbus::preventSleep()
{
    DBusConnection* connection = nullptr;
    DBusMessage* message = nullptr;
    dbus_error_init(&error_);

    try
    {
        char buff[12];
        sprintf(buff, "%d", getpid());
        const char* pid = buff;

        connection = connectToDbus();
        message = createInhibitCall(pid, "Xibo Player");
        auto result = sendAndWaitForResponse(connection, message);

        Log::debug("[DBus] ScreenSaver is suspended: {}", result);
    }
    catch (std::exception& e)
    {
        Log::error("[DBus] ScreenSaver wont'be suspended: {}", e.what());
    }

    if (connection) dbus_connection_unref(connection);
    if (message) dbus_message_unref(message);
}

DBusConnection* Dbus::connectToDbus()
{
    auto connection = dbus_bus_get(DBUS_BUS_SESSION, &error_);
    if (!connection) throw Dbus::Error{error_};

    return connection;
}

DBusMessage* Dbus::createInhibitCall(const char* pid, const char* reason)
{
    auto message = dbus_message_new_method_call(
        "org.freedesktop.ScreenSaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", "Inhibit");
    if (!message) throw Dbus::Error{error_};

    bool result =
        dbus_message_append_args(message, DBUS_TYPE_STRING, &pid, DBUS_TYPE_STRING, &reason, DBUS_TYPE_INVALID);
    if (!result)
    {
        dbus_message_unref(message);
        throw Dbus::Error{error_};
    }

    return message;
}

uint32_t Dbus::sendAndWaitForResponse(DBusConnection* connection, DBusMessage* message)
{
    uint32_t result = 0;

    auto reply = dbus_connection_send_with_reply_and_block(connection, message, DBUS_TIMEOUT_USE_DEFAULT, &error_);
    if (!reply) throw Dbus::Error{error_};

    bool success = dbus_message_get_args(reply, &error_, DBUS_TYPE_UINT32, &result, DBUS_TYPE_INVALID);
    if (!success)
    {
        dbus_message_unref(reply);
        throw Dbus::Error{error_};
    }

    return result;
}
