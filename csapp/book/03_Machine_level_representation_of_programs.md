
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

## 3.4 Accessing Information
An x86-64 central processing unit (CPU) contains a set of 16 general-purpose registers storing 64-bit values. 
these registers are used to store integer data as well as pointers. 
their names all begin with `%r`, but otherwise follow multiple different naming conventions, owing to the historical evolution of the instruction set. 

the original 8086 had eight 16-bit registers, shown as registers `%ax` through `%bp`.
each had a specific purpose, and hence they were given names that reflected how they were to be used.
With the extension to IA32, these registers were expanded to 32-bit registers, labeled `%eax` through `%ebp`. In addition, eight new registers were added, and these were given labels according to a new naming convention: `%r8` through `%r15`.
instructions can operate on data of different sizes stored in the low-order bytes of the 16 registers. 
Byte-level operations can access the least significant type, 
16-bit operations can access the least significant 2 bytes, 32-bit operations can access the least significant 4 bytes,
and 64-bit operations can access entire registers.

we will present a number of instructions for copying and generating 1-, 2-, 4- and 8-byte values. 
When these instructions have registers as destinations, two conventions arise for what happens to the remaining bytes in the register for instructions that generate less than 8 bytes: those that generate 1 or 2 byte quantities leave the remaining bytes unchanged. 
those that generate 4 byte quantities set the upper 4 bytes of the register to zero.

a set of standard programming conventions governs how the registers are to be used for managing the stack, passing function arguments, returning values from functions, and storing local and temporary data.

### 3.4.1 operand specifiers
source values can be given as constants or read from registers or memory.
Results can be stored in either registers or memory.
Thus, the different operand possibilities can be classified into three types.
the primary difference between register and memory is that register holds the data that the CPU is currently processing whereas, the memory holds the data the that will b`, but otherwise follow multiple different naming conventions, owing to the historical evolution of the instruction set. 

the original 8086 had eight 16-bit registers, shown as registers `%ax` through `%bpe required for processing.
1. immediate: is for constant values. In ATT-format assembly code, these are written with a `$` followed by an integer using standard C notation. Different instructions allow different ranges of immediate values. the assembler will automatically select the most compact way of encoding a value.
2. register: denotes the contents of a register, one of the sixteen 8-, 4-, 2- or 1-byte low-order portions of the registers for operands having 64, 32, 16 or 8 bits. we use the notation r_a to denote an arbitrary register a and indicate its value with the reference R[ra], viewing the set of registers as an array R indexed by register identifiers.
3. memory reference, in which we access some memory location according to a computed address, often called the effective address. since we view the memory as a large array  of bytes, we use the notation Mb[Addr] to denote a reference to the b-byte value stored in memory starting at address Addr. To simplify things, we will generally drop the subscript b

The most general form is shown at the bottom of the table with syntax Imm(rb, ri, s). Such a reference has four components: an immediate offset 

this general form is often seen when referencing elements of arrays.
the other forms are simply special cases of this general form where some of the components are omitted.

### 3.4.2 Data Movement Instructions
Among the most heavily used instructions are those that copy data from one location to another. 
The generality of the operand notation allows a simple data movement instruction to express a range of possibilities that in many machines would require a number of different instructions.
We present a number of different data movement instructions, differing in their source and destination types,
what conversions they perform, and other side effects they may have.
In our presentation, we group the many different instructions into instruction classes, where the instructions in a class perform the same operation but with different operand sizes.
these instructions copy data from a source location to a destination location, without any transformation.
the class consists of four instructions: `movb, movw, movl and movq`. 
all four of these instructions have similar effects: they differ primarily in that they operate on data of different sizes: 1,2, 4, 8 bytes, respectively.
the source operand designates a value that is immediate, stored in a register, or stored in memory.
The destination operand designates a location that is either a register or a memory address.
x86-64 imposes the restriction that a move instruction cannot have both operands refer to memroy locations. 
Copying a value from one memory location to another requires two instructions - the first to load the source value into a register, and the second to write this register value to the destination.
referring to Figure 3.2, register operands for these instructions can be the labeled portions of any of the 16 registers, where the size of the register must match the size designated by the last character fo the instruction.
the only exception is that when `movl` has a register as the destination, it will also set the high-order 4 bytes of the register to 0.
A final instruction documented is for dealing with 64-bit immediate data. 
The regular `movq` instruction can only have immediate source operands that can be represented as 32-bit.
they document two classes of data movement instructions for use when copying a smaller source value to a larger destination.
All of these instructions copy data from a source, which can be either a register or stored in memory, to a register destination.
instructions in the MOVZ class fill out the remaining bytes of the destination with zeros, while those in the MOVS class fill them out by sign extension, replicating copies of the most significant bit of the source operand.
Observe that each instruction name has size designator as tis final two characters - the first specifying the source size, and the second specifying the destination size.
as can be seen, there are three instructions in each of these classes, covering all cases of 1- and 2-byte source sizes and 2- and 4-byte destination sizes, considering only cases where the destination is larger than the source, of course.

Note the absence of an explicit instruction to zero-extend a 4-byte source value to an 8-byte destination.

PP3.2 S222 
as we have seen, the assembly code generated by gcc includes suffixes on the instructions, while the disassembler does not. 
Being able to switch between these two forms is an important skill to learn.
One important feature is that memory references in x86-64 are always given with quad word registers, such as %rax, even if the operand is a byte, single word, or double word.
这道题就是应该看source和des.如果source可以直接判断就看source, 如果source不能判断，就看des. 看register是什么。

Aside Comparing byte movement instructions
the following example illustrates how different data movement instructions either do or do not change the high-order bytes of the destination. Observe that the three byte-movement instructions `movb`, `movsbq` and `movzbq` differ from each other in subtle ways.
in the following discussion, we use hexadecimal notation for all of the values.
the first two lines of the code initialize registers %rax and %dl to `0011223344556677` and `AA`, respectively.
the remaining instructions all copy the low-order byte of %rdx to the low-order byte of %rax.
the `movb` instruction does not change the other bytes.
the `movsbq` instruction sets the other 7 bytes to either all ones or all zers depending on the high-order bit of the source byte.
Since hexadecimal A represents binary value 1010, sign extension causes the higher-order bytes to each be set to FF. the `movzbq` instruction always sets the other 7 bytes to zero.

PP3.3 S223
movb cannot use %ebx as address register
movl %rax不是 Mismatch between instruction suffix and register ID
movw 两个都是memory cannot have both source and destination be momery references
movb sl是啥？No register named %sl
movq imme不能作为des, cannot have immediate as destination
movl %rdx是 qw, Destination operand incorrect size
movb %si 不是b, Mismatch between instruction suffix and register ID

### 3.4.3 Data Movement Example
As an example of code that uses data movement instructions, consider the data exchange routine.
```C
long exchange(long *xp, long y)
{
    long x = *xp;
    *xp = y;
    return x;
}
```
```assembly code
long exchange(long *xp, long y)
xp in %rdi, y in %rsi
exchange:
movq (%rdi), %rax # get x at xp. Set as return value
movq %rsi, (%rdi) #store y at xp
ret # return
```
as Figure b shows, function exchange is implemented with just three instructions:
two data movements plus an instruction to return back to the point from which the function was called.
We will cover teh details of function call and return in Section 3.7.
Until then, it suffices to say that arguments are passed to functions in registers.
Our annotated assembly code documents theres.
A function returns a value by storing it in register %rax, or in one of the low-order portions of this register.

when the procedure begins execution, procedure parameters xp and y are stored in registers %rdi and %rsi, respectively.
instruction 2 then reads x from memory and stores the value in register %rax, a direct implementation of the operation x = *xp in the C program.
Later, register %rax will be used to return a value from the function, and so the return value will be x.
Instruction 3 writes y to the memory location designated by xp in register %rdi, a direct implementation of the operation *xp = y.
this example illustrates how the mov instructions can be used to read from memory to a register, and to write from a register to memory.

two features about this assembly code are worth noting. First we see that what we call "pointers" in C are simply addresses.
Dereferencing a pointer involves copying that pointer into a register, and then using this register in a memory reference.
Second, local variables such as x are often kept in registers rather than stored in memory locations.
Register access is much faster than memory access.

PP3.4 S224
the first instruction in the sequence should read from memory, do the appropriate conversion, and set the appropriate portion of register %rax.
the second instruction should then write the sppropriate portion of %rax to memory.
in both cases, the portions may be %rax, %eax, %ax or %al, and they may differ from one another.

|src_t|dest_t|Instruction
|--|--|--|
|char|int|movb(%rdi), %al movw%rax, (%rsi)

New to C? some examples of pointers
Function exchange provides a good illustration of the use of pointers in C. Argument xp is a pointer to a long integer, while y is a long integer itself. the statement 
`long x = *xp;` indicates that we should read the value stored in the location designated by xp and store it as a local variable named x. this read operation is know as pointer dereferencing. The C operator * performs pointer dereferencing.
the statement `*xp = y;` does the reverse - it writes the value of parameter y at the location designated by xp.
this is also a form of pointer dereferencing, but it indicates a write operation since it is on the left-hand side of the assignment.

the following is an example of exchange in action:
```C
long a = 4;
long b = exchange(&a, 3);
printf("a= %ld, b = %ld\verb@\@n", a, b);
```
PP3.5 S226
### 3.4.4 Pushing and Popping Stack Data
the final two data movement operations are used to push data onto and pop data from the program stack, as documented in Figure 3.8.
As we will see, the stack plays a vital role in the handling of procedure calls.
By way of background, a stack is a data structure where values can be added or deleted, but only according to a "last-in, first-out" discipline.
We add data to a stack via a push operation and remove it via a pop operation, with the property that the value popped will always be the value that was most recently pushed and is still on the stack.
A stack can be implemented as an array, where we always insert and remove elements from one end of the array. This end is called the top of the stack.
the program stack is stored in some region of memory.
As illustrated in Figure 3.9, the stack grows downward such that the top element of the stack has the lowest address of all stack elements.

the `pushq` instruction provides the ability to push data onto the stack, while the `popq` instruction pops it.
each of these instruction takes a single operand - the data source for pushing and the data destination for popping

pushing a quad word value onto the stack involves first decrementing the stack pointer by 8 and then writing the value at the new top-of-stack address.
therefor, the behavior of the instruction pushq %rbp is equivalent to that of the pair of instructions
```
subq $8, %rsp  Decrement stack pointer
movq %rbp, (%rsp)  Store %rbp on stack
```
except that the pushq instruction is encoded in the machine code as a single byte, whereas teh pair of instructions shown above requires a total of 8 bytes.

## 3.5 Arithmetic and Logical Operations
### 3.5.1 Load Effective Address
the load effective address instruction `leaq` is actually a variant of the movq instruction.
it has the form of an instruction that reads from memory to a register, but it does not reference memory at all.
its first operand appears to be a memory reference, but instead of reading from the designated location, the instruction copies the effective address to the destination.
This instruction can be used to generate pointer for later memory references.
In addition, it can be used to compactly describe common arithmetic operations.

PP3.6 S229
q+9
p+q
p+3q
8p+2
3q+14
p+7q+6

As an illustration of the use of `leaq` in compiled code, consider the following C program
```C
long scale(long x, long y, long z)
{
    long t = x + 4 * y + 12 * z;
    return t;
}
```
when compiled, the arithmetic operations of the function are implemented by a sequence of three `leaq` functions, as is documented by the comments on the right-hand side:
the ability of the leaq instruction to perform addition and limited forms of multiplication proves useful when compiling simple arithmetic expressions such as this example.

pp3.7 s230
rbx = 10y
rbx = 10y + z
rbx = 10y + z + x*y

### 3.5.2 Unary and Binary operations
operations in the second group are unary operations, with the single operand serving as both source and destination.
this operand can be either a register or a memory location.
the third group consists of binary operations, 
For example, the instruction `subq %rax, %rdx` decrements register `%rdx` by the value in `%rax`.
Note that when the second operand is a memory location, teh processor must read the value from memory, perform the operation, and then write the result back to memory.