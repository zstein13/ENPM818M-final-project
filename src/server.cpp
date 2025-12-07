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
    // User-Role header
    std::string user_role;
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
        // Get user_role header value
        if (line.find(user_header) == 0) {
            user_role = line.substr(user_header.length() + 1);
            // Remove \r and \n from user_role string
            while (!user_role.empty() && (user_role.back() == '\r' || user_role.back() == '\n')) {
                user_role.pop_back();
            }
            std::cout << "user role: " << user_role << '\n';
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
        if (user_role == "Viewer" || user_role == "Admin"){
            handle_get_all(method, client_sock);
        }
        else {
            handle_error(Code::UNAUTHORIZED, method, client_sock);
        }
    }
    else if (endpoint == "/student")
    {
        if (user_role == "Admin") {
            if (method == "POST")
            {
                // Add content type and data
                handle_post_student(method, content_type, update_student, client_sock);
            }
            // Add PUT
            else if (method == "PUT")
            {
                handle_put_student(method, content_type, update_student, client_sock);
            }
            else
            {
                handle_error(Code::NOT_ALLOWED, method, client_sock);
            }
        } 
        else {
            handle_error(Code::UNAUTHORIZED, method, client_sock);
        }
    }
    else if (endpoint.find("/student/") != std::string::npos)
    {
        std::string email{endpoint.substr(9)};
        if (method == "GET")
        {
            if (user_role == "Viewer" || user_role == "Admin") {
                handle_get_student(method, email, client_sock);
            }
            else {
                handle_error(Code::UNAUTHORIZED, method, client_sock);
            }
        }
        else if (method == "DELETE")
        {
            if (user_role == "Admin"){
                handle_delete_student(method, email, client_sock);
            }
            else {
                handle_error(Code::UNAUTHORIZED, method, client_sock);
            }
        }
        else
        {
            handle_error(Code::NOT_ALLOWED, method, client_sock);
        }
    }
    else
    {
        handle_error(Code::NOT_FOUND, method, client_sock);
    }
}

void handle_home(std::string method, int client_sock)
{
    // Create repsonse object
    if (method != "GET")
    {
        handle_error(Code::NOT_ALLOWED, method, client_sock);
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
        handle_error(Code::NOT_ALLOWED, method, client_sock);
    }
    else
    {
        Response res = {Code::OK, method, "\"This HTTP server is a final project for ENPM818M.\""};
        send_response(client_sock, res);
    }
}

void handle_error(Code code, std::string method, int client_sock) {
    Response res{code, method, get_status_msg(code)};
    send_response(client_sock, res);
}

void handle_get_all(std::string method, int client_sock)
{
    if (method != "GET")
    {
        handle_error(Code::NOT_ALLOWED, method, client_sock);
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
    Code code;
    Response res;
    std::vector<Student> students{read_file()};

    // Find student using lambda function
    auto it = find_student(students, email);

    if (it != students.end()) {
        
        students.erase(it);
        
        code = update_file(students);
        
        std::string res_string;
        
        if (code == Code::OK) {
            res_string = "\"" + email + " was deleted from the database...\"";
        } 
        else {
            res_string = "\"Could not delete " + email + " from the database... \"";
        }
        res = {code, method, res_string};
    } 
    else {
        res = {Code::BAD_REQUEST, method, "Bad Request! Student doesn't exist in DB"};
    }
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
        handle_error(Code::NOT_IMPLEMENTED, method, client_sock);
    }

}

void handle_put_student(std::string method, std::string content_type, Student student, int client_sock) {
    Response res;
    Code code;
    if (content_type.substr(0, 33) == supported_content_type) {
        // Get list of students from read_file()
        std::vector<Student> students{read_file()};

        // Check to see if student is in students
        auto it = find_student(students, student.email);

        if (it == students.end())
        {
            // Student does not exist in file
            res = {Code::BAD_REQUEST, method, "Bad Request! Student does not exist in DB"};
        }
        else
        {
            // Modify it object with student fields update_file()
            it->name = student.name;
            it->course = student.course;

            code = update_file(students);
            res = {code, method, student.to_json()};
        }
        send_response(client_sock, res);
    }
    else {
        handle_error(Code::NOT_IMPLEMENTED, method, client_sock);
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
