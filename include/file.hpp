#include <vector>
#include "student.hpp"

#define FILE_NAME "db.csv"
#pragma once

std::vector<Student> read_file();

Student write_student(Student student);

void delete_student(Student student);