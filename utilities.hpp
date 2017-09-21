#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Parser.hpp"

#include <memory>
#include <optional>
#include <wx/xml/xml.h>

namespace utilities
{
    template <typename Parser> std::shared_ptr<Parser> GetParser(wxXmlNode* node)
    {
        return std::make_shared<Parser>(node);
    }

    template <typename T> std::optional<T> GetValue(const wxString& optionName)
    {
        if(!optionName.IsEmpty())
            return std::stoi(optionName.ToStdString());
        return {};
    }

    template <> inline std::optional<wxString> GetValue(const wxString& optionName)
    {
        return optionName;
    }
}

#endif // UTILITIES_HPP
