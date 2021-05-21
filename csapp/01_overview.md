# Overview
- [Overview](#overview)
  - [Course theme](#course-theme)
    - [Abstraction is Good but don't forget reality](#abstraction-is-good-but-dont-forget-reality)
  - [Five realities](#five-realities)
    - [1: ints are not Integers, FLoats are not Reals](#1-ints-are-not-integers-floats-are-not-reals)
    - [2: you've got to know assembly](#2-youve-got-to-know-assembly)
    - [3: Memory Matters](#3-memory-matters)
    - [Memory Referencing Errors](#memory-referencing-errors)
    - [4: There is more to performance than asymptotic complexity](#4-there-is-more-to-performance-than-asymptotic-complexity)
    - [5: computers do more than execute programs](#5-computers-do-more-than-execute-programs)
  - [How the course fits into the CS/ECE curriculum](#how-the-course-fits-into-the-csece-curriculum)

## Course theme
### Abstraction is Good but don't forget reality
- most CS and CE courses emphasize abstraction
  - abstract data types
  - asymptotic analysis
- these abstractions have limits
  - especially in the presence of bugs
  - need to understand details of underlying implementations

## Five realities
### 1: ints are not Integers, FLoats are not Reals
- Cannot assume all "usual" mathematical properties
  - due to finiteness of representations
  - Integer oprations satisfy **ring** properties
    - Commutativity, associativity, distributivity
  - Floating point operations satisfy "ordering" properties
    - Monotonicity, values of signs
    
### 2: you've got to know assembly 
- understanding assembly is key to machine-level execution model

### 3: Memory Matters
- Memory is not unbounded
  - it must be allocated and managed
  - Many applications are memory dominated
- Memory referencing bugs especially pernicious
  - Effects are distant in both time and space
- Memory performance is not uniform
  - Cache and virtual memory effects can greatly affect program performance
  - adapting program to characteristics of memory system can lead to major speed improvements
  
### Memory Referencing Errors
- C and C++ do not provide any memory protection
  - Out of bounds array references
  - Invalid pointer values
  - Abuses of malloc/free
- Can lead to nasty bugs
  - Whether or not bug has any effect depends on system and compiler
  - Action at a distance
    - corrupted object logically unrelated to one being accessed
    - Effect of bug may be first observed long after it is generated
- How can I deal with this?
  - use of develop tools to detect referencing errors (e.g. Valgrind)

### 4: There is more to performance than asymptotic complexity
- Must understand system to optimize performance
  - how programs compiled and executed
  - how to measure program performance and identify bottlenecks
  - how to improve performance without destroying code modularity and generality
- Hierarchical memory organization
- Performance depends on access patterns
  - including how step through multi-dimensional array

### 5: computers do more than execute programs
- they need to get data in and out
  - I/O system critical to program reliability and performance
- They communicate with each other over networks
  - Many system-level issues arise in presence of network
    - Concurrent operations by autonomous processes
    - Coping with unreliable media
    - Cross platform compatibility
    - Complex performance issues

## How the course fits into the CS/ECE curriculum

read each chapter three timnes, work the practice problems, 