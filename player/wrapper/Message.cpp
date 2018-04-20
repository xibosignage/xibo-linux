#include "Message.hpp"

Gst::Message::Message(GstMessage* handler) :
    m_handler(handler)
{
    gchar* debug = nullptr;
    GError* error = nullptr;
//    gst_message_parse_error(m_handler, &error, &debug);

//    m_message = error->message;
//    m_debug_info = debug;

//    g_free(debug);
//    g_error_free(error);
}

std::string Gst::Message::get_message() const
{
    return m_message;
}

std::string Gst::Message::get_debug_info() const
{
    return m_debug_info;
}

Gst::MessageType Gst::Message::type() const
{
    return static_cast<MessageType>(m_handler->type);
}
