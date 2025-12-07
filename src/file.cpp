#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "file.hpp"
#include "student.hpp"

std::vector<Student> read_file()
{
    // Open File
    std::ifstream file;
    file.open(FILE_NAME);

    // Error handling
    if (!file.is_open())
    {
        std::cout << "Error! Could not open file..." << '\n';
    }

    std::string row;
    // Call getline to ignore first row
    std::getline(file, row);

    // CSV processing fields
    std::vector<Student> students;
    Student student;
    std::string field;
    std::stringstream stream;
    char comma{','};

    while (std::getline(file, row))
    {
        stream = static_cast<std::stringstream>(row);

        // Get Email
        if (std::getline(stream, field, comma))
        {
            student.email = field;
        }
        // Get Name
        if (std::getline(stream, field, comma))
        {
            student.name = field;
        }
        // Get Course
        if (std::getline(stream, field, comma))
        {
            student.course = field;
        }
        students.push_back(student);
    }

    file.close();

    return students;
}

// void write_student(Student student) {

// }