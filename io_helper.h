#ifndef io_helper
#define io_helper

#include <iostream>
#include <limits>
#include <string_view>
#include <limits.h>

inline void clear_input()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template<typename T>
    inline T get_input(std::string_view prefix)
    {
        std::cout << prefix;

        T local_buffer{};
        while(true)
        {
            std::cin >> local_buffer;
            if(std::cin.fail())
            {
                clear_input();
                std::cout << prefix;
                continue;
            }
            else
            {
                return local_buffer;
            }
        }
    }

template <typename T>
    inline T get_input(std::string_view prefix, T min, T max)
    {
        std::cout << prefix;

        T local_buffer{};
        while(true)
        {
            std::cin >> local_buffer;
            if(std::cin.fail())
            {
                clear_input();
                std::cout << prefix;
                continue;
            }
            else if(local_buffer < min || local_buffer > max)
            {
                local_buffer = {};
                std::cout << prefix;
                continue;
            }
            else
            {
                return local_buffer;
            }
        }
    }

template<>
    inline std::string get_input<std::string>(std::string_view prefix)
    {
        std::cout << prefix;

        std::string local_buffer{};
        std::getline(std::cin, local_buffer);

        return std::move(local_buffer);
    }


#endif