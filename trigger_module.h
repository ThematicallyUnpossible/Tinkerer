#ifndef trigger_module
#define trigger_module
#include "io_helper.h"
#include "utility.h"
#include <optional>

inline void trigger_hook_fcn(const std::string& target_pid)
{
    std::string lib_name = get_input<std::string>("Enter lib name : ");
    std::optional<std::string> lib_base = string_find_base(target_pid, lib_name);
    if(lib_base)
    {
        std::cerr << "lib is loaded" << "\n";
        return;
    }

    return;
}

#endif