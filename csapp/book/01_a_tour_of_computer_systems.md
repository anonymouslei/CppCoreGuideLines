# a tour of computer systems
1. you are going to learn practical skills such as how to avoid strange numerical errors caused by teh way that computers represent numbers.
2. you will learn how to optimize your C code by ysing clever tricks that exploit the designs of modern processors and memory systems.
3. you will learn how the compiler implements procedure calls and how to use this knowledge to avoid the security holes from buffer overflow vulnerabilities that plague network and Internet software.
4. you will learn how to recognize and avoid the nasty errors during linking that confound the average programmer.
5. you will learn how to write your own unix shell, your own dynamic storage allocation package, and even your own Web server.
6. you will learn the promises and pitfalls of concurrency, a topic of increasing importance as multiple processor cores are integrated onto single chips.
## 1.1 information is bits + context
## 1.2 Programs are translated by other programs into different forms
In oder to run `hello.c` on the system, the individual C statements must be translated by other programs into a sequence of low-level machine-language instructions.
These instructions are then packaged in a form called an executable object program and stored as a binary disk file.
Object programs are also referred to as executable object files.

```bash
gcc -o hello hello.c
```
![compilation_system](../pic/compilation_system.png)
four phases: preprocessor, compiler, assembler, and linker
- Preprocessing phase.
the preprocessor(cpp) modifies the original C program according to directives that begin with the "#" character. 
For example, the `#include <stdio.h>` command in line 1 of `hello.c` tells the preprocessor to read the contents of the system header file `stdio.h` and insert it directly into the program text. 
The result is another C program, typically with the .i suffix.
- compilation phase.
The compiler(cc1) translates teh text file `hello.i` into the text file `hello.s`, which contains an assemble-language program.
this program includes the following definition of function `main`:
```
main:
    subq $8, %rsp
    movl $.LCO, %edi
    call puts
    movl $0, %eax
    addq $8, %rsp
    ret
```
Assembly language is useful because it provides a common output language for different compilers for different high-level languages.
- assemble phase
Next, the assembler translates `hello.s` into machine-language instructions, packages them in a form known as a `relocatable object program`, and stores the result in the object file `hello.o`. 
this file is a binary file containing 17 bytes to encode the instructions form function `main`. 
- linking phase
notice that our `hello` program calls the `printf` function, which is part of the standard C library provided by every C compiler.
The printf function resides in a separate precompiled object file called `printf.o`, which must somehow be merged with our `hello.o` program.
The linker(ld) handles this merging.
The result is the `hello` file, which is an executable object file that is ready to be loaded into memory and executed by the system.

## 1.3 it pays to understand how compilation system work

## 1.4 Processors Read and Interpret Instructions Stored in Memory
The register file is a small storage device that consists of a collection of word-size registers, 
each with its own unique name.
The ALU computes new data and address values.
- load: copy a byte or a word from main memory into a register, overwriting the previous contents of the register
- Store: copy a byte of a word from a register to a location in main memory, overwriting the previous contents of that location
- Operate: copy the contents of tow registers to the ALU, perform an arithmetic operation on the two words, and store the result in a register, overwriting the previous contents of that register.
- Jump: extract a word from the instruction itself and copy that word into the program counter, overwriting the previous value of the PC
  
## 1.5 Caches Matters
## 1.6 Storage Devices Form a Hierarchy
## 1.7 The operating system manages the Hardware
we can think of the operating system as a layer of software interposed between the application program and the hardware.
All attempts by an application program to manipulate the hardware must go through the operating system.

The operating system has two primary purpose:
    1. to protect the hardware from misuse by runaway applications
    2. to provide applications with simple und uniform mechanisms for manipulating complicated and often wildly different low-level hardware devices.

### 1.7.1 processes
when a program such as hello runs on a modern system, the operating system provides the illusion that the program is only one running on the system.
The program appears to have exclusive use of both the processor, main memory, and I/O devices.
The processor appears to execute the instructions in the program, one after other, without interruption.
By concurrently, we mean that the instructions of one process are interleaved with the instructions of another process.

The transition from one process to another is managed by the operating system kernel.
The kernel is the portion of the operating system code that is always resident in memory.
When an application program requires some action by the operating system, such as to read or write a file, 
it executes a special system call instruction,
transferring control to the kernel.
The kernel then performs the requested operation and returns back to the application program.
Note that the kernel is not separate process. Instead, it is a collection of code and data structures that the system uses to manage all the progresses.

### 1.7,2
### 1.7.3 Virtual memory
Virtual memory is an abstraction that provides each process with the illysion that it has exclusive use of the main memory.
Each process has the same uniform view of memory, which is known as its virtual address space.

In Linux, the topmost region of the address space is reserved for code data in the operating system that is common to all processes.

The lower region of the address space holds the code and data defined by the user's process.
The virtual address space seen by each process consists of a number of well-defined areas, each with a specific purpose.
- Program code and data. Code begins at the same fixed address for all processes, followed by data locations that correspond to global C variables. 
  The code and data areas are initialized directly from the contents of an executable object file.
- heap: The code and data areas are followed immediately by the run-time heap. The heap expands and contracts dynamically at run time as a result of calls to C standard library routines such as malloc and free.
- shared libraries. Near the middle of the address space is an area that holds the code and data form shared libraries such as the C standard library and the math library. 
- stack. at the top of the user's virtual address space is the user stack that the compiler uses to implement function calls.Like the heap, the user stack expands and contracts dynamically during the execution of the program.
  In particular, each time we call a function, the stack grows. each time we return from a function, it contracts.
- kernel virtual memory. The top region of the address space is reserved for the kernel.
  Application programs are not allowed to read or write the contents of this area or to directly call functions defined in the kernel code. Instead, they must invoke the kernel to perform these operations.

