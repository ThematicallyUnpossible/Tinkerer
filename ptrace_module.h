#ifndef ptrace_module
#define ptrace_module

#include <string>
#include <optional>

namespace PtraceModule
{
    struct DataStructure
    {
        std::string m_string_target_name{};
        std::string m_string_target_pid{};

        std::string m_string_target_base{};
        std::string m_string_target_libc_base{};
    };

    enum class State
    {
        NotReady,
        Attached,
        Detached,
        Stopped,
        Running,
        Exited
    };

    class Object
    {
        private:
            State m_state{State::NotReady};
            DataStructure m_data_structure{};

            explicit Object(DataStructure&& data_structure) : 
                m_data_structure{std::move(data_structure)}
            {
                //empty body
            }


        public:
            Object() = delete;

            static std::optional<Object> instantiate(const std::string& target_process_name);
            const DataStructure& peek_data() const;


    };

};




#endif