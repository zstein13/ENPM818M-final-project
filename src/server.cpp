#include <string>

#include "server.hpp"
#include "response.hpp"
#include "code.hpp"
#include "header.hpp"

void send_response(int client_sock, std::string response)
{
    write(client_sock, response.c_str(), response.length());
}

void handle_request(int client_sock)
{
    char buffer[BUFFER_SIZE];
    std::string buffer_str;
    buffer_str.reserve(BUFFER_SIZE);
    buffer_str = buffer;

    // Create repsonse object
    Response res{Code::OK, "GET", "Hi Abby!"};
    Header head{res};

    std::string server_response{static_cast<std::string>(head) + static_cast<std::string>(res)};
    // Send welcome message
    send_response(client_sock, server_response);

    close(client_sock);
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