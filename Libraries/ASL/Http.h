#pragma once

#include <ASL/ByteBuffer.h>
#include <ASL/CaseInsensitiveString.h>
#include <ASL/Forward.h>
#include <ASL/Types.h>

#include <unordered_map>

namespace http {

enum class Method : u8 {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

const char* to_string(const Method& method) noexcept
{
    switch (method) {
    case Method::GET:
        return "GET";
    case Method::HEAD:
        return "HEAD";
    case Method::POST:
        return "POST";
    case Method::PUT:
        return "PUT";
    case Method::DELETE:
        return "DELETE";
    case Method::CONNECT:
        return "CONNECT";
    case Method::OPTIONS:
        return "OPTIONS";
    case Method::TRACE:
        return "TRACE";
    case Method::PATCH:
        return "PATCH";
    default:
        return "<error-unknown-method>";
    }
}

enum class Version : u8 {
    V10,
    V11,
    V20,
};

const char* to_string(const Version& version)
{
    switch (version) {
    case Version::V10:
        return "HTTP/1.0";
    case Version::V11:
        return "HTTP/1.1";
    case Version::V20:
        return "HTTP/2.0";
    default:
        return "<error-unknown-version>";
    }
}

using Headers = std::unordered_map<CaseInsenitiveString, std::string>;

class LIB_API Request {
public:
    void serialize(const ByteBuffer&);
    void deserialize(ByteBuffer&);

private:
    Method m_method;
    std::string m_path;
    Version m_version;
    Headers m_headers;
    ByteBuffer m_body;
};

class LIB_API Response {
public:
    void serialize(const ByteBuffer&);
    void deserialize(ByteBuffer&);

private:
    Version m_version;
    u8 m_status;
    Headers m_headers;
    ByteBuffer m_body;
};

} // namespace http