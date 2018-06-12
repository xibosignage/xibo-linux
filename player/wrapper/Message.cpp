#include "Message.hpp"

Gst::Message::Message(GstMessage* handler, bool take_ownership) :
    m_handler(handler), m_take_ownership(take_ownership)
{   
}

Gst::MessageError Gst::Message::parse_error()
{
    gchar* debug = nullptr;
    GError* error = nullptr;
    gst_message_parse_error(m_handler, &error, &debug);

    std::string text = error->message;
    std::string debug_info = debug;

    g_free(debug);
    g_error_free(error);

    return Gst::MessageError{text, debug_info};
}

Gst::MessageType Gst::Message::type() const
{
    return static_cast<Gst::MessageType>(m_handler->type);
}

Gst::MessageError::MessageError(const std::string& text, const std::string& debug_info) :
    m_text(text), m_debug_info(debug_info)
{
}

const std::string&Gst::MessageError::get_text() const
{
    return m_text;
}

const std::string&Gst::MessageError::get_debug_info() const
{
    return m_debug_info;
}
