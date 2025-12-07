#include <string>
#include <sstream>

#pragma once

struct Student
{
    std::string email;
    std::string name;
    std::string course;

    operator std::string() const {
        std::stringstream stream;

        stream << email << "," << name << "," << course;

        return stream.str();
    }

    const std::string to_json()
    {
        std::stringstream stream;

        stream << "{";
        // Email
        stream << "\"email\": \"" << email << "\",";
        // Name
        stream << "\"name\": \"" << name << "\",";
        // Course
        stream << "\"course\": \"" << course << "\"";
        stream << "}";

        return stream.str();
    }
};
