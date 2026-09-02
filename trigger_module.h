#ifndef trigger_module
#define trigger_module
#include "io_helper.h"
#include "utility.h"
#include <cstring>
#include <optional>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <cstdint>

inline void trigger_hook_fcn(const std::string& target_pid)
{
    clear_input();
    std::string lib_name = get_input<std::string>("Enter lib name : ");
    std::optional<std::string> lib_base = string_find_base(target_pid, lib_name);
    if(!lib_base)
    {
        std::cerr << "UNABLE TO FIND LIB BASE" << "\n";
        return;
    }

    std::optional<std::string> target_base =  string_find_base(target_pid, "DebugMe");
    if(!target_base)
    {
        std::cerr << "UNABLE TO FIND TARGET BASE" << "\n";
        return;
    }

    unsigned long long ull_self_base = std::stoull(target_base.value(), nullptr,  16);

    uint8_t bytes_array[14] = 
    {
        0xFF,
        0x25,
        0x00,
        0x00,
        0x00,
        0x00
    };

    unsigned long long ull_lib_base = std::stoull(lib_base.value(), nullptr, 16);
    unsigned long long ull_hook_addr = ull_lib_base + 0x1119;
    memcpy(&bytes_array[6], &ull_hook_addr, 8);

    std::cout << "TARGET PID : " << target_pid << "\n";
    std::string target_mem_path = "/proc/"+target_pid+"/mem";
    int target_fd = open(target_mem_path.c_str(), O_RDWR);
    if(target_fd != -1)
    {
        std::cout << "HANDLE TO PROGRAM PID : " << target_pid << " SUCCEEDED." << "\n";
    }
    else
    {
        std::cerr << "UNABLE TO GET HANDLE : " << std::strerror(errno) << "\n";
        return;
    }
    lseek(target_fd, (ull_self_base+0x2377),SEEK_SET);    
    write(target_fd, bytes_array, 14);

    return;
}

#endif