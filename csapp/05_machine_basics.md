# Machine-level programming I: Basics

- [Machine-level programming I: Basics](#machine-level-programming-i-basics)
	- [Assembly Basics: registers, operands, move](#assembly-basics-registers-operands-move)
		- [Definitions](#definitions)
		- [Assembly/Machine Code View](#assemblymachine-code-view)
	- [History of Intel processors and architectures](#history-of-intel-processors-and-architectures)
	- [C, assembly, machine code](#c-assembly-machine-code)
	- [Arithmetic & logical operations](#arithmetic--logical-operations)

## Assembly Basics: registers, operands, move
### Definitions
- Architecture: (also ISA: instruction set architecture) the parts of a processor design that one needs to understand for writing assembly / machine code.
  - examples: instruction set specification, registers
- Microarchitecture: implementation of the architecture
  - examples: cache sizes and core frequency
- Code forms:
  - machine code: the byte-level programs that a processor executes
  - assembly code: a text representation of machine code

### Assembly/Machine Code View
- Programmer-visible state
  - PC: program counter
    - address of next instruction
    - called "RIP"
  - Register file
    - heavily used program data
  - condition codes
    - store status infomation about most recent arithmetic or logical operation
    - used for conditional branching
  - Memory
    - byte addressable array
    - code and user data
    - stack to support procedures

## History of Intel processors and architectures
## C, assembly, machine code
## Arithmetic & logical operations

S14: cache: if you re-access that memory it will go faster. 
But it is not visible in terms of there is no instructions to manipulate the cache.
there is no way you can directly access the cache. 

`gcc -Og -S`: take the C code and turn it into assembly code
`-S`: means stop, just do the first part C to assembly code
`-Og`: is a specification of what kind of optimization I want the compiler to do
If you don't given directive, it will generate completely unoptimized code and it's actually very hard to read that code.

```shell
gcc -Og sum.c -o sum
objdump -d sum > sum.d// disassembler and redirect to a file
```
 S50: the disassembler didn't have access to the source  code. It didn't have even to the assembly code. it figured this out just by the bytes in the actual object code file.
 ```
 (gdb) disassemble sumstore

 ```
 for each register if you use the sort of %r name of it, you will get 64 bit. But if you use the %e version of it. you will get 32 bits.
 %e version is just the low-order 32 bits of a larger %r entity. (感觉现在的机器变了)
 旧版最后几页ppt新版没有

 it does not make sense to habe an immediate value as a destination. it is a constant.
 for sort of the sake of convenience for the hardware designers, It doesn't let you directly copy from one memory location oto another. you have to use two instructions.
 1. copy from memory to read it from memory copy it to a register
 2. take that value in the register and write it to memory

 S29 %rdi will be the first argument register and %rsi will be the second argument reegister. there can be up to six of these.