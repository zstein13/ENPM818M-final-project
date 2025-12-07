#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "server.hpp"
#include "response.hpp"
#include "code.hpp"
#include "header.hpp"
#include "file.hpp"
#include "student.hpp"

Student parse_data(std::string data) {
    Student student;

    std::stringstream data_stream{data};
    std::string key_value;
    // Break up data_stream into key/value pairs
    while (std::getline(data_stream, key_value, '&'))
    {
        // Get key and value from key_value. '=' char is the delimiter
        size_t equal_position{key_value.find('=')};
        std::string key{key_value.substr(0,equal_position)};
        std::string value{key_value.substr(equal_position+1)};

        // Set student values
        if (key == "email") {
            student.email = value;
        }
        else if (key == "name") {
            student.name = value;
        }
        else if (key == "course") {
            student.course = value;
        }
    }

    return student;
}

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
    char buffer[buffer_size];
    // Read from socket
    ssize_t bytes_read = read(client_sock, buffer, sizeof(buffer));

    if (bytes_read < 0)
    {
        perror("Read failed");
        return;
    }

    std::string buffer_str(buffer, bytes_read);
    std::cout << buffer_str << '\n';

    // String parsing variables
    std::stringstream request_stream{buffer_str};
    // HTTP method
    std::string method;
    // Request Endpoint
    std::string endpoint;
    // Content-Type header
    std::string content_type;
    // POST/PUT data
    std::string data;
    // Student object to update db with
    Student update_student;

    // Get method and endpoint from request_stream
    request_stream >> method >> endpoint;

    // Process request stream
    std::string line;
    while (std::getline(request_stream, line))
    {
        // Check for content type
        if (line.find("Content-Type:") == 0) {
            content_type = line.substr(14);
        }
        // Process data if content type is application/x-www-form-urlencoded
        if (content_type.substr(0,33) == supported_content_type) {
            // Check if line is empty (i.e. end of headers)
            if (line.empty() || line == "\r" || line == "\n") {
                // Get data line
                std::getline(request_stream, data);
                update_student = parse_data(data);
            }
        }
    }

    // Endpoint handling
    if (endpoint == "/")
    {
        handle_home(method, client_sock);
    }
    else if (endpoint == "/about")
    {
        handle_about(method, client_sock);
    }
    else if (endpoint == "/students")
    {
        handle_get_all(method, client_sock);
    }
    else if (endpoint == "/student")
    {
        if (method == "POST")
        {
            // Add content type and data
            handle_post_student(method, content_type, update_student, client_sock);
        }
        // Add PUT
        // else if (method == "PUT") {
            // handle_put_student(method, client_sock);
        // }
        else
        {
            handle_not_allowed(method, client_sock);
        }
    }
    else if (endpoint.find("/student/") != std::string::npos)
    {
        std::string email{endpoint.substr(9)};
        if (method == "GET")
        {
            handle_get_student(method, email, client_sock);
        }
        else if (method == "DELETE")
        {
            handle_delete_student(method, email, client_sock);
        }
        else
        {
            handle_not_allowed(method, client_sock);
        }
    }
    else
    {
        handle_not_found(method, client_sock);
    }
}

void handle_home(std::string method, int client_sock)
{
    // Create repsonse object
    if (method != "GET")
    {
        handle_not_allowed(method, client_sock);
    }
    else
    {
        Response res = {Code::OK, method, "\"Welcome to the basic HTTP Server!\""};
        // Send welcome message
        send_response(client_sock, res);
    }
}

void handle_about(std::string method, int client_sock)
{
    // Create repsonse object
    if (method != "GET")
    {
        handle_not_allowed(method, client_sock);
    }
    else
    {
        Response res = {Code::OK, method, "\"This HTTP server is a final project for ENPM818M.\""};
        send_response(client_sock, res);
    }
}

void handle_not_found(std::string method, int client_sock)
{
    // Create repsonse object
    Response res{Code::NOT_FOUND, method, "\"Error! Endpoint not found...\""};
    send_response(client_sock, res);
}

void handle_not_allowed(std::string method, int client_sock)
{
    // Create repsonse object
    Response res = {Code::NOT_ALLOWED, method, "\"405 Method Not Allowed...\""};
    send_response(client_sock, res);
}

void handle_not_implemented(std::string method, int client_sock) {
    Response res = {Code::NOT_IMPLEMENTED, method, "\"501 Not Implemented. Server is unable to fulfill the request...\""};
    send_response(client_sock, res);
}

void handle_get_all(std::string method, int client_sock)
{
    if (method != "GET")
    {
        handle_not_allowed(method, client_sock);
    }
    else
    {
        std::vector<Student> students{read_file()};
        std::stringstream stream;

        bool first{true};
        // Create student list
        stream << "[";
        for (Student stud : students)
        {
            if (!first)
            {
                stream << ",";
            }
            first = false;
            stream << stud.to_json();
        }

        stream << "]";

        Response res = {Code::OK, method, stream.str()};
        send_response(client_sock, res);
    }
}

void handle_get_student(std::string method, std::string email, int client_sock)
{
    std::vector<Student> students{read_file()};

    // Create Lambda function to find student based off of email
    auto it = find_student(students, email);

    Response res;
    if (it != students.end())
    {
        // Found student
        res = {Code::OK, method, it->to_json()};
    }
    else
    {
        res = {Code::BAD_REQUEST, method, "\"400 Bad Request...\""};
    }
    send_response(client_sock, res);
}

void handle_delete_student(std::string method, std::string email, int client_sock)
{
    std::vector<Student> students{read_file()};

    // Find student using lambda function
    auto it = find_student(students, email);
    students.erase(it);

    update_file(students);

    std::stringstream res_string;

    res_string << "\"" << email << " was deleted from the database... \"";

    Response res = {Code::OK, method, res_string.str()};
    send_response(client_sock, res);
}

void handle_post_student(std::string method, std::string content_type, Student student, int client_sock)
{
    Response res;
    Code code;
    if (content_type.substr(0, 33) == supported_content_type)
    {
        // Get list of students from read_file()
        std::vector<Student> students{read_file()};

        // Check to see if student is in students
        auto it = find_student(students, student.email);

        if (it == students.end()) {
            // New student. Write student to DB
            code = write_student(student);
            res = {code, method, student.to_json()};
        }
        else {
            // Student already exists in file
            res = {Code::BAD_REQUEST, method, "Bad Request! Student already exists in DB"};
        }

        send_response(client_sock, res);
    }
    else {
        handle_not_implemented(method, client_sock);
    }

}

void server()
{
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
    server_addr.sin_port = htons(server_port);

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

    printf("HTTP server started on port %d\n", server_port);

    // Accept client connections and handle them
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len)) > 0)
    {
        printf("Client connected\n");

        // Add fork logic to handle multiple connections
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Fork failed");
            close(client_sock);
            continue;
        }

        if (pid == 0)
        {
            close(server_sock);
            handle_request(client_sock);
            exit(0);
        }
        else
        {
            close(client_sock);
        }
    }

    // Close server socket
    close(server_sock);
}
