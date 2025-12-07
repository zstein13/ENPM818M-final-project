#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "file.hpp"
#include "student.hpp"
#include "code.hpp"

std::vector<Student> read_file()
{
    // Open File
    std::ifstream file;
    file.open(FILE_NAME);

    // Error handling
    if (!file.is_open())
    {
        std::cout << "Error! Could not open file..." << "\n";
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

std::vector<Student>::iterator find_student(std::vector<Student>& students, std::string email)
{
    // find student with lambda function
    auto it{std::find_if(students.begin(), students.end(),
        [email](const Student& student) { return student.email == email; })
    };

    return it;
}

Code update_file(std::vector<Student> students) {
    // Get the first line from FILE_NAME
    Code code;
    std::ifstream file_read;
    file_read.open(FILE_NAME);

    // Error handling
    if (!file_read.is_open())
    {
        code = Code::INTERNAL_ERROR;
    }

    std::string file_head;
    // Call getline to ignore first row
    std::getline(file_read, file_head);
    file_read.close();

    std::ofstream file_write(FILE_NAME);
    
    if (!file_write.is_open()) {
        code = Code::INTERNAL_ERROR;
    } else {
        file_write << file_head << "\n";

        for (const Student& student : students) {
            file_write << static_cast<std::string>(student) << "\n";
        }
        file_write.close();
        code = Code::OK;
    }
    
    return code;
}

Code write_student(Student student) {
    // Append to the file
    std::ofstream file_write(FILE_NAME, std::ios::app);
    Code code;

    if (!file_write.is_open())
    {
        code = Code::INTERNAL_ERROR;

    } else {
        file_write << static_cast<std::string>(student) << "\n";
        file_write.close();
        code = Code::OK;
    }
    return code;
}