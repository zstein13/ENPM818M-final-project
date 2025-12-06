#include <string>
#include <sstream>

struct Record
{
    std::string email;
    std::string name;
    std::string course;

    operator std::string() const {
        std::stringstream stream;

        stream << email << "," << name << "," << course;

        return stream.str();
    }
};
