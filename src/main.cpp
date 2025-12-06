#include <iostream>
#include <string>

#include "response.hpp"
#include "server.hpp"
#include "code.hpp"
#include "header.hpp"

int main() {
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