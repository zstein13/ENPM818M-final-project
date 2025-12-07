#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <sstream>

#include "response.hpp"
#include "student.hpp"

#pragma once

const int server_port{8081};
const int buffer_size{1024};
const std::string supported_content_type{"application/x-www-form-urlencoded"};

/**
 * @brief 
 * 
 * @param data 
 * @return Student 
 */
Student parse_data(std::string data);

/**
 * @brief 
 * 
 * @param client_sock 
 * @param response 
 */
void send_response(int client_sock, Response response);

/**
 * @brief 
 * 
 * @param client_sock 
 */
void handle_request(int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param endpoint 
 * @param client_sock 
 */
void handle_home(std::string method, int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param endpoint 
 * @param client_sock 
 */
void handle_about(std::string method, int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param client_sock 
 */
void handle_not_found(std::string method, int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param client_sock 
 */
void handle_not_allowed(std::string method, int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param client_sock 
 */
void handle_not_implemented(std::string method, int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param client_sock 
 */
void handle_get_all(std::string method, int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param email 
 * @param client_sock 
 */
void handle_get_student(std::string method, std::string email, int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param email 
 * @param client_sock 
 */
void handle_delete_student(std::string method, std::string email, int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param content_type 
 * @param data 
 * @param client_sock 
 */
void handle_post_student(std::string method, std::string content_type, Student student, int client_sock);

/**
 * @brief 
 * 
 * @param method 
 * @param content_type 
 * @param data 
 * @param client_sock 
 */
void handle_put_student(std::string method, std::string content_type, std::string data, int client_sock);

/**
 * @brief 
 * 
 */
void server();