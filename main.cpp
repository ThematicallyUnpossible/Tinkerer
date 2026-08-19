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
    if(!PtraceModuleObject)
    {
        std::cerr << "Unable to construct PtraceModule object."  << "\n";
    }

    std::cout << "Program Entry : "<< PtraceModuleObject->peek_data().m_string_target_base << "\n";
    std::cout << "Program Libc Base  : "<< PtraceModuleObject->peek_data().m_string_target_libc_base << "\n";

    PtraceModule::LibMeta lib_fcnhook{};
    lib_fcnhook.m_string_lib_name = get_input<std::string>("Enter lib name : ");
    lib_fcnhook.m_string_lib_path = get_input<std::string>("Enter lib path : ");

    std::cout << "Lib Name : " << lib_fcnhook.m_string_lib_name << "\n";
    std::cout << "Lib Path : " << lib_fcnhook.m_string_lib_path << "\n";

    PtraceModuleObject->load_library(std::move(lib_fcnhook));


    return 0;  
}
