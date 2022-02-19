
# Machine-level Representation of Programs
the gcc C compiler generates its output in the form of assembly code, a textual representation of the machine code giving the individual instructions in the program.
Gcc then invokes both an assembler and a linker to generate the executable machine code from the assembly code.
programmers seeking to maximize the performance of a critical section of code often try different variations of the source code, each time compiling and examining the generated assembly code to get a sense of how efficiently teh program will run.

there are times when the layer of abstraction provided by a high-level language hides information about the run-time behavior of a program that we need to understand.

for example, when writing concurrent programs using a thread package, it is important to understand how program data are shared or kept private by the different threads and precisely how and where shared data are accessed.

unterstanding how these vulneerabilities arise and how to guard against them requires a knowledeg of the machine-level representation of programs.

relative to the computations expressed in the C code, optimizing compilers can rearrange execution order, eliminate unneeded computation, replace slow operations with faster ones, and even change recursive computations into iterative ones.

## 3.1 A historical perspective

