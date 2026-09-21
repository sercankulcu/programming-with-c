cmake_minimum_required(VERSION 3.16)
project(intarray C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(ENABLE_ASAN "Build with AddressSanitizer" OFF)

add_library(intarray intarray.c)
target_include_directories(intarray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(intarray PRIVATE -Wall -Wextra -Wpedantic)

add_executable(app main.c)
target_link_libraries(app PRIVATE intarray)

if(ENABLE_ASAN)
    # the flag has to reach both compiling and linking
    add_compile_options(-fsanitize=address,undefined -g)
    add_link_options(-fsanitize=address,undefined)
endif()
