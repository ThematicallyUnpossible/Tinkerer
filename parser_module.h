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
    std::string path_to_program{get_input<std::string>("Enter program path : ")};


    int fd = open(path_to_program.c_str(), O_RDONLY);
    if(fd < 0)
    {
        std::cerr << "UNABLE TO OPEN HANDLE TO PROGRAM : " << path_to_program << "\n";
        return;
    }

    struct stat st;
    fstat(fd, &st);

    void* map = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(map == MAP_FAILED)
    {
        std::cerr << "UNABLE TO MAP A REGION " << "\n";
        return;
    }

    auto* header = static_cast<Elf64_Ehdr*>(map);

    if(
        header->e_ident[EI_MAG0] != ELFMAG0 ||
        header->e_ident[EI_MAG1] != ELFMAG1 ||
        header->e_ident[EI_MAG2] != ELFMAG2 ||
        header->e_ident[EI_MAG3] != ELFMAG3
    )
    {
        munmap(map, st.st_size);
        std::cout << "NOT A VALID ELF64 PROGRAM" << "\n";
        return;
    }

    std::cout << "VALID ELF64!" << "\n";

    auto* section_header = reinterpret_cast<Elf64_Shdr*>(static_cast<char*>(map) + header->e_shoff);

    const auto& shstrtab = section_header[header->e_shstrndx];
    const char* shstrtab_hdr = static_cast<char*>(map) + shstrtab.sh_offset;
    
    for(int i{0}; i < header->e_shnum; i++)
    {
        std::cout << shstrtab_hdr + section_header[i].sh_name << "\n";
    }




}


#endif