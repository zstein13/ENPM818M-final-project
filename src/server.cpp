#include <string>
#include <sstream>

#include "server.hpp"
#include "response.hpp"
#include "code.hpp"
#include "header.hpp"

void send_response(int client_sock, Response response)
{
    Header head{response};

    std::string server_response{static_cast<std::string>(head) + static_cast<std::string>(response)};
    // Send welcome message
    write(client_sock, server_response.c_str(), server_response.length());

    close(client_sock);
}

void handle_request(int client_sock)
{
    char buffer[BUFFER_SIZE];
    // Read from socket
    ssize_t bytes_read = read(client_sock, buffer, sizeof(buffer));

    if (bytes_read < 0)
    {
        perror("Read failed");
        return;
    }

    std::string buffer_str(buffer, bytes_read);
    std::cout << buffer_str << '\n';

    std::stringstream request_stream{buffer_str};
    std::string method;
    std::string endpoint;

    request_stream >> method >> endpoint;

    if (endpoint == "/") {
        handle_home(method, client_sock);
    } else if (endpoint == "/about") {
        handle_about(method, client_sock);
    } else {
        handle_not_found(method, client_sock);
    }

}

void handle_home(std::string method, int client_sock) {
    // Create repsonse object
    Response res{Code::OK, method, "Welcome to the basic HTTP Server!"};
    
    // Send welcome message
    send_response(client_sock, res);

}

void handle_about(std::string method, int client_sock) {
    // Create repsonse object
    Response res{Code::OK, method, "This HTTP server is a final project for ENPM818M."};

    send_response(client_sock, res);
}

void handle_not_found(std::string method, int client_sock)
{
    // Create repsonse object
    Response res{Code::NOT_FOUND, method, "Error! Endpoint not found..."};

    send_response(client_sock, res);
}
