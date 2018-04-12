#pragma once

#include <cstdint>
#include <string>

namespace utilities
{
    uint32_t to_hex(const std::string& str_color);
    const std::string& example_dir();
    const std::string& xlf_file();
}
