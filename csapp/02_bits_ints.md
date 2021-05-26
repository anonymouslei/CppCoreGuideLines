# Bits, Bytes and Integers
- [Bits, Bytes and Integers](#bits-bytes-and-integers)
  - [Respresenting information as bits](#respresenting-information-as-bits)
  - [Bit-level manipulations](#bit-level-manipulations)
    - [Bit-Level OPerations in C](#bit-level-operations-in-c)
    - [Contrast: Logic Operations in C](#contrast-logic-operations-in-c)
    - [Shift Operations](#shift-operations)
  - [Integers](#integers)
    - [Representation: unsigned and signed](#representation-unsigned-and-signed)
      - [Numeric Ranges](#numeric-ranges)
      - [Values for different word sizes](#values-for-different-word-sizes)
      - [Unsigned & Signed Numeric Values](#unsigned--signed-numeric-values)
    - [Conversion, casting](#conversion-casting)
    - [Expanding, truncating](#expanding-truncating)
    - [Addition, negation, multiplication, shifting](#addition-negation-multiplication-shifting)
    - [Summary](#summary)
  - [Representations in memory, pointers, strings](#representations-in-memory-pointers-strings)

## Respresenting information as bits
- Byte = 8 bits
- Hexadecimal 00 to FF
- int has 32 (4 * 8) bits. 

## Bit-level manipulations
### Bit-Level OPerations in C
- Operations
  - & (ampersand) Intersection
  - | (vertical bar) Union
  - ^ (caret) Symmetric difference
  - ~ (tilde) Complement
### Contrast: Logic Operations in C
- contrast to Bit-level Operators, Logic Operations: &&, ||, !
  - always return 0 or 1
  - Early termination
  - **watch out for && vs. & (and || vs. |)**

### Shift Operations
- Left Shift: x << y
  - shift bit-vector x left y positions
  - throw away extra bits on left
  - fill with 0's on right
- Right shift: x >> y
  - shift bit-vector x right y positions
  - **Logical shift**
    - fill with 0's on left
  - **Arithmetic shift**
    - replicate most significant bit on left
- Undefined Behavior
  - shift amount < 0 or >= word size

## Integers
### Representation: unsigned and signed
#### Numeric Ranges
- Unsigned Values
  - UMin = 0
  - UMax = 2^w - 1
- Two's Complement Values
  - TMin = -2^(w - 1)
  - TMax = 2^(w - 1) - 1 
  
#### Values for different word sizes
|  |   |  |   |
|--| --|--| --|
|  | 8  | 16 | 32  |
|UMax  |255   |65535  |4294967295   |
|TMax  |127   |32767  |2147483647   |
|TMin  |-128   |-32768  |-2147483648   |

- Observations
  -  |Tmin| = TMax + 1
  -  UMax = 2 * TMax + 1

#### Unsigned & Signed Numeric Values
- Equivalence
  - same encodings for nonnegative values

### Conversion, casting

<!-- TODO: S31 -->
### Expanding, truncating
### Addition, negation, multiplication, shifting
### Summary
## Representations in memory, pointers, strings

&, |, ^, ~ (ampersand, vertical bar, caret and tilde)
S24: B2U: from a bit level representation to an unsigned number of some bit pattern

until: 25:58

<!-- Then I'll treat themn as a signed case,  -->
if either of them is unsigned, Then I will convert the other one to be an unsigned number and do the operation.

可以使用对称性来计算绝对值

```c
if (x < 0)
    return -x;
else
    return x;
```
if we input TMin, it will returns TMin.
It is always a corner case and whenever we do program testing.

UMax = 2*TMax + 1

it takes three clock cycles to do multiplication.
shift needs one clock cycles

it takes 30 clock cycles to do divide

negate :
1. complement it meaning flip all the bits
2. add one to that

```
 1 0 1 0
 0 1 0 1 (flip all the bits)
 0 0 0 1 (add one)
 0 1 1 0 (result)
 ```

2^10 which is 1024 which is approximately equal to 10^3.
What it means is that 10 bits worth of number is about the same as three decimal digits.
So for example 2^20 is around 10^6.

it turns out that increasingly it's getting hard to find big-Endian machines

ux > -1 // 无符号的数会将-1转化为成无符号数，-1的二进制都是1，是最大的无符号数。所以ux>-1恒不成立