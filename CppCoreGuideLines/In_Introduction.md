# In: Introduction
- [In: Introduction](#in-introduction)
  - [In.target: Target readership](#intarget-target-readership)
  - [In.aims: Aims](#inaims-aims)
  - [In.not: Non-aims](#innot-non-aims)
  - [In.force: Enforcement](#inforce-enforcement)
  - [In.struct: The structure of this document](#instruct-the-structure-of-this-document)
  - [In.sec: Major sections](#insec-major-sections)

## In.target: Target readership
the rules emphasize static type and resource safety. for that reason, they emphasize possibilities for range checking, for avoiding dereferencing `nullptr`, for avoiding dangling pointers, and the systematic use of exceptions (via RAII). Partly to chieve that and partly to minimize obscure code as a source of errors, the rules also emphasize simplicity and the hiding of necessary complexity behind well-specified interfaces.

## In.aims: Aims
## In.not: Non-aims
## In.force: Enforcement
where appropriate, we label a rule (in the #Enforcement# sections) with the name of groups of related rules(called "profiles"). A rule can be part of several profiles ,or none.

We prefer **“mechanical”** checking and **static checking**.
run-time checks are suggested only rarely where no alternative exists.
## In.struct: The structure of this document
## In.sec: Major sections
- [P:Philosophy](./P_Philosophy.md)
- I: Interfaces
- F: Functions
- C: Classes and class hierarchies
- Enum: Enumerations
- R: Resource management
- ES: Expressions and statements
- Per: Performance
- CP: Concurrency and parallelism
- E: Error handling
- Con: Constants and immutability
- T: Templates and generic programming
- CPL: C-style programming
- SF: Source files
- SL: The Standard Library