#include <string>
#include <sstream>
#include "code.hpp"

#pragma once

struct Response
{
    Code status;
    std::string request_type;
    std::string data;

    operator std::string() const {
        std::stringstream stream;
        stream << "{";
        // Status
        stream << "\"status\": \"" << static_cast<int>(status) << "\",";
        // request_type
        stream << "\"request_method\": \"" << request_type << "\",";
        // data
        stream << "\"data\": " << data;
        stream << "}";

        return stream.str();
    }
};
