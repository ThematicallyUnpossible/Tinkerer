#include <iostream>
#include "io_helper.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <elf.h>

#ifndef parser_module
#define parser_module


inline void parse_elf64()
{
    std::string test = get_input<std::string>("Enter elf program path : ");
    int fd = open(test.c_str(), O_RDONLY);
    if(fd < 0)
    {
        std::cerr << "UNABLE TO OPEN FILE!" << "\n";
        return;
    }

    struct stat st;
    fstat(fd, &st);

    void* map = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    if(map == MAP_FAILED)
    {
        std::cerr << "UNABLE TO MAP!" << "\n";
    }

    auto* header = static_cast<const Elf64_Ehdr*>(map);

    if (header->e_ident[EI_MAG0] != ELFMAG0 ||
        header->e_ident[EI_MAG1] != ELFMAG1 ||
        header->e_ident[EI_MAG2] != ELFMAG2 ||
        header->e_ident[EI_MAG3] != ELFMAG3) {
        std::cerr << "NOT A VALID ELF!\n";
        munmap(map, st.st_size);
        return;
    }
}


#endif