#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <sstream>

#pragma once

#define SERVER_PORT 8081
#define BUFFER_SIZE 1024

/**
 * @brief 
 * 
 * @param client_sock 
 * @param response 
 */
void send_response(int client_sock, const char *response);

/**
 * @brief 
 * 
 * @param client_sock 
 */
void handle_request(int client_sock);

/**
 * @brief 
 * 
 */
void server();