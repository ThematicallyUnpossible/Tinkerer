#include <iostream>
#include "ptrace_module.h"
#include "trigger_module.h"

void run_injector(char* argv)
{
    std::optional<PtraceModule::Object> PtraceModuleObject = PtraceModule::Object::instantiate(argv);
    if(!PtraceModuleObject)
    {
        std::cerr << "Unable to construct PtraceModule object."  << "\n";
    }

    std::cout << "Program PID : " << PtraceModuleObject->peek_data().m_string_target_pid << "\n";
    std::cout << "Program Entry : "<< PtraceModuleObject->peek_data().m_string_target_base << "\n";
    std::cout << "Program Libc Base  : "<< PtraceModuleObject->peek_data().m_string_target_libc_base << "\n";

    if(!PtraceModuleObject->queue_loadable())
    {
        std::cout << "Failed to queue library." << "\n";
    }

    bool inject_result = PtraceModuleObject->inject_loadable();
    trigger_hook_fcn(PtraceModuleObject->peek_data().m_string_target_pid);

}

int main(int argc, char* argv[]){

    if(argc != 2)
    {
        std::cerr << "Invalid usage. Expected : Tinker <program_name>" << "\n";
        return 1;
    }

    run_injector(argv[1]);
    
    

    return 0;  
}

