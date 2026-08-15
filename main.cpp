#include <iostream>
#include "io_helper.h"
#include "ptrace_module.h"

int main(int, char**){

    // string
    auto str = get_input<std::string>("Enter a string: ");
    std::cout << "You entered: " << str << "\n\n";

    // int
    auto i = get_input<int>("Enter an int: ");
    std::cout << "You entered: " << i << "\n\n";

    // int with limits
    auto limited_i = get_input<int>("Enter an int [1-100]: ", 1, 100);
    std::cout << "You entered: " << limited_i << "\n\n";

    // double
    auto d = get_input<double>("Enter a double: ");
    std::cout << "You entered: " << d << "\n\n";

    // double with limits
    auto limited_d = get_input<double>("Enter a double [0.0-10.0]: ", 0.0, 10.0);
    std::cout << "You entered: " << limited_d << "\n\n";

    std::optional<PtraceModule::Object> test_object = PtraceModule::Object::instantiate();

    return 0;
}
