
# Machine-level Representation of Programs
the gcc C compiler generates its output in the form of assembly code, a textual representation of the machine code giving the individual instructions in the program.
Gcc then invokes both an assembler and a linker to generate the executable machine code from the assembly code.
programmers seeking to maximize the performance of a critical section of code often try different variations of the source code, each time compiling and examining the generated assembly code to get a sense of how efficiently teh program will run.

there are times when the layer of abstraction provided by a high-level language hides information about the run-time behavior of a program that we need to understand.

for example, when writing concurrent programs using a thread package, it is important to understand how program data are shared or kept private by the different threads and precisely how and where shared data are accessed.

unterstanding how these vulneerabilities arise and how to guard against them requires a knowledeg of the machine-level representation of programs.

relative to the computations expressed in the C code, optimizing compilers can rearrange execution order, eliminate unneeded computation, replace slow operations with faster ones, and even change recursive computations into iterative ones.

## 3.1 A historical perspective

## 3.2 Program Encodings
```bash
gcc -Og -o p p1.c p2.c
```
The command-line option -Og instructs the compiler to apply a level of optimization that yields machine code that follows the overall structure of the original C code.
Invoking higher levels of optimization can generate code that is so heavily transformed that the relationship between the generate code that is so heavily transformed that the relationship between the generated machine code and the original source code is difficult to understand.
In practice, higher levels of optimization are considered a better choice in terms of the resulting program performance.

the gcc command invokes an entire sequence of programs to turn the source code onto executable code.
- the C preprocessor expands the source code to include any files specified with `#define` declarations.
- the compiler generates assemblycode versions of the two source files having names p1.s and p2.s
- the assembler converts the assembly code into binary object-code files p1.o and p2.o

    object code is one form of machine code - it contains binary representations of all of the instructions, but the addresses of global values are not yet filled in.
- finally, the linker merges these two object-code files along with code implementing library functions and generates the final executalbe code file p(as specified by the command-line).
- executable code is the second form of machine code we will consider - it is the exact form of code that is executed by the processor.

### 3.2.1 Machine-level code
computer systems employ several different forms of abstraction, hiding details of an implementation through the use of a simpler abstract model.

- the format and behavior of a machine-level program is defined by the instruction set architecture, defining the processor state, the format of the instructions, and the effect each of these instructions will have on the state.
    most ISAs, describe the behavior of a program as if each instruction is executed in sequence, with one instruction completing before the next one begins.
    the processor hardware is far more elaborate, executing many instructions concurrently, but it employs safeguards to ensure that the overall behavior matches the sequential operation dictated by the ISA.
- second, the memory addresses used by a machine-level program are virtual addresses, providing a memory model that appears to be a very large byte array.
the actual implementation of the memory system involves a bombination of multiple hardware memories and operating system software.
the compiler does most of the work in the overall compilation sequence, transforming programs expressed in the relatively abstract execution model provided by C into the very elementary instructions that the processor executes.

whereas C provides a model in which objects of different data types can be declared and allocated in memory, machine code views the 

Aggregate data types in C such as arrays and structures are represented in machine code as contiguous collections of bytes.
even for scalar data types, assembly cod emakes no distinctions between signed or unsigned integers, between different types of pointers, or even between pointers and integers.

### 3.2.2 Code examples
```C
long mult2(long, long);

void multstore(long x, long y, long *dest) {
    long t = mult2(x, y);
    *dest = t;
}
```

```bash
gcc -Og -S mstore.c
```
```
multstore:
    pushq %rbx
    movq %rdx, %rbx
    call mult2
    movq %rax, (%rbx)
    popq %rbx
    ret
```
Each indented line in the code corresponds to a single machine instruction.
if we use the -c command-line option, gcc will both compiler and assemble the code
```
gcc -Og -c mstore.c
```
this will generate an object-code file mstore.o that is in binary format and hence cannot be viewed directly.

a key lesson to learn from this is that the program executed by the machine is simply a sequence of bytes encoding a series of instructions.

```
objdump -d mstore.o
```
several features about machine code and its disassembled representation are worth noting
- x86-64 instructions can range in length from 1 to 15 bytes. the instruction encoding is designed so that commonly used instructions and those with fewer operands require a smaller number of bytes than do less common ones or ones with more operands.
- the instruction format is designed in such a way that from a given starting position, there is a unique decoding of the bytes into machine instructions.
- The disassembler determines the assembly code based purely on the byte sequences in the machine-code file. it does not require access to the access to the source or assembly-code versions of the program.
- the disassembler uses a slightly different naming convention for the instructions than does the assembly code generated by gcc.

#### how do I display the byte representation of a program?
to display the binary object code for a program, we use a disassembler to determine that the code for the procedure is 14 bytes long. 
then we run the GNU debugging tool GDB on file mstore.o and give it the command
```
(gdb) x/14xb multstore
```
telling it to display (abbreviated 'x') 14 hex-formatted (also 'x') bytes ('b') starting at the address where function multstore is located.

```c
#include <stdio.h>

void multstore(long, long, long *);

int main() {
    long d;
    multstore(2, 3, &d);
    printf("2 * 3 --> %ld\n", d);
    return 0;
}
long mult2(long a, long b) {
    long s = a * b;
    return s;
}
```
```shell
gcc -Og -o prog main.c mstore.c
```

### 3.2.3 Notes on Formatting
all of the lines beginning with '.' are directives to guide the assembler and linker.
we can generally ignore these.
On the other hand, there are no explanatory remarks about what the instructions do or how they relate to the source code.

For some applications, the programmer must drop down to assembly code to access low-level features of the machine.
One approach is to write entire functions in assembly code and combine them with C functions during the linking stage.

#### Combining assembly code with C programs
Although a C Compiler does a good job of converting the computations expressed in a program into machine code, 
there are some features of a machine that cannot be accessed by a C program.

For example, every time an x86-64 processor executes an arithmetic or logical operation, it sets a 1-bit condition code flag, named PF, to 1 when the lower 8 bits in the resulting computation have an even number of ones and to 0 otherwise.

there are two ways to incorporate assembly code into C programs.
- we can write an entire function as a separate assembly-code file and let the assembler and linker combine this with code we have written in C.
- second, we can use the inline assembly feature of GCC, where brief sections of assembly code can be incorporated into a C program using the asm directive.
  this approach has the advantage that it minimizes the amount of machine-specific code.


## 3.3 Data Formats
