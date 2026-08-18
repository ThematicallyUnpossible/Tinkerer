#include <iostream>
#include "io_helper.h"
#include "ptrace_module.h"

int main(int argc, char* argv[]){

    if(argc != 2)
    {
        std::cerr << "Invalid usage. Expected : Tinker <program_name>" << "\n";
        return 1;
    }

    std::optional<PtraceModule::Object> PtraceModuleObject = PtraceModule::Object::instantiate(argv[1]);
    if(!PtraceModuleObject){
        std::cerr << "Unable to construct PtraceModule object."  << "\n";
    }

    std::cout << PtraceModuleObject->peek_data().m_string_target_base << "\n";


    return 0;  
}
