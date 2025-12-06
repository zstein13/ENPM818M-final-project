#include <string>
#include <sstream>

struct Response
{
    int status;
    std::string request_type;
    std::string data;

    operator std::string() const {
        std::stringstream stream;
        stream << "{";
        // Status
        stream << "\"status\": \"" << std::to_string(status) << "\",";
        // request_type
        stream << "\"request_type\": \"" << request_type << "\",";
        // data
        stream << "\"data\": \"" << data;
        stream << "}";

        return stream.str();
    }
}; 
