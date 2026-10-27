cmake_minimum_required(VERSION 3.10.0)
project(iostream VERSION 0.1.0 LANGUAGES C CXX)

add_compile_options(
    -O3
    -fno-exceptions
    -fno-rtti
    -ffreestanding
    -fno-asynchronous-unwind-tables
    -fno-ident
)

add_executable(iostream src/main.cpp)

target_link_options(iostream PRIVATE
    -nostdlib       
    -e _start       
    -s              
    -Wl,--gc-sections  
)
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
