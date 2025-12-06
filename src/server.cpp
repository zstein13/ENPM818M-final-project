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

void server() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        exit(1);
    }

    // Bind address and port to socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_sock);
        exit(1);
    }

    // Start listening for connections
    if (listen(server_sock, 5) < 0)
    {
        perror("Listen failed");
        close(server_sock);
        exit(1);
    }

    printf("HTTP server started on port %d\n", SERVER_PORT);

    // Accept client connections and handle them
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len)) > 0)
    {
        printf("Client connected\n");
        handle_request(client_sock);
    }

    // Close server socket
    close(server_sock);
}