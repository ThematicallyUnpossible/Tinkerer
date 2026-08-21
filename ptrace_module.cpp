#include "ptrace_module.h"
#include "io_helper.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <optional>
#include <sys/ptrace.h>
#include <sys/user.h>

namespace 
{
    std::optional<std::string> string_find_base(std::string look_inside_pid, const std::string& string_lookup_name)
    {
        std::string string_path = "/proc/" + look_inside_pid + "/maps";
        std::ifstream ifstream_path(string_path);

        std::string current_page{};
        while(std::getline(ifstream_path, current_page))
        {
            if(current_page.find(string_lookup_name) != std::string::npos)
            {
                return current_page.substr(0, current_page.find('-'));
            }
        }
        
        return std::nullopt;
    }

    void DEBUG_PRINT_LOADABLE_LIST(std::vector<PtraceModule::LibMeta>& list)
    {
        if(list.empty())
        {
            std::cerr << "(DEBUG) EMPTY QUEUE LIST" << "\n";
            return;
        }
        int i =  0;
        for(const auto& x : list)
        {
            std::cout << "Queued " << i << " | name : " << x.m_string_lib_name << " | path : " << x.m_string_lib_path << " | " << "\n";
            i++;
        }
        return;
    }
}



std::optional<PtraceModule::Object> PtraceModule::Object::instantiate(const std::string& target_process_name)
{
    for(const auto& entry : std::filesystem::directory_iterator("/proc"))
    {
        std::string entry_comm = entry.path()/"comm";
        std::ifstream ifstream_entry_comm(entry_comm);
        if(!ifstream_entry_comm)
        {
            continue;
        }
        std::string string_proc_name{};
        std::getline(ifstream_entry_comm, string_proc_name);
        if(string_proc_name == target_process_name)
        {
            TargetMetadata temporary
                {
                    .m_string_target_name = string_proc_name,
                    .m_string_target_pid = entry.path().filename().string()
                };
            
            //////////////////////////////////////////
            /////////////////GET////PROGRAM///BASE////
            //////////////////////////////////////////
            std::optional<std::string> target_process_base = string_find_base(temporary.m_string_target_pid, target_process_name);
            if(target_process_base)
            {
                temporary.m_string_target_base = target_process_base.value();
            }
            else
            {
                return std::nullopt;
            }

            //////////////////////////////////////////
            /////////GET////PROGRAM///LIBC////BASE////
            //////////////////////////////////////////
            std::optional<std::string> target_libc_base = string_find_base(temporary.m_string_target_pid, "libc");
            if(target_libc_base)
            {
                temporary.m_string_target_libc_base = target_libc_base.value();
            }
            else 
            {
                return std::nullopt;
            }

            return Object(std::move(temporary));
        }
    }
    return std::nullopt;
}

bool PtraceModule::Object::queue_loadable()
{
    std::string loadable_string_path = get_input<std::string>("Enter library path : ");

    //////////////////////////////////////////
    /////////VALIDATE/////LIBRARY/////////////
    //////////////////////////////////////////

    std::filesystem::path loadable_fs_path = loadable_string_path;
    if(!std::filesystem::exists(loadable_fs_path))
    {
        return false;
    }
    if(!(std::filesystem::is_regular_file(loadable_fs_path)))
    {
        return false;
    }
    if(loadable_fs_path.extension() != ".so")
    {
        return false;
    }

    std::string loadable_string_name = get_input<std::string>("Enter library name : ");

    m_loadable_list.push_back
    (
        {
            .m_string_lib_name =  loadable_string_name,
            .m_string_lib_path = loadable_string_path
        }
    );

    return true;
}

bool PtraceModule::Object::inject_loadable()
{
    DEBUG_PRINT_LOADABLE_LIST(m_loadable_list);
    if(m_loadable_list.empty())
    {
        return false;
    }

    unsigned lib_index{get_input<unsigned>("Choose queued library's index to be load : ", 0, (m_loadable_list.size()-1) )};

    LibMeta stolen_loadable{std::move(m_loadable_list[lib_index])};
    m_loadable_list.erase(m_loadable_list.begin()+lib_index); //this can be written when the lib is succesfully written
    DEBUG_PRINT_LOADABLE_LIST(m_loadable_list);

    //////////////////////////////////////////
    /////////////LOAD/////LIBRARY/////////////
    //////////////////////////////////////////

    unsigned long long ull_target_pid = std::stoull(m_target_metadata.m_string_target_pid, nullptr,  16);
    if(ptrace(PTRACE_ATTACH, ull_target_pid, nullptr, nullptr) == -1)
    {
        std::cerr << "ATTACH FAILED";
        return false;
    }


    user_regs_struct save, current;
    if(ptrace(PTRACE_GETREGS, ull_target_pid, &current, nullptr) == -1)
    {
        std::cerr << "FAILED TO GETREGS";
        return false;
    }    

    //save starting checkpoint
    save = current;

    constexpr unsigned long long SYSCALL_MMAP         {0xCC050F};
    constexpr unsigned long long MMAP_SYSCALL_NUMBER  {0x9};
    constexpr unsigned long long MMAP_ADDRESS         {0};
    constexpr unsigned long long MMAP_BYTES_SIZE      {0x1000};
    constexpr unsigned long long MMAP_FLAG_RWX        {0x7};
    constexpr unsigned long long MMAP_FLAG_PROTENIMOUS{0x22};
    constexpr unsigned long long MMAP_FLAG_FD         {0xFFFFFFFFFFFFFFFF};
    constexpr unsigned long long MMAP_ALLOCATE_OFFSET {0};

    current.rax = MMAP_SYSCALL_NUMBER;
    current.rdi = MMAP_ADDRESS;
    current.rsi = MMAP_BYTES_SIZE;
    current.rdx = MMAP_FLAG_RWX;
    current.r10 = MMAP_FLAG_PROTENIMOUS;
    current.r8  = MMAP_FLAG_FD;
    current.r9  = MMAP_ALLOCATE_OFFSET;

    

    return false;
}

const PtraceModule::TargetMetadata& PtraceModule::Object::peek_data() const
{
    return m_target_metadata;
}