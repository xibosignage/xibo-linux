#include "Message.hpp"

Gst::Message::Message(GstMessage* handler, bool takeOwnership) : m_handler(handler), m_takeOwnership(takeOwnership) {}

Gst::MessageError Gst::Message::parseError()
{
    gchar* debug = nullptr;
    GError* error = nullptr;
    gst_message_parse_error(m_handler, &error, &debug);

    std::string text = error->message;
    std::string debugInfo = debug;

    g_free(debug);
    g_error_free(error);

    return Gst::MessageError{text, debugInfo};
}

Gst::MessageType Gst::Message::type() const
{
    return static_cast<Gst::MessageType>(m_handler->type);
}

Gst::MessageError::MessageError(std::string_view text, std::string_view debugInfo) :
    m_text(text),
    m_debugInfo(debugInfo)
{
}

std::string_view Gst::MessageError::getText() const
{
    return m_text;
}

std::string_view Gst::MessageError::getDebugInfo() const
{
    return m_debugInfo;
}
