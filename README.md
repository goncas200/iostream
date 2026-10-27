# Freestanding C++ Linux Kernel Library & Custom iostream

A bare-metal, low-level custom C++ library designed to run directly on top of the **Linux Kernel (x86_64)**. This project operates entirely in a **freestanding environment**, bypassing the standard GNU C Library (`glibc`) and traditional runtime startup files (`crt0`) by communicating directly with the hardware via inline assembly.

## Features

- **Zero-glibc Dependency**: Employs raw inline assembly (`__asm__ volatile`) to invoke Linux software interrupts directly.
- **Custom Entry Point**: Operates without a `main()` function, implementing a pure `_start` linkage symbol.
- **Custom Memory Allocator**: Overrides global operators `new`, `delete`, `new[]`, and `delete[]` using a custom `malloc`/`free` layer built on top of `sys_mmap` (9) and `sys_munmap` (11). Includes a strict `MAGICNUM` metadata structural validation integrity check.
- **Custom Output Stream (`std::cout`)**: A lightweight `ostream` implementation featuring native string writing and optimized integer-to-string conversion mechanics using `sys_write` (1).
- **Safe Buffered Input**: An interactive `input()` routine utilizing `sys_read` (0) that automatically flushes and handles standard input overflows cleanly.
- **System Termination**: A clean `sys_exit` implementation invoking `sys_exit` (60) to avoid hardware segmentation faults on program completion.

## System Call Reference Mapping

| Operation | Service | Syscall ID (RAX) | Key Registers / Description |
| :--- | :--- | :---: | :--- |
| `malloc` | `sys_mmap` | `9` | `RDI` (0), `RSI` (size), `RDX` (PROT_READ\|PROT_WRITE), `R10` (MAP_ANONYMOUS\|MAP_PRIVATE) |
| `free` | `sys_munmap` | `11` | `RDI` (address), `RSI` (size) |
| `print` | `sys_write` | `1` | `RDI` (1 = stdout), `RSI` (char pointer), `RDX` (string length) |
| `input` | `sys_read` | `0` | `RDI` (0 = stdin), `RSI` (buffer destination), `RDX` (max bytes) |
| `sys_exit`| `sys_exit` | `60` | `RDI` (exit error code status) |

## Project Architecture

- `src/`
  - `iostream.hpp`: Primary library component comprising system calls, operators, memory allocators, and stream classes.
  - `main.cpp`: Freestanding application entry point utilizing the customized runtime environment.
- `CMakeLists.txt`: Build automation script configured for strict freestanding compilation parameters.
- `.gitignore`: Configured to isolate build footprints.

## Build and Compilation Specifications

The compilation pipeline is tailored via custom CMake rules to remove all standard library attachments, optimization overheads, and unnecessary metadata tables (`-nostdlib`, `-ffreestanding`, `-fno-exceptions`, `-fno-rtti`).

### Prerequisites
- **Target OS**: Linux
- **Architecture**: x86_64
- **Tools**: CMake (3.10.0+) and a C++ compiler (`g++` or `clang++`).

### Building via Terminal

```bash
# 1. Create and enter a compilation directory
mkdir build && cd build

# 2. Configure the project binaries via CMake
cmake ..

# 3. Compile the target codebase
cmake --build .
```

### Execution
Execute the fully decoupled, freestanding binary:
```bash
./iostream
```

## Implementation Snippets

### Application Entry Point (`src/main.cpp`)
```cpp
/*
    main.cpp Gonçalo João
    Custom iostream
    Copyright (c) 2026 Gonçalo João. All rights reserved.
*/

#include "iostream.hpp"

extern "C" void _start() {
    std::cout << "Hello, World!\n";
    sys_exit(0);
}
```

### Custom Allocator Architecture
Every memory chunk assigned through `new` prefixes a 16-byte metadata block to monitor allocation scope and buffer health:
- **Bytes 0–7**: Verification constant signature (`0x676f6e63616c6f69`).
- **Bytes 8–15**: Size footprint calculation sequence of the chunk.

```cpp
((sizetype*)ptr) = MAGICNUM;
((sizetype*)ptr) = new_size;
return (void*)(((sizetype*)ptr) + 2); // Shift pointer forward to user space
```

## License

Copyright © 2026 Gonçalo João.

This project is licensed under the **GNU General Public License v3.0** - see the [LICENSE](LICENSE) file for the full license text.
