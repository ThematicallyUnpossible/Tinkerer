
#include "ptrace_module.h"
#include <iostream>
#include <fstream>

#ifndef utility
#define utility

    inline std::optional<std::string> string_find_base(std::string look_inside_pid, const std::string& string_lookup_name)
    {
        std::string string_path = "/proc/" + look_inside_pid + "/maps";
        std::ifstream ifstream_path(string_path);

        std::string current_page{};
        while(std::getline(ifstream_path, current_page))
        {
            if(current_page.find(string_lookup_name) != std::string::npos)
            {
                return current_page.substr(0, current_page.find('-'));
            }
        }
        
        return std::nullopt;
    }

    inline void DEBUG_PRINT_LOADABLE_LIST(std::vector<PtraceModule::LibMeta>& list)
    {
        if(list.empty())
        {
            std::cerr << "(DEBUG) EMPTY QUEUE LIST" << "\n";
            return;
        }
        int i =  0;
        for(const auto& x : list)
        {
            std::cout << "Queued " << i << " | name : " << x.m_string_lib_name << " | path : " << x.m_string_lib_path << " | " << "\n";
            i++;
        }
        return;
    }

#endif