#ifndef ptrace_module
#define ptrace_module

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <optional>

namespace PtraceModule
{
    struct DataStructure
    {
        std::string m_string_target_name{};
        std::string m_string_target_pid{};
    };

    enum class State
    {
        Attached,
        Detached,
        Stopped,
        Running,
        Exited
    };

    class Object
    {
        private:
            State m_state{State::Detached};
            DataStructure m_data_structure{};

            explicit Object(DataStructure&& data_structure) : 
                m_data_structure{std::move(data_structure)}
            {
                //empty body
            }

        public:
            Object() = delete;

            static std::optional<Object> instantiate(const std::string& target_process_name)
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
            
            const DataStructure& peek_data() const 
            {
                return m_data_structure;
            }

    };

};




#endif