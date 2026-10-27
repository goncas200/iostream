/*
    main.cpp Gonçalo João
    Custom iostream
*/

#include "iostream.hpp"

[[noreturn]] inline void sys_exit(sizetype code) {
    __asm__ volatile(
        "movq $60, %%rax \n\t"
        "movq %0, %%rdi \n\t"
        "syscall \n\t"
        :
        : "r" (code)
        : "rax", "rdi", "rcx", "r11", "memory"
    );
    while(true);
}

extern "C" void _start() {
    const char* a = input("Hello: ");
    std::cout << a << "\n";
    const char* b = input("Hello: ");
    std::cout << b << "\n";
    std::cout << "hh\n";
    sys_exit(0);
}