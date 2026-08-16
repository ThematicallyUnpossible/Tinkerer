#include <iostream>
#include "io_helper.h"
#include "ptrace_module.h"

int main(int, char**){
    std::optional<PtraceModule::Object> test = PtraceModule::Object::instantiate("Discord");
    std::cout << test->peek_data().m_string_target_pid;
    return 0;  
}
