#ifndef ptrace_module
#define ptrace_module

#include <string>
#include <optional>

namespace PtraceModule
{
    struct DataStructure
    {
        std::string m_string_target_path{};
        std::string m_string_target_id{};
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

            explicit Object(State state, DataStructure&& data_structure) : m_state{state}, m_data_structure{std::move(data_structure)}
            {
                //empty body
            }

        public:
            Object() = delete;

            std::optional<Object> instantiate()
            {
                DataStructure temporary_data_structure{"test",  "test2"};
                return Object(State::Detached, std::move(temporary_data_structure));
            }
            
    };

};





#endif