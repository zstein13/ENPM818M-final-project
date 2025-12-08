# ENPM818M-final-project
Implementing an HTTP Server for my final project for ENPM818M: Introduction to Networking and Distributed Systems 5G/6G.

The HTTP Server will be built using C++

## Project Requirements

- [x] HTTP Server can handle requests from an HTTP Client and returns a 200 response.
- [x] HTTP Server can route HTTP Client requests to certain endpoints and return endpoint specific information.
- [x] HTTP Server can handle GET requests and return server information.
- [x] HTTP Server can handle POST requests by updating  a “database” file on the server side.
- [x] HTTP Server can handle PUT requests by updating  a “database” file on the server side.
- [x] HTTP Server can handle DELETE requests by updating  a “database” file on the server side.
- [x] HTTP Server can handle different headers sent in the HTTP Client request.
- [x] HTTP Server implements error handling: 400 Error
- [x] HTTP Server implements error handling: 404 Error
- [x] HTTP Server implements error handling: 500 Error

## Endpoints

- GET /
- GET /about
- GET /students
    - Requires `User-Role: Viewer or Admin` header
- GET /student/\<email\>
    - Requires `User-Role: Viewer or Admin` header
- DELETE /student/\<email\>
    - Requires `User-Role: Admin` header
- POST /student
    - Requires `User-Role: Admin` header
    - Add email, name, and course as data fields
- PUT /student
    - Requires `User-Role: Admin` header
    - Add email, name, and course as data fields

## Building and Running the Project

To build and run the project, follow these steps:

1. Create a build directory and navigate into it:
    ```bash
    mkdir build && cd build
    ```

2. Generate the build files using CMake:
    ```bash
    cmake ..
    ```

3. Build the executable file:
    ```bash
    make
    ```

4. Run the executable file:
    ```bash
    ./http_server
    ```
