# Machine-Level Programming II: control
- [Machine-Level Programming II: control](#machine-level-programming-ii-control)
  - [Recall](#recall)
    - [why use LEA?](#why-use-lea)
    - [sidebar: instruction suffixes](#sidebar-instruction-suffixes)
  - [Control](#control)
    - [processor state (x86-64, Partial)](#processor-state-x86-64-partial)
    - [condition codes (Implicit setting)](#condition-codes-implicit-setting)
    - [condition codes (explicit setting: compare)](#condition-codes-explicit-setting-compare)
    - [condition codes (explicit setting: test)](#condition-codes-explicit-setting-test)
    - [condition codes (explicit reading: set)](#condition-codes-explicit-reading-set)
  - [Conditional branches](#conditional-branches)
    - [jumping](#jumping)
    - [using conditional moves](#using-conditional-moves)
  - [Loops](#loops)
  - [switch statements](#switch-statements)

## Recall
### why use LEA?
- CPU designers' intended use: calculate a pointer to an object
  - an array element, perhaps
  - For instance, to pass just one array element to another function

 | assemble                 | C equivalent    |
 | ------------------------ | --------------- |
 | lea(%rbx, %rdi, 8), %rax | rax = &rbx[rdi] |
- compiler authors like to use it for ordinary arithmetic
  - it can do complex calculations in one instruction
  - it's one of the only three-operand instructions the x86 has
  - it doesn't touch the condition codes (we'll come back to this)

  | assembly                 | C equivalent  |
  | ------------------------ | ------------- |
  | lea(%rbx, %rbx, 2), %rax | rax = rbx * 3 |


### sidebar: instruction suffixes

- mose x86 instructions can be written with or without a suffix
  - imul %rcx, %rax
  - imulq % rcx, %rax (there is no difference!)

- the suffix indicates the operation size
  - b=byte, w=short, l=int, q=long
  - if present, must match register names
- assembly output from the compiler (gcc -S) usually has suffixes
- disassembly dumps (objdump -d, gdb 'disas') usually omit suffixes
- intel's manuals always omit the suffixes

## Control
### processor state (x86-64, Partial)
- information about currently executing program
  - temporary data (%rax, ...)
  - location of runtime stack (%rsp)
  - location of current code control point (%rip, ...)
  - status of recent tests (CF, ZF, SF, OF) condition codes

### condition codes (Implicit setting)
- single bit registers
  - CF Carry Flag (for unsigned)
  - SF sign flag (for signed)
  - ZF zero flag
  - OF Overflow flag (for signed)
- implicitly set (as side effect) of arithmetic operations
  - example: addq Src, Dest <-> t = a+b
  - CF set: if carry/borrow out from most significant bit(unsigned overflow)
  - ZF set: if t == 0
  - SF set: if t < 0 (as signed)
  - OF set: if two's-complement (signed) overflow
(a > 0 && b > 0 && t < 0) || (a < 0 && b < 0 && t>=0)
- not set by leaq instruction

### condition codes (explicit setting: compare)
- explicit setting by compare instruction
  - cmpq Src2, Src1
  - cmpq b, a like computing a - b without setting destination

  - CF set: if carry/borrow out from most significant bit (used for unsigned comparisons)
  - ZF set: if a==b
  - SF set: if (a-b) < 0 (as signed)
  - OF set: if two's-complement (signed) overflow 
  (a > 0 && b < 0 && (a-b)<0 || (a<0 && b>0 && (a-b)>0)

### condition codes (explicit setting: test)
- explicit setting by test instruction
  - testq src2, src1
    - testq b, a like computing a&b without setting destination
  - sets condition codes based on value of src1 & src2
  - useful to have one of the operands be a mask

  - ZF set when a&b == 0
  - SF set when a&b < 0
  - very often: testq %rax, %rax

### condition codes (explicit reading: set)
- explicit reading by set instructions
  - setX Dest: set low-order byte of destination Dest to 0 or 1 based on combinations of condition codes
  - does not alter remaining 7 bytes of Dest

| SetX  | Condition    | Description               |
| ----- | ------------ | ------------------------- |
| sete  | ZF           | Equal/zero                |
| setne | ~ZF          | not equal / not zero      |
| sets  | SF           | Negative                  |
| setns | ~SF          | nonnegative               |
| setg  | ~(SF^OF)&~ZF | greater (signed)          |
| setge | ~(SF^OF)     | greater or equal (signed) |
| setl  | SF^OF        | less(signed)              |
| setle | (SF^OF)\|ZF  | Less or equal (signed)    |
| seta  | ~CF&~ZF      | above(unsigned)           |
| setb  | CF           | below(unsigned)           |

## Conditional branches
### jumping
- jX instructions
  - jump to different part of code depending on condition codes
  - implicit reading of condition codes

### using conditional moves
- conditional move instructions
  - instruction supports:
    - if (Test) Dest <- Src
  - supported in post-1995 x86 processors
  - GCC tries to use them 
    - but, only when known to be safe

- why
  - branches are very disruptive to instruction flow through pipelines
  - conditional moves do not require control transfer
(S28)



## Loops
## switch statements

last time we introduced the very basic principles of machine level programming. 
The idea of there being special state that you have access to the registers.
Register is part of memory?
No
Are they part of the cache? No
They special.
They are different, they don't exist to a C level program.
When you are programming in C, you don't have any visibility into them.  

This class we will learn how you control the flow of execution of instructions at the machine level.
And how you use those techniques to implement basic constructs conditional statements, loops and switch statements.

S10: `%rsp` is special. 
It is the stack pointer tells you where you are. 
It manages the execution of procedures.
SO you can not just use that or set it in arbitrary ways.
But the other ones you more or less come with qualifications to that statement.

S11:
how do you know if addition of two values is overflowed?
If the signs of the operands are the same, 
but the sign of the result is fifferent than that.
So either you added two positive numbers and got a negative result.
That was a positive overflow.
Or you added two negative nembers and got a positiv result.
That's negative overlfow.

S16: remember that the arguments in the way we write them in machine code is the opposite of what you'd expect. 

S17: 
you'd think it's when you have two values and you want to compare them to each other. 
And test is if you really just have one value and you want to see what it is like.
It's zero? negative?

S20
it turns out for every one of these 16 registers, 
you can directly set the lowest order byte of it to either 0 or 1.
And it woun't affect it, it turns out it doesn't affect any of the other 7 bytes of that register.

S21:
the first argument is passed in register %rdi and the second register %rsi,
And that if I am going to return a result from a function I do that by storing it and register %rax before I return.

setg: set this single byte register, the lowest byte of %rax to 1. 
If the result is greater.

S22: take this one bit result, which is the lower bit of register %rax, 
copy it to the same place as it already is.
Then set the remaining 7 bytes to 0.

S25: 
in general in assembly code if you give a name and then a colon,
what's to the left of that is called a label.
And it is just a tag that it is only visible in teh assembly code,
it is not actualy in the object code.
When you are looking at assembly code, 
One of the features it provides is a little bit easier way to understand it then to look at addresses.

I don't have to do anything special to return a value back from a function.
All I habe to do the stick register %rax, 
and calling function will know that's where to find the return value.

1. the result to end up in %rax
2. remember teh all the two argument instructions,
you take two balues and then second argument the destination argument,
you overwrite what was there with the new result.

This is a pretty typical pattern,
you move some value into a register and then you into %rax is ver common.
And then you do some arithmetic operation with that register as a destination to kind of update it.

S28: conditional move,
the basic idea is I am going to compute both them 'then' and the 'else' the values they'd produce.
And only after that I choose which one I use.
It seems wasteful but it actually turns out to be more efficient, If there are fairly simple computations.

A modern processor is a little like an oil tanker sailing in the sea, meaning that it plows along in a certain direction.
And it really is hard to get it to stop or to turn.
Think of an instruction sequences you know the ocean of code that you're cruising through, 
And these things can just fly through a straight sequence of code.
Because they are do pipelining.
Meaning they start bringing in parts of one instruction before they're done with the next. 
And actually go up tp depths of 20 or more instructions deep of how far had they're working sort of fetching forward and future instructions.
While finishing up the ones that still remain.

But all of a sudden you come and then what happens when they hit a branch.
They'll try to guess, it's called branch prediction,
and guess where is this kind of branch going to go.
where it you know well the conditional branch will be taken or is it going to fall through,
they are good at it 98% of the time.
and as long as they get it right it is very efficient.
But if they get it wrong it's as if you have to stop the thing back it up turn the other way start again.
And that can take 40 instructions 40 clock cycles say to do in bad situations.

So these conditional move instructions it turns out it's much easier to just plow through compute both.
and then at the last minute all you have to do is move a value into a register or not.
And that doesn't require stopping the whole processor and making this turn.

S48: ja: jump above
If x was either less than zero or greater than 6. It would go to default case.

that lets me index into a table and extract out of that an address and then jump to that address.

S53 TODO: do not understand

the question is of what if it's a negative number for your case.
It will typically add a bias to it effectively.
so that whatever your lowest value was become zero,
or it'll do something like that to avoid having to do negative indexing.

if you only have case 0 and case 10000, so it still create a million entry table for just two cases.
so it will trun it into if else code.

It knows in advance all the values and it will actually set up a tree of if-else.
it does it with a logarithmic number of tests rather than a linear number of tests.

