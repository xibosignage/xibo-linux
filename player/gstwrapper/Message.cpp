#include "Message.hpp"

Gst::Message::Message(GstMessage* handler, bool takeOwnership) : handler_(handler), takeOwnership_(takeOwnership) {}

Gst::MessageError Gst::Message::parseError()
{
    gchar* debug = nullptr;
    GError* error = nullptr;
    gst_message_parse_error(handler_, &error, &debug);

    std::string text = error->message;
    std::string debugInfo = debug;

    g_free(debug);
    g_error_free(error);

    return Gst::MessageError{text, debugInfo};
}

Gst::MessageType Gst::Message::type() const
{
    return static_cast<Gst::MessageType>(handler_->type);
}

Gst::MessageError::MessageError(std::string_view text, std::string_view debugInfo) :
    text_(text),
    debugInfo_(debugInfo)
{
}

std::string_view Gst::MessageError::getText() const
{
    return text_;
}

std::string_view Gst::MessageError::getDebugInfo() const
{
    return debugInfo_;
}
