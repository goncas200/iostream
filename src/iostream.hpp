/*
    Custom Iostream Gonçalo João
    Run on linux kernel
    Copyright (c) 2026 Gonçalo João. All rights reserved.
*/

#pragma once

#ifndef sizetype
#define sizetype unsigned long int // 8 bytes unsigned integer (used with movq)
#endif

#ifndef MAGICNUM
#define MAGICNUM 0x676f6e63616c6f69ULL
#endif

#ifndef MIN_INT
#define MIN_INT -2147483648
#endif


void* malloc(sizetype size) {
    void* ptr = nullptr;
    sizetype new_size = size + 16;
    __asm__ volatile (
        "movq $9, %%rax \n\t"
        "movq $0, %%rdi \n\t"
        "movq %1, %%rsi \n\t"
        "movq $3, %%rdx \n\t"
        "movq $0x22, %%r10 \n\t"
        "movq $-1, %%r8 \n\t"
        "movq $0, %%r9 \n\t"
        "syscall \n\t"
        "movq %%rax, %0 \n\t"
        : "=r"(ptr)
        : "r" (new_size)
        : "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "memory"
    );
    ((sizetype*)ptr)[0] = MAGICNUM;
    ((sizetype*)ptr)[1] = new_size;
    return (void*)(((sizetype*)ptr) + 2);
}

void free(void* ptr) {
    if (ptr == nullptr)
        return;
    sizetype* st_ptr = (sizetype*) ptr;
    st_ptr -= 2;
    if (st_ptr[0] != MAGICNUM)
        return;
    __asm__ volatile (
        "movq $11, %%rax \n\t"
        "movq %0, %%rdi \n\t"
        "movq %1, %%rsi \n\t"
        "syscall \n\t"
        :
        : "r" (st_ptr), "r" (st_ptr[1])
        : "rax", "rdi", "rsi", "rcx", "r11", "memory"
    );
}

class ostream {
    sizetype print(const char* str) {
        sizetype size = 0;
        while (str[size] != '\0') { // Find string size
            ++size;
        }
        sizetype bytes_written = 0;
        __asm__ volatile (
            "movq $1, %%rax \n\t" // mov 1 to rax for syscall_write
            "movq $1, %%rdi \n\t" // mov 1 to rdi for fd stdout
            "movq %1, %%rsi \n\t" // mov const char* str to rsi
            "movq %2, %%rdx \n\t" // mov unsigned long long int (size) to rdx
            "syscall \n\t"        // Sys call calls the syscall_write function
            "movq %%rax, %0 \n\t" // move bytes_written from rax to sizetype bytes_written
            : "=r" (bytes_written) // stored in registors
            : "r" (str), "r" (size) // Stored in registors
            : "rax", "rdi", "rsi", "rdx", "rcx", "r11", "memory" //Clobbers
        );
        return bytes_written;
    }
public:
    friend ostream& operator<<(ostream& cout, const char* str) { // Just runs the function 
        cout.print(str);
        return cout;
    }
    friend ostream& operator<<(ostream& cout, int str) { // Turns int to str
        if (str == MIN_INT) {
            cout.print("-2147483648");
            return cout;
        }
        if (str == 0) {
            cout.print("0");
            return cout;
        }
        bool is_negative = false;
        if (str < 0) {
            is_negative = true;
            str = str - str - str; //Turns it positive
        }
        int final_int[13]; // int array to store the individual numbers
        char final_str[13]; // Final string to put in print function
        int i = 0;
        while ((str / 10.0) > 0) { // Checks if there's still numbers to turn into chars, when 0 its done
            final_int[i++] = (str % 10); // The module is the number for instance 111 % 10 = 1
            str = str / 10; // Removes the last number because its already put in array for instance 111 / 10 = 11, because is stored as int
        }
        int aa = 0; // New intinerator
        if (is_negative) {
            final_str[aa++] = '-';
        }
        for (int a = i - 1; a >= 0; --a) {
            final_str[aa++] = final_int[a] + '0'; // Turns the array arround while turning the numbers into chars
        }
        if (!is_negative)
            final_str[i] = '\0'; // Closes the array
        else
            final_str[i+1] = '\0';
        cout.print(final_str); // syscall
        return cout; // returns ostream& for formatting
    }
};
namespace std   {
    ostream cout;
}


void input(const char* str , char* buffer, sizetype buffer_size) {
    std::cout << str;
    static char userinput[4001];
    static char trash[301];
    sizetype bytes_read = 0;
    __asm__ volatile (
        "movq $0, %%rax \n\t"
        "movq $0, %%rdi \n\t"
        "movq %1, %%rsi \n\t"
        "movq $4000, %%rdx \n\t"
        "syscall \n\t"
        "movq %%rax, %0 \n\t"
        : "=r" (bytes_read)
        : "r" (userinput)
        : "rax", "rdi", "rsi", "rdx", "rcx", "r11", "memory"
    );
    if (bytes_read > 0) {
        if (userinput[bytes_read-1] == '\n') {
            userinput[bytes_read-1] = '\0';;
        }
        else {
            userinput[bytes_read] = '\0';
            __asm__ volatile (
                "movq $0, %%rax \n\t"
                "movq $0, %%rdi \n\t"
                "movq %1, %%rsi \n\t"
                "movq $300, %%rdx \n\t"
                "syscall \n\t"
                "movq %%rax, %0 \n\t"
                : "=r" (bytes_read)
                : "r" (trash)
                : "rax", "rdi", "rsi", "rdx", "rcx", "r11", "memory"
            );
            while (trash[bytes_read-1] != '\n' && bytes_read > 0) {
            __asm__ volatile (
                "movq $0, %%rax \n\t"
                "movq $0, %%rdi \n\t"
                "movq %1, %%rsi \n\t"
                "movq $300, %%rdx \n\t"
                "syscall \n\t"
                "movq %%rax, %0 \n\t"
                : "=r" (bytes_read)
                : "r" (trash)
                : "rax", "rdi", "rsi", "rdx", "rcx", "r11", "memory"
            );
            }
        }
    }
    int i = 0;
    for (; i < buffer_size-1 && i < bytes_read; ++i) {
        buffer[i] = userinput[i];
    }
    buffer[i] = '\0';
}


void* operator new(sizetype size) noexcept {
    return malloc(size);
}

void* operator new[](sizetype size) noexcept {
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

// Junta estas duas:
void operator delete(void* ptr, sizetype size) noexcept {
    (void)size;
    free(ptr);
}

void operator delete[](void* ptr, sizetype size) noexcept {
    (void)size;
    free(ptr);
}