#include "ptrace_module.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <optional>

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

            
            
            return Object(std::move(temporary));
        }
    }
    return std::nullopt;
}

const PtraceModule::DataStructure& PtraceModule::Object::peek_data() const
{
    return m_data_structure;
}