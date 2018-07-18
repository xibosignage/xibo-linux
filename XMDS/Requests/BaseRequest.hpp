#ifndef BASEREQUEST_HPP
#define BASEREQUEST_HPP

#include <string>
#include <tuple>
#include <sstream>

template<const char* ResponseName, typename... Args>
class BaseRequest
{
public:
    template<typename... T>
    BaseRequest(T... args) :
        m_server_key("serverKey"),
        m_hardware_key("hardwareKey"),
        m_fields(std::make_tuple(args...))
    {
    }

    template<typename T>
    class Field
    {
    public:
        Field(std::string_view name) : m_field_name(name) { }
        std::string type() const
        {
            if constexpr(std::is_same_v<T, std::string>)
                return "string";
            else if constexpr(std::is_same_v<T, int>)
                return "int";
            else if constexpr(std::is_same_v<T, double>)
                return "double";

            return "unsupported";
        }
        std::string name() const
        {
            return m_field_name;
        }
        void set_value(const T& value)
        {
            m_value = value;
        }
        T value() const
        {
            return m_value;
        }

    private:
        std::string m_field_name;
        T m_value;

    };

    std::string str() const;
    std::string server_key() const;
    void server_key(const std::string& value);
    std::string hardware_key() const;
    void hardware_key(const std::string& value);

protected:
    template<size_t Index> auto& field();
    template<size_t Index> const auto& field() const;

private:
    template<size_t...Ints> std::string create_request(std::index_sequence<Ints...>) const;

private:
    Field<std::string> m_server_key;
    Field<std::string> m_hardware_key;
    std::tuple<Field<Args>...> m_fields;

};

template<const char* ResponseName, typename... Args>
std::string BaseRequest<ResponseName, Args...>::str() const
{
    return create_request(std::make_index_sequence<sizeof...(Args)>{});
}

template<const char* ResponseName, typename... Args>
std::string BaseRequest<ResponseName, Args...>::server_key() const
{
    return m_server_key.value();
}

template<const char* ResponseName, typename... Args>
void BaseRequest<ResponseName, Args...>::server_key(const std::string& value)
{
    m_server_key.set_value(value);
}

template<const char* ResponseName, typename... Args>
std::string BaseRequest<ResponseName, Args...>::hardware_key() const
{
    return m_hardware_key.value();
}

template<const char* ResponseName, typename... Args>
void BaseRequest<ResponseName, Args...>::hardware_key(const std::string& value)
{
    m_hardware_key.set_value(value);
}

template<const char* ResponseName, typename... Args>
template<size_t Index>
auto& BaseRequest<ResponseName, Args...>::field()
{
    return std::get<Index>(m_fields);
}

template<const char* ResponseName, typename... Args>
template<size_t Index>
const auto& BaseRequest<ResponseName, Args...>::field() const
{
    return std::get<Index>(m_fields);
}

template<const char* ResponseName, typename... Args>
template<size_t...Ints>
std::string BaseRequest<ResponseName, Args...>::create_request(std::index_sequence<Ints...>) const
{
    std::stringstream stream;
    stream << R"(<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:tns="urn:xmds" xmlns:types="urn:xmds/encodedTypes" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">)";
    stream << R"(<soap:Body soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">)";
    stream << "<tns:" << ResponseName <<  ">";
    stream << "<" << m_server_key.name() << " xsi:type=\"xsd:" << m_server_key.type() << "\">" << m_server_key.value() << "</" << m_server_key.name() << ">";
    stream << "<" << m_hardware_key.name() << " xsi:type=\"xsd:" << m_hardware_key.type() << "\">" << m_hardware_key.value() << "</" << m_hardware_key.name() << ">";
    ((stream << "<" << field<Ints>().name() << " xsi:type=\"xsd:" << field<Ints>().type() << "\">" << field<Ints>().value() << "</" << field<Ints>().name() << ">"), ...);
    stream << "</tns:" << ResponseName <<  ">";
    stream << "</soap:Body>";
    stream << "</soap:Envelope>";

    return stream.str();
}

#endif // BASEREQUEST_HPP
