# Machine-level programming I: Basics

- [Machine-level programming I: Basics](#machine-level-programming-i-basics)
  - [History of Intel processors and architectures](#history-of-intel-processors-and-architectures)
  - [Assembly Basics: registers, operands, move](#assembly-basics-registers-operands-move)
    - [Definitions](#definitions)
    - [Assembly/Machine Code View](#assemblymachine-code-view)
    - [Assembly Characteristics: Data types](#assembly-characteristics-data-types)
    - [assembly characteristics: operations](#assembly-characteristics-operations)
    - [Moving Data](#moving-data)
    - [movq Operand Combinations](#movq-operand-combinations)
    - [simple memory addressing modes](#simple-memory-addressing-modes)
    - [Complete Memory Addressing Modes](#complete-memory-addressing-modes)
    - [address computation examples](#address-computation-examples)
  - [Arithmetic & logical operations](#arithmetic--logical-operations)
    - [Address computation instruction](#address-computation-instruction)
    - [some arithmetic operations](#some-arithmetic-operations)
    - [arithmetic expression example](#arithmetic-expression-example)
  - [C, assembly, machine code](#c-assembly-machine-code)


## History of Intel processors and architectures

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

### Assembly Characteristics: Data types
- "Integer" data of 1,2,4, or 8 bytes
  - data values
  - addresses (untyped pointers)
- Floating point data of 4, 8 or 10 bytes
- (SIMD vector data types of 8,16,32 or 64 bytes) 
- code: byte sequences encoding series of instructions
- no aggregate types such as arrays or structures
  - just contiguously allocated bytes in memory

### assembly characteristics: operations
- Transfer data between memory and register
  - load data from memory into register
  - store register data into memory
- Perform arithmetic function on register or memory data
- transfer control
  - unconditional jumps to/from procedures
  - conditional branches
  - indirect branches

### Moving Data
- Moving Data
  `movq source, Dest`
- Operand Types
  - immediate: constant integer data
    - example: $0x400, $-533
    - like C constant, but prefixed with `$`
    - Encoded with 1, 2, or 4 bytes
  - register: one of 16 integer registers
    - example: %rax, %r13
    - but %rsp reserved for special use
    - others have special uses for particular instructions
  - memory: 8 consecutive bytes of memory at address given by register
    - simplest example: (%rax)
    - various other "addressing modes"

**warning**: intel docs use `mov Dest, source` 
### movq Operand Combinations

| source | dest | src, dest | C Analog|
| --  | --   | --           | --|
| Imm| reg| movq $0x4, %rax | temp = 0x4; |
| Imm| mem| movq $-147, (%rax) | *p = -147 |
| reg | reg | movq %rax, %rdx |  temp2 = temp1; |
| reg | mem | movq %rax, (%rdx) | *p = temp; |
| mem | reg | movq (%rax), %rdx | temp = *p; |

**cannot do memory-memory transfer with a single instruction**

### simple memory addressing modes
TODO: 看不懂
- normal (R) Mem[Reg[R]]
  - Register R specifies memory address
  - Aha! Pointer dereferencing in C

  `movq (%rcx), %rax`
- Displacement D(R) Mem[Reg[R]+D]
  - Register R specifies start of memory region
  - Constant displacement D specifies offset

  `movq 8 (%rbp), %rdx`

### Complete Memory Addressing Modes
- most general form
  | D(Rb, Ri, S) | Mem[Reg[Rb]+S*Reg[Ri]+D]|
  | -- | -- |

  - D: constant "displacement" 1, 2, or 4 bytes
  - Rb: Base register: any of 16 integer registers
  - Ri: Index register: any, except for %rsp
  - S: scale: 1, 2, 4, or 8 (why these numbers?)

- special case
  |(Rb, Ri)| Mem[Reg[Rb]+Reg[Ri]]|
  |--|--|
  |D(Rb,Ri)|Mem[Reg[Rb]+Reg[Ri]+D|
  |(Rb,Ri,S)|Mem[Reg[Rb]+S*Reg[Ri]]

### address computation examples

|%rdx|0xf000|
|--|--|
|%rcx|0x0100|

|expression|address computation|address|
|--|--|--|
|0x8(%rdx)|0xf000+0x8|0xf008|
|(%rdx,%rcx)|0xf000+0x100|0xf100|
|(%rdx,%rcx,4)|0xf000+4*0x100|0xf400|
|0x80(,%rdx,2)|2*0xf000+0x80|0x1e080|

## Arithmetic & logical operations
### Address computation instruction
- leaq Src, Dst
  - Src is address mode expression
  - Set Dst to address denoted by expression
- Uses
  - computing addresses without a memory reference
  - computing arithmetic expressions of the form x + k*y
    - k = 1,2,4, or 8

### some arithmetic operations
- two operand instructions
  | format | computation| |
  |--| --| --|
  | addq | Src, Dest | Dest = Dest + Src |
  |subq | Src, Dest | Dest = Dest - Src|
  | imulq | Src, Dest | Dest = Dest * Src |
  | salq (also called shlq arithmetic logical) | Src, Dest | Dest = Dest << Src |
  | sarq | Src, Dest | Dest + Dest >> src|
  | shrq | Src, Dest| Dest = Dest >> Src| 
  | xorq | Src, Dest | Dest = Dest ^ Src |
  |andq| Src, Dest| Dest=Dest&Src|
  | orq | Src, Dest | Dest = Dest | Src|
- watch out for argument order!
- No distinction between signed and unsigned int (Why?)
- one operand instructions
  | format | computation| |
  |--| --| --|
  |incq|Dest|Dest = Dest + 1|
  |decq|Dest|Dest = Dest -1|
  |negq|Dest|Dest = -Dest|
  |notq|Dest|Dest =~Dest|
- see book for more instructions
### arithmetic expression example
- Interesting instructions
  - leaq: address computation
  - salq: shift
  - imulq: multiplication
TODO:新版S43页看不懂



## C, assembly, machine code
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