#ifndef ptrace_module
#define ptrace_module

#include <string>
#include <optional>
#include <vector>

namespace PtraceModule
{
    struct TargetMetadata
    {
        std::string m_string_target_name{};
        std::string m_string_target_pid{};

        std::string m_string_target_base{};
        std::string m_string_target_libc_base{};
    };

    struct LibMeta
    {
        std::string m_string_lib_name{};
        std::string m_string_lib_path{};
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
            TargetMetadata m_target_metadata{};
            std::vector<LibMeta> m_loadable_list{};

            explicit Object(TargetMetadata&& data_structure) : 
                m_target_metadata{std::move(data_structure)}
            {
                //empty body
            }


        public:
            Object() = delete;

            static std::optional<Object> instantiate(const std::string& target_process_name);
            bool queue_loadable();
            const TargetMetadata& peek_data() const;


    };

};




#endif