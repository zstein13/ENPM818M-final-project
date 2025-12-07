#include <string>

#pragma once

enum class Code {
    OK=200,
    BAD_REQUEST=400,
    NOT_FOUND=404,
    NOT_ALLOWED=405,
    INTERNAL_ERROR=500,
    NOT_IMPLEMENTED=501
};

/**
 * @brief Get the status msg object
 * 
 * @param code 
 * @return std::string 
 */
std::string get_status_msg(Code code);
