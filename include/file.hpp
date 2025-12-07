#include <vector>
#include "student.hpp"
#include "code.hpp"

#define FILE_NAME "db.csv"
#pragma once

/**
 * @brief Read the entire contents of the file and returns a vector of Students
 * 
 * @return std::vector<Student> 
 */
std::vector<Student> read_file();

/**
 * @brief Finds a student from the email address
 * 
 * @param students 
 * @param email 
 * @return std::vector<Student>::iterator 
 */
std::vector<Student>::iterator find_student(std::vector<Student>& students, std::string email);

/**
 * @brief Updates the file with the Student objects in students
 * 
 * @param students 
 */
Code update_file(std::vector<Student> students);

/**
 * @brief Writes a new student to the file
 * 
 * @param student 
 */
Code write_student(Student student);
