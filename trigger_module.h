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
    if(lib_base)
    {
        std::cerr << "lib is loaded : 0x" << lib_base.value() << "\n";
    }

    std::cout << "test" << "\n";
    std::string target_mem_path = "/proc/"+target_pid+"/mem";
    int target_fd = open(target_mem_path.c_str(), O_RDWR);
    if(target_fd != -1)
    {
        std::cout << "HANDLE TO PROGRAM PID : " << target_pid << " SUCCEEDED." << "\n";
    }
    else
    {
        std::cerr << "Failed to open mem path. Error: " << std::strerror(errno) << "\n";
        return;
    }

    std::string string_self_pid = std::to_string(getpid());
    std::optional<std::string> self_base =  string_find_base(string_self_pid, "Tinkerer");
    std::cout << "SELF BASE : " <<  self_base.value() <<  "\n";
    unsigned long long ull_self_base = std::stoull(self_base.value(), nullptr,  16);



    

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
    memcpy(&bytes_array[6], &ull_hook_addr, sizeof(ull_lib_base));

    lseek(target_fd, (ull_self_base+0x2377),SEEK_SET);    
    write(target_fd, bytes_array, 14);


    // hook_bytes[0] = 0xFF;
    // hook_bytes[1] = 0x25;
    // hook_bytes[2] = 0x00;
    // hook_bytes[3] = 0x00;
    // hook_bytes[4] = 0x00;
    // hook_bytes[5] = 0x00;
    // std::memcpy(&hook_bytes[6], &hook_address, 8);

    // std::string mem_path = "/proc/" + std::to_string(pid) + "/mem";
    // int fd = open(mem_path.c_str(), O_RDWR);
    // if (fd == -1) {
    //     return false;
    // }

    // lseek(fd, target_address, SEEK_SET);
    // ssize_t byte_written = write(fd, hook_bytes, 14);
    // close(fd);

    return;
}

#endif