#include <iostream>
#include "io_helper.h"
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

    std::string table_of_choices
    {
        "1. Create loadable \n"
        "2. Load existing \n"
        "3. Trigger hook function\n"
        "4. Exit\n"
        "Enter your choice : "
    };
    int choice{3};

    while(true)
    {
        choice = get_input<int>(table_of_choices, 1, 4);
        if(choice == 1)
        {
            clear_input();
            if(!PtraceModuleObject->queue_loadable())
            {
                std::cout << "Failed to queue library." << "\n";
                continue;
            }
        }
        else if(choice == 2)
        {
            bool inject_result = PtraceModuleObject->inject_loadable();
        }
        else if (choice == 3)
        {
            trigger_hook_fcn(PtraceModuleObject->peek_data().m_string_target_pid);
        }
        else if (choice == 4)
        {
            return;
        }
    }

    


}

int main(int argc, char* argv[]){

    if(argc != 2)
    {
        std::cerr << "Invalid usage. Expected : Tinker <program_name>" << "\n";
        return 1;
    }

    std::string table_of_choices {
    "1. Inject & hook\n"
    "Enter your choice : "
    };
    int choice = get_input<int>(table_of_choices, 1, 1);
    clear_input();
    if(choice == 1)
    {
        run_injector(argv[1]);
    }
    
    

    return 0;  
}

