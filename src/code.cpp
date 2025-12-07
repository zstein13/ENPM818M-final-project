#include "code.hpp"

std::string get_status_msg(Code code) {
    
    switch (code)
    {
    case Code::OK:
        return "\"OK\"";
        break;
    case Code::UNAUTHORIZED:
        return "\"Unauthorized\"";
        break;
    case Code::BAD_REQUEST:
        return "\"Bad Request\"";
        break;
    case Code::NOT_FOUND:
        return "\"Not Found\"";
        break;
    case Code::INTERNAL_ERROR:
        return "\"Internal Error\"";
        break;
    case Code::NOT_IMPLEMENTED:
        return "\"Not Implemented\"";
        break;
    default:
        return "\"Error\"";
        break;
    }
}