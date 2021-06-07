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
  - [IEEE floating point standard: Definition](#ieee-floating-point-standard-definition)
    - [driven by numerical concerns](#driven-by-numerical-concerns)
  - [Example and properties](#example-and-properties)
  - [Rounding, addition, multiplication](#rounding-addition-multiplication)
  - [Floating point in C](#floating-point-in-c)
  - [Summary](#summary)


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

<!-- S14 -->

## IEEE floating point standard: Definition
### driven by numerical concerns
  - nice standards for rounding, overflow, underflow
  - hard to make fast in hardware
## Example and properties
## Rounding, addition, multiplication
## Floating point in C
## Summary



S10: the significand M and this is sometimes called the mantissa.

10min starts to tell how float represents.

0<= Exp <= 2^8-1 (255)
-127 <= E <= 128

Normalized encoding: we can't represent 0, because there is always a 1. 

Denormalized values: there is no implied 1. we can represent zero.
but you have odd situation where there's a positive and negative zero.

it has a nice smooth transition from denorm to normalize.

S26: every time you increase the exponent by one, the numbers are spaced twice as far apart as teh previous.
the numbers that were represented by the previous exponent.

S34 didn't understand
50min

puzzles:
float will lost some bits. because it only has 23 bits. but int has 32 bits.