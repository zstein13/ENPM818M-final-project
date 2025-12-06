#include <string>
#include <sstream>
#include "code.hpp"
#include "response.hpp"

#pragma once

struct Header
{
    Response res;
    Code code{res.status};
    size_t content_length{static_cast<std::string>(res).length()};

    operator std::string() const
    {
        std::stringstream stream;
        
        stream << "HTTP/1.1 " << static_cast<int>(code) << " " << get_status_msg(code) << "\r\n";
        stream << "Content-Type: application/json\r\n";
        stream << "Content-Length: " << content_length << "\r\n";
        stream << "\r\n";

        return stream.str();
    }
};
