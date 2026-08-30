cmake_minimum_required(VERSION 3.10.0)
project(Tinkerer VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(Tinkerer main.cpp
io_helper.h
ptrace_module.h
ptrace_module.cpp
trigger_module.h
utility.h
)

