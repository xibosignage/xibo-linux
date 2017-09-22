#include "utilities.hpp"

template <> std::optional<wxString> utilities::GetValue(const wxString& optionName)
{
    return optionName;
}

template <> std::optional<ScaleType> utilities::GetValue(const wxString& optionName)
{
    if(!optionName.empty())
    {
        if(optionName == "center")
            return ScaleType::Center;
        else if(optionName == "stretch")
            return ScaleType::Stretch;
        else
            return ScaleType::Invalid;
    }
    return {};
}

template <> std::optional<Align> utilities::GetValue(const wxString& optionName)
{
    if(!optionName.empty())
    {
        if(optionName == "left")
            return Align::Left;
        else if(optionName == "center")
            return Align::Center;
        else if(optionName == "right")
            return Align::Right;
        else
            return Align::Invalid;
    }
    return {};
}

template <> std::optional<Valign> utilities::GetValue(const wxString& optionName)
{
    if(!optionName.empty())
    {
        if(optionName == "top")
            return Valign::Top;
        else if(optionName == "middle")
            return Valign::Middle;
        else if(optionName == "bottom")
            return Valign::Bottom;
        else
            return Valign::Invalid;
    }
    return {};
}

template <> std::optional<Render> utilities::GetValue(const wxString& optionName)
{
    if(!optionName.empty())
    {
        if(optionName == "native")
            return Render::Native;
        else if(optionName == "html")
            return Render::HTML;
        else
            return Render::Invalid;
    }
    return {};
}
