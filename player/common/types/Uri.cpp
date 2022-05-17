#include "Uri.hpp"

#include "common/Utils.hpp"
#include "common/fs/FilePath.hpp"
#include "common/types/internal/UriParser.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma_numeric.hpp>
#include <boost/spirit/include/karma_uint.hpp>

#include <string>
#include <istream>

namespace bsq = boost::spirit::qi;
namespace bk = boost::spirit::karma;

bsq::int_parser<unsigned char, 16, 2, 2> hex_byte;

template <typename InputIterator>
struct unescaped_string
    : bsq::grammar<InputIterator, std::string(char const *)> {
  unescaped_string() : unescaped_string::base_type(unesc_str) {
    unesc_char.add("+", ' ');

    unesc_str = *(unesc_char | "%" >> hex_byte | bsq::char_);
  }

  bsq::rule<InputIterator, std::string(char const *)> unesc_str;
  bsq::symbols<char const, char const> unesc_char;
};

std::string Uri::unescape(const std::string &input) {
  std::string retVal;
  retVal.reserve(input.size());
  typedef std::string::const_iterator iterator_type;

  char const *start = "";
  iterator_type beg = input.begin();
  iterator_type end = input.end();
  unescaped_string<iterator_type> p;

  if (!bsq::parse(beg, end, p(start), retVal))
    retVal = input;
  return retVal;
}

Uri Uri::fromString(const std::string& str)
{
    UriParser parser;
    return parser.parse(unescape(str));
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
