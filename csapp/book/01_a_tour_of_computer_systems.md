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