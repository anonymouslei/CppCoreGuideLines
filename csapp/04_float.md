# floating point
- [floating point](#floating-point)
  - [Background: Fractional binary numbers](#background-fractional-binary-numbers)
    - [representation](#representation)
    - [Observations](#observations)
    - [Limitation #1](#limitation-1)
    - [Limitation #2](#limitation-2)
    - [Numerical Form](#numerical-form)
    - [Encoding](#encoding)
    - [Precision options](#precision-options)
    - [three "kinds" of floating point numbers](#three-kinds-of-floating-point-numbers)
      - [Normalized values](#normalized-values)
      - [Denormalized values](#denormalized-values)
      - [special values](#special-values)
  - [Example and properties](#example-and-properties)
    - [Tiny floating point example](#tiny-floating-point-example)
    - [Distribution of values](#distribution-of-values)
    - [special properties of the IEEE encoding](#special-properties-of-the-ieee-encoding)
  - [Rounding, addition, multiplication](#rounding-addition-multiplication)
    - [Basic idea](#basic-idea)
    - [Rounding](#rounding)
    - [closer look at round-to-even](#closer-look-at-round-to-even)
    - [rounding binary numbers](#rounding-binary-numbers)
    - [FP multiplication](#fp-multiplication)
    - [Floating point addition](#floating-point-addition)
    - [Mathematical properties of FP add](#mathematical-properties-of-fp-add)
    - [Mathematical properties of FP Mult](#mathematical-properties-of-fp-mult)
  - [Floating point in C](#floating-point-in-c)
    - [puzzles](#puzzles)
  - [Summary](#summary)
    - [creating floating point number](#creating-floating-point-number)
    - [Interersting numbers](#interersting-numbers)


## Background: Fractional binary numbers
### representation
- bits to right of "binary point" represent fractional powers of 2
- represents rational number:
  $$\sum_{k=-j}^{i} b_k*2^{k}$$
### Observations
- numbers of form 0.11111....(2) are just below 1.0
  - use notation 1.0-e
### Limitation #1
- can only exactly represent numbers of the form x/2^k
  - other rational numbers have repeating bit representations
### Limitation #2
- just one setting of binary point within the w bits
  - limited range of numbers (very small values? very larget?)

### Numerical Form
$$(-1)^{s}M2^{E}$$
- **sign bit s**  determines whether number is negative or positive
- **Significand M** normally a fractional value in range [1.0, 2.0)
- **Exponent E** weights value by power of two
### Encoding
- MSB s is sign bit s
- **exp** field encodes E (but is not equal to E)
- **frac** field encodes M (but is not equal to M)

### Precision options
- single precision: 32 bits
s: 1 bit
exp: 8-bits
frac: 23 bits
- double precision: 64 bits
s: 1 bit
exp: 11 bits
frac: 52 bits
- other formats: half precision, quad precision

### three "kinds" of floating point numbers
- denormalized: 00...00
- normalized: exp != 0 and exp != 11...11
- special: 11...11

it has a nice smooth transition from denorm to normalize.
#### Normalized values
$$v=(-1)^{s}M2^{E}$$
- when: exp != 00...0 and exp != 11...1
- exponent coded as a biased value: E = exp - Bias
  - exp: unsigned value of exp field
  - bias = $2^{k-1}-1$, where k is number of exponent bits
    - single precision: 127 (exp: 1...254, E: -126...127)
    - double precision: 1023 (exp: 1...2046, E: -1022...1023)
- significand coded with implied leading 1: M = 1.xxx...x(2)
  - xxx...x: bits of frac field
  - Minimum when frac=00...00(M = 1.0)
  - Maximum when frac=111...11(M = 2.0-e)
  - get extra leading bit for "free"

Normalized encoding: we can't represent 0, because there is always a 1. 
#### Denormalized values
- condition: exp = 00...00
- Exponent value: E = 1 - bias (instead of exp - Bias)
- significand coded with implied leading 0: M = 0.xx...x(2)
- cases:
  - exp = 00...00, frac = 00...00
    - represents zero value
    - note distinct values: +0 and -0
  - exp = 00...00, frac != 00...00
    - numbers closest to 0.0
    - equispaced

Denormalized values: there is no implied 1. we can represent zero.
but you have odd situation where there's a positive and negative zero.


#### special values
- condition: exp = 11...11
- case: exp = 11...11, frac = 00...00
  - **represents value infinity**
  - operation that overflows
  - both positive and negative
- case: exp = 11...11, frac != 00...00
  - not-a-number(NaN)
  - represents case when no numeric value can be determined
  - e.g., sqrt(-1)
## Example and properties
### Tiny floating point example
- 8-bit floating point representation
  - the sign bit is in the most significant bit
  - the next four bits are the exp, with bias of 7
  - the last three bits are the frac
- same general form as IEEE format
  - normalized, denormalized
  - representation of 0, NaN, infinity

### Distribution of values
the distribution gets denser toward zero

S26: every time you increase the exponent by one, the numbers are spaced twice as far apart as the previous.
the numbers that were represented by the previous exponent.

### special properties of the IEEE encoding
- FP zero same as integer zero
  - all bits = 0
- can (almost) use unsigned integer comparison
  - must first compare sign bits
  - must consider -0 = 0
  - NaNs problematic
    - will be greater than any other values

## Rounding, addition, multiplication
### Basic idea
- first compute exact result
- make it fit into desired precision
  - possibly overflow if exponent too large
  - possible round to fit into frac
### Rounding
- Towards zero
- Round down
- round up
- nearest even

### closer look at round-to-even
- when exactly halfway between two possible values
  - round so that least significant digit is even
- round to nearest hundredth
  - 7.895000 => 7.90
  - 7.885000 => 7.88

### rounding binary numbers
- binary fractional numbers
  - "even" when least significant bit is 0
  - "half way" when bits to right of rounding position = 100...(2)
- examples
round to nearest 1/4(2 bits right of binary point)

|value|binary|rounded|action|rounded value|
|--|--|--|--|--|
|2 3/32| 10.00011 | 10.00 | (<1/2-down) | 2 |
|2 3/16| 10.00110 | 10.01 | (>1/2-up) | 2 1/4|
|2 7/8 | 10.11100 | 11.00 | (1/2 -up) | 3 |
|2 5/8 | 10.10100 | 10.10 | (1/2 -down) | 2 1/2|

### FP multiplication
- $(-1)^{s1}M2^{E1}$ * $(-1)^{s2}M2^{E2}$
- exact result: $(-1)^{s}M2^{E}$
  - sign s: s1 ^ s2
  - significand M: M1 * M2
  - exponent E: E1 + E2
- fixing
  - if M >= 2, shift M right, increment E
  - if E out of range, overflow
  - round M to fit frac precision
- implementation
  - biggest chore is multiplying significands
  > 1.010*2^2 * 1.110*2^3 = 10.0011*2^5 = 1.00011*2^6 = 1.001*2^6

### Floating point addition
- $(-1)^{s1}M2^{E1}$ * $(-1)^{s2}M2^{E2}$
  - assume E1 > E2
- exact result: $(-1)^{s}M2^{E}$
  - sign s, significand M:
    - result of signed align & add
  - exponent E: E1
- fixing
  - if M>=2, shift M right, increment E
  - if M <1, shift M left k positions, decrement E by k
  - overflow if E out of range
  - round M to fit frac precision
> 1.010*2^2 + 1.110*2^3 = (0.1010+1.1100)*2^3 = 10.00110 *2^3 = 1.00110*2^4 = 1.010 * 2^4

### Mathematical properties of FP add
- closed under addition?  **yes**
  - but may generate infinity or NaN
- commutative? **yes**
- associative? **no**
  - overflow and inexactness of rounding 
  > (3.14 + 1e10) - 1e10 = 0
  > 3.14 + (1e10 - 1e10) = 3.14
- 0 is additive identity? **yes**
- every element has additive inverse? **almost**
  - yes, except for infinities & NaNs
- a >= b => a + c >= b + c? **almost**
  - except for infinities & NaNs

### Mathematical properties of FP Mult
- closed under multiplication? **yes**
  - but may generate infinity or NaN
- Multiplication commutative? **yes**
- multiplication is associative? **no**
  - possibility of overflow, inexactness of rounding
  - > (1e20*1e20)*1e-20 = inf,
    > 1e20*(1e20*1e-20) = 1e20
- 1 is multiplicative identity? **yes**
- multiplication distributes over addition? **no**
  - possibility of overflow, inexactness of rounding
  - > 1e20*(1e20-1e20) = 0.0
    > 1e20*1e20 - 1e20*1e20 = NaN
- monotonicity
  - a>=b & c>=0 => a*c >= b*c? **almost**
    - except for infinities & NaNs 

## Floating point in C
- C guarantees two levels
  - float: single precision
  - double: double precision
- conversions/casting
  - casting between int, float, and double changes bit representation
  - double/float -> int
    - truncates fractional part
    - like rounding toward zero
    - Not defined when out of range or NaN: Generally sets to TMin
  - int -> double
    - exact conversion, as long as int has <= 53 bit word size
  - int -> float 
    - will round according to rounding mode
### puzzles
```c
int x = ...;
float f = ...;
double d = ...;
```
assume neither d nor f is NaN
- x == (int)(float) x   // wrong, float will lost some bits. because it only has 23 bits. but int has 32 bits.

## Summary
- IEEE Floating point has clear mathematical properties
- represents numbers of form M*2^E
- one can reason about operations independent of implementation
  - as if computed with perfect precision and then rounded
- not the same as real arithmetic
  - violates associativity/distributivity
  - makes life difficult for compilers & serious numerical applications programmers

### creating floating point number
- steps
  - normalize to have leading 1
  - round to fit within fraction
  - postnormalize to deal with effects of rounding 
- case study
  - convert 8-bit unsigned numbers to tiny floating point format 
  - normalize
    - set binary point so that numbers of form 1.xxx
    - adjust all to have leading one
      - decrement exponent as shift left
  - postnormalize
    - rounding may have cased overflow
    - handle by shifting right & incrementing exponent 

### Interersting numbers
| description| exp | frac | numeric value{single, double}| note|
| --         | --  | --   | --           | --|
| zero       | 00...00 | 00...00 | 0.0 |
| smallest Pos. denorm | 00...00 | 00...01 | 2^(-{23,52})*2^(-{126,1022})| single ~= 1.4*1e-45 double ~= 4.9*1e-324
| largest denormalized | 00..00 | 11..11 | (1.0-e)*2^{126,1022} | single~=1.18*1e-38, double~=2.2*1e-308 |
smallest Pos.Normalized | 00..01 | 00..00 | 1.0*2^{126, 1022} | just larger than largest denormalized |
| one | 01..11 | 00..00| 1.0 ||
|largest normalized| 11..10 | 11..11 | (2.0-e)*2^{127,1023}| single~=3.4*1e38, double~=1.8*1e308 |
