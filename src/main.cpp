#include <iostream>
#include <string>

#include "response.hpp"
#include "server.hpp"

int main() {
    Response res{200, "GET", "Some data"};

    std::cout << "Response: " << static_cast<std::string>(res) << '\n';

    server();
}