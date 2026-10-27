/*
    main.cpp Gonçalo João
    Custom iostream
    Copyright (c) 2026 Gonçalo João. All rights reserved.

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
    char* a = new char[20];
    delete[] a;
    std::cout << -2147483647 << "\n";
    sys_exit(0);
}