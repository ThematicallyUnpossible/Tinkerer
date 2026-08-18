#include "ptrace_module.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <optional>


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
            DataStructure temporary
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

            return Object(std::move(temporary));
        }
    }
    return std::nullopt;
}

const PtraceModule::DataStructure& PtraceModule::Object::peek_data() const
{
    return m_data_structure;
}