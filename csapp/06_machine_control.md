# Machine-Level Programming II: control
## Control
## Conditional branches
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

26:16