#include <iostream>
#include <string>

#include "response.hpp"
#include "server.hpp"
#include "code.hpp"
#include "header.hpp"
#include "record.hpp"

int main() {
<<<<<<< Updated upstream
    server();
=======
    // server();
    Record rec{"zstein13@umd.edu", "Zach", "ENPM818M"};
    std::cout << "Record: " << static_cast<std::string>(rec) << '\n';
>>>>>>> Stashed changes
}