#include "Uri.hpp"

#include "common/Utils.hpp"
#include "common/fs/FilePath.hpp"
#include "common/types/internal/UriParser.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <istream>

std::string Uri::removeEscapedSymbols(std::string url)
{
    //    boost::replace_all(url, "%21", "!");
    //    boost::replace_all(url, "%23", "#");
    //    boost::replace_all(url, "%24", "$");
    //    boost::replace_all(url, "%25", "%");
    //    boost::replace_all(url, "%26", "&");
    //    boost::replace_all(url, "%27", "'");
    //    boost::replace_all(url, "%28", "(");
    //    boost::replace_all(url, "%29", ")");
    //    boost::replace_all(url, "%2A", "*");
    //    boost::replace_all(url, "%2B", "+");
    //    boost::replace_all(url, "%2C", ",");
    boost::replace_all(url, "%2F", "/");
    boost::replace_all(url, "%3A", ":");
    //    boost::replace_all(url, "%3B", ";");
    //    boost::replace_all(url, "%3D", "=");
    //    boost::replace_all(url, "%3F", "?");
    //    boost::replace_all(url, "%40", "@");
    //    boost::replace_all(url, "%5B", "]");
    //    boost::replace_all(url, "%5D", "[");

    return url;
}

Uri Uri::fromString(const std::string& str)
{
    UriParser parser;
    return parser.parse(removeEscapedSymbols(str));
}

Uri Uri::fromFile(const FilePath& path)
{
    return Uri{Scheme{"file"}, path.string()};
}

Uri::Uri(const Scheme& scheme, const Authority& authority, const std::string& path) :
    scheme_(scheme),
    authority_(authority),
    path_(path)
{
}

Uri::Uri(const Scheme& scheme, const Host& host, const std::string& path) :
    scheme_(scheme),
    authority_(Authority{{}, host, Uri::Port::fromScheme(scheme)}),
    path_(path)
{
}

Uri::Uri(const Scheme& scheme, const std::string& path) : scheme_(scheme), path_(path) {}

std::string Uri::string() const
{
    std::string uri;

    uri += static_cast<std::string>(scheme_) + "://";
    if (authority_)
    {
        if (auto info = authority_->optionalUserInfo())
        {
            uri += static_cast<std::string>(info.value()) + "@";
        }

        uri += static_cast<std::string>(authority_->host());

        if (auto port = authority_->optionalPort(); port && DefaultPorts.at(scheme_) != port)
        {
            uri += ":" + port->string();
        }
    }
    uri += path_;

    return uri;
}

boost::optional<Uri::Authority> Uri::optionalAuthority() const
{
    return authority_;
}

Uri::Authority Uri::authority() const
{
    assert(authority_);

    return authority_.value();
}

const Uri::Scheme& Uri::scheme() const
{
    return scheme_;
}

const std::string& Uri::path() const
{
    return path_;
}

std::ostream& operator<<(std::ostream& out, const Uri& uri)
{
    out << "Scheme: " << static_cast<std::string>(uri.scheme());
    if (auto auth = uri.optionalAuthority())
    {
        if (auto info = auth->optionalUserInfo())
        {
            out << " UserInfo: " << static_cast<std::string>(info.value());
        }
        out << " HostType: " << Utils::toString(auth->host().type());
        out << " Host: " << static_cast<std::string>(auth->host());
        if (auto port = auth->optionalPort())
        {
            out << " Port: " << static_cast<unsigned short>(port.value());
        }
    }
    out << " Target: " << uri.path();

    return out;
}

bool operator==(const Uri& first, const Uri& second)
{
    return first.string() == second.string();
}

bool operator!=(const Uri& first, const Uri& second)
{
    return !(first == second);
}
