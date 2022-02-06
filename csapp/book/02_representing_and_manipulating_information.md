# Part 1: Program Structure and Execution
# Chapter 2: Representing and Manipulating information
Modern computers store and process information represented as two-valued signals.
these lowly binary digits, or bits, form the basic of the digital revolution.

- unsigned: encodings are based on traditional binary notation, representing numbers greater than or equal to 0.
- two's-complement: encodings are the most commonm way to represent signed integers, that is, numbers that may be either positive or negative.
- Folating-point: encodings are a base-2 version of scientific notation for representing real numbers.
  
## 2.1 Information Storage
Rather than accessing individual bits in memory, most computers use blocks of 8 bits, or bytes, as the smallest addressable unit of memory.
A machine-level program views memory as a very large array of bytes, referred to as virtual memory.
Every byte of memory is identified by a unique number, known as its address, and the set of all possible addresses is known as the virtual address space.

### 2.1.1 Hexadecimal Notation
### 2.1.2 Data Sizes
since a virtual address is encoded by such a word, the most important system parameter determined by the word size is the maximum size of the virtual address space.

That is, for a machine with a w-bit word size, the virtual addresses can range from 0 to 2w -1, giving the program access to at most 2w bytes.

### 2.1.3 Addressing and byte ordering
what the address of the object will be, and how we will order the bytes in memory.
In virtually all machines, a multi-byte object is stored as a contiguous sequence of bytes, with the address of the object given by the smallest address of the bytes used.

```C
#include <stdio.h>

typedef unsigned char* byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    int i;
    for (i = 0; i< len; i++)
        printf(" %.2x", start[i]);
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void* x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}
```

The typedef declaration in C provides a way of giving a name to a data type.
This can be a greate help in improving readability.

### 2.1.4 Representing Strings
A string in C is encoded by an array of characters terminated by teh null character.

### 2.1.5 Representing Code
a program, from the perspective of the machine, is simply a sequence of bytes.
### 2.1.6 introduction to boolean algebra
since binary values are at the core of how computers encode, store, and
manipulate information, a rich body of mathematical knowledge has evolved
around the valuse 0 and 1.
he showed that Boolean algebra could be applied to the design and analysis
of networks of electromechanical relays.
Although computer technology has advanced considerably since, boolean algebra
still plays a central role in teh design and analysis of digital systems.

the mask represents the set of enabled signals.

### 2.1.7 bit-level operations in c
one useful feature of C is that it supports bitwise boolean operations.
as our examples show, the best way to determine the effect of a bit-level
experssion is to expand the hexadecimal arguments to their binary representations,
perform the operations in binary, and then convert back to hexadecimal.

```c
void inplace_swap(int* x, int *y) {
    *y = *x ^ *y;
    *x = *x ^ *y;
    *y = *x ^ *y;
}
```
as the name implies, we claim that the effect of this procedure is to swap
teh values  stored at the location denoted by pointer variables x and y.
```c
void reverse_array(int a[], int cnt)
{
    int first, last;
    for (first = 0, last = cnt-1;
    first <= last;
    first++, last--)
        inplace_swap(&a[first], &a[last]);
}
```

### 2.1.7 Bit-level operations in C
One useful eature of C is that it supports bitwise Boolean operations.
One common use of bit-level operations is to implement masking operations, where a mask is a bit pattern that indicates a selected set of bits within a word.

the expression ~0xFF creates a mask where the 8 least-significant bits equal 0 and the rest equal 1.

### 2.1.8 Logical operations in C
the expression `a&& 5/a` will never cause a division by zero.
`p && *p++` will never cause the dereferencing of a null pointer.

### 2.1.9 shift operations in C
`x << k` x is shifted k bits to the left, dropping off the k most significant bits and filling the right end with k zeros.
shift operations associate from left to right, so `x << j<< k` is equivalent to `(x <<j) << k`.

`x>>k` machines support two forms of right shift:
- logical
a logical right shift fills the left end with k zeros.
- arithmetic
an arithmetic right shift fills the left end with k repetitions of the most significant bit.

The C standards do not precisely define which type of right shift should be used with signed numbers - either arithmetic or logical shifts may be used.
this unfortunately means that any code assuming one form or the other will potentially encounter portability problems.

#### aside: shifting by k, for large values of k
for a data type consisting of w bits, what should be the effect of shifting by some value k>= w?
The C standards carefully avoid stating what should be done in such a case.
the shift amount is computed as k mod w. 
This behavior is not guaranteed for C programs, and so shift amounts should be kept less than the word size.

#### aside: operator precedence issues with shift operations
it might be tempting to write the expression `1<<2 + 3 << 4`, however, 
in C the former expression is equivalent to 1 <<(2+3) << 4, since
addition (and subtraction) have higher precedence that shifts.
The left-to-right associativity rule then causes this to be parenthesized as (1 << (2+3) << 4).

## 2.2 Integer Representations
### 2.2.1 Integral data types
the range of negative numbers extends one further than the range of positive numbers.
C standards require only a symmetric range of positive and negative numbers. 

### 2.2.2 unsigned encodings
B2U(for "binary to unsigned")

the unsigned binary representation has the important property that every number between 0 an 2w -1 has a unique encoding as a w-bit value.

Principle: Uniqueness of unsigned encoding:
Function B2Uw is a bijection.

### 2.2.3 Tow's-complement encodings
B2T(binary to two's complement)
this defined by interpreting the most significant bit of the word to have negative weight.
This asymmetry arises because half the bit patterns represent negative numbers, while half represent nonnegative numbers. Since 0 is nonnegative,
this means that it can represent one less positive number than negative.

### 2.2.4 Conversions between Signed and Unsigned
we see, then, that function T2U describes the conversion of a two's complement number to its unsigned counterpart, while U2T converts in the opposite direction.
P108

when mapping a signed number to its unsigned counterpart, negative numbers are converted to large positive numbers, while nonnegative numbers remain unchanged.

To summarize, we considered the effects of converting in both directions between unsigned and two's-complement representations.
numbers in this range have identical unsigned and two's -complement representations.
for values outside of this range, the conversions either add or subtract 2w. 

### 2.2.5 signed versus unsigned in C
When an operation is performed where one operand is signed and the other is unsigned, C implicitly casts the signed argument to unsigned and performs the operations assuming the numbers are nonnegative.
As we will see, this convention makes little difference for standard arithmetic operations, 
but it leads to nonintuitive results for relational operators such as < and >.

### 2.2.6 Expanding the Bit Representation of a Number
#### web aside Data: TMIN
one point worth making is that the relative order of conversion from one data size to another and between unsigned and signed can affect the behavior of a program.
this shows that, when converting from short to unsigned, the program first changes the size and then the type.

Function fun1 extracts a value from the low-order 8 bits of the argument, giving an integer ranging between 0 and 255. Function fun2 extracts a value from the low-order. Function fun2 extracts a value from the low-order 8 bits of the argument, but it also performs sign extension. the result will be a number between -128 and 127.

### 2.2.7 Truncating numbers
this intuition behind this principle is simply that all of the bits that were truncated have weights of the form 2i.

the effect of this truncation on unsigned values is to simply find their residue, modulo 8. The effect of the truncation on signed values is a bit more complex.
we first compute the modulo 8 residue of the argument.
this will give values 0 through 7 for arguments 0 through 7,
and also for arguments -8 through -1.

### 2.2.8 Advice on signed versus unsigned
the implicit casting of signed to unsigned leads to some non-intuitive behavior. 
Nonintuitive features often lead to program bugs, and ones involving the nuances of implicit casting can be especially difficult to see.

this problem is designed to demonstrate how easily bugs can arise due to the implicit casting from signed to unsigned.
It seems quite natural to pass parameter length as an unsigned, 
since one would never want to use a negative length.
The stopping criterion i<= length-1 also seems quite natural.
but combining these two yields an unexpected outcome!

Since parameter length is unsigned, the computation 0-1 is performed using unsigned arithmetic, which is equivalent to modular addition.

we have seen multiple ways in which the subtle features of unsigned arithmetic, and especially the implicit conversion of signed to unsigned, can lead to errors
or vulnerabilities.

## 2.3 Integer arithmetic
### 2.3.1 Unsigned addition
#### Aside Security vulnerability in getpeername
it is designed to copy some of the data maintained by the operating system kernel to a designated region of memory accessible to the user.
Most of the data structures maintained by the kernel should not be readable by a user,
since they may contain sensitive information about other users and about other jobs running on the system.
but the region shown as kbuf was intended to be one that the user could read.
suppose, however, that some malicious programmer writes code that calls copy_from_kernel with a negative value of maxlen.

Modular addition forms a mathematical structure known as an abelian group.
It is commutative and associative;
it has an identity element 0, and every element has an additive inverse.

### 2.3.2 Two's-complement addition 
this function should return 1 if arguments x and y can be added without causing overflow.
```c
int tadd_ok(int x, int y) {
    int sum = x + y;
    int neg_over = x < 0 && y < 0 && sum >=0;
    int pos_over = x >= 0 && y >= 0 && sum < 0;
    return !neg_over && !pos_over;
}
```

```c
// determine whether arguments can be subtracted without overflow
int tsub_ok(int x, int y) {
    return tadd_ok(x, -y); // except y is TMin
}
```
One lesson to be learned from this exercise is that TMin should be included as one of the cases in any test procedure for a function.

### 2.3.3 Two's Complement Negation
for w = 4, we have TMin4=-8. So -8 is its own additive inverse,
while other values are negated by integer negation.

#### bit-level representation of two's complement negation
there are several clever ways to determine the two's-complement negation of a value represented at the bit level.

One technique for performing two's-complement negation at the bit level is to complement the bits and then increment the result.

a second way:
splitting the bit vector into two parts.

### 2.3.4 Unsigned multiplication
### 2.3.5 two's complement multiplication
Although the bit-level representations of the full products may differ, those of the truncated products are identical.

you are given the assignment to develop code for a function `tmult_ok` that will determine whether two arguments can be multiplied without causing overflow.


1. we know that x*y can be written as a 2w-bit two's-complement number. 

### 2.3.6 Multiplying by constants
Historically, the integer multiply instruction on many machines was fairly slow, requiring 10 or more clock sycles, whereas other integer operations - such as addition, subtraction, bit-level operations, and shifting--required only 1 clock cycle.

In Chapter 3, we will see many examples of the LEA instruction in action.
the instruction is provided to support pointer arithmetic, but the C compiler often uses it as a way to perform multiplication by small constants.
for each value of k, we can compute two multiples:
2k and 2k+1.
Thus, we can compute multiples 1,2,3,4,5,6,and 9.

we can view the bit pattern [11011] as having a run of 6 ones with a zero in the middle, and so we apply the rule for form B, but then we subtract the term corresponding to the middle zero bit.

How should the compiler decide which form to use?
Assuming that addition and subtraction have the same performance, the rule is to choose form A when n=m, either form when n = m + 1, and form B when n > m+1.
the justification for this rule is as follows.
when n = m, form A requires only a single shift, 

### 2.3.7 dividing by powers of 2
integer division on most machine is even slower than integer multiplication--requiring 30 or more clock cycles.
diving by a power of 2 can also be performed using shift operations, but we use a right shift rather than a left shift. 
the two different right shifts-logical and arithmetic-serve this purpose for unsigned and two's-complement numbers, respectively.
interger division always rounds toward zero. 
to define this precisely,
let us introduce some notation.

the case for using shifts with unsigned arithmetic is straightforward, in part because right shifting is guaranteed to be performed logically for unsigned valuse.

#### principle unsigned division by a power of 2
for C variables x and k with unsigned values x and k, such that 0<=k < w, the C expression x >> k yields the value x/2k.
as examples, figure 2.28 shows the effects of performing logical right shifts on a 16-bit representation of 12340 to perform division by 1,2,16,256. 

#### derivation
the examples illustrate that arithmetic right shift is similar to division by a power of 2, except that it rounds down rather than toward zer.

the case for dividing by a power of 2 with two's-complement arithmetic is slightly more complex. First, the shifting should be performed using an arithmetic right shift, to ensure that negative values remain negative.
#### principle: two's-complement division by a power of 2, rounding down
Let C variables x and k have two's-complement value x and unsigned value k, respectively, such that 0 <= k < w. 
the C expression x >> k, when the shift is performed arithmetically, yields the value [x/2k]

for x >= 0, variable x has 0 as the most significant bit, and so the effect of an arithmetic shift is the same as for a logical right shift.
thus, an arithmetic right shift by k is the same as division by 2k for a nonnegative number.
as an example of a negative number, Figure 2.29 shows the effect of applying arithmetic right shift to a 16-bit representation of -12,340for different shift amounts.
for the case when no rounding is required (k=1), the result will be x/2k. 
when rounding is required, shifting causes the result to be rounded downward.
for example, the shifting right by four has the effect of rounding -771.25 down to -772. we will need to adjust our strategy to handle division for negative values of x.

#### derivation: two's complement division by a power of 2, rounding down
we can correct for the improper rounding that occurs when a negative number is shifted right by "biasing" the value before shifting.

#### principe: two's-complement division by a power of 2, rounding up
Figure 2.30 demonstrates how adding the appropriate bias before performing the arithmetic right shift causes the result to be correctly rounded.
we can see that the bits to the left of these may or may not be incremented.
For the case where no rounding is required, adding the bias only affects bits that are shifted off.
for the cases where rounding is required, adding the bias causes the upper bits to be incremented, so that the result will be rounded toward zero.

write a function `div16` that returns the value x/16 for integer argument x.
your function should not use division, modulus, multiplication, any conditionals (if or ?:),
any LOOPS.
```c
int div16(int x) {
    /* compute bias to be either 0 (x >= 0) or 15 (x < 0>) */
    int bias = (x >> 31) & 0xF;
    return (x + bias) >> 4;
}
```
the only challenge here is to compute the bias without any testing or conditional operations.
we use the trick that the expression x >> 31 generates a word with all ones if x is negative,
and all zeros otherwise. 
by masking off the appropriate bits, we get the desired bias value.

we now see that division by a power of 2 can be implemented using logical or arithmetic right shifts.
this is precisely the reason the two types of right shifts are available on most machines.
Unfortunately, this approach does not generalize to division by arbitrary constants.
unlike multiplication, we cannot express division by arbitrary constants K in terms of division by powers of 2.

### 2.3.8 Final thoughts on integer arithmetic
the "integer" arithmetic performed by computers is really a form of modular arithmetic. 
the finite word size used to represent numbers limits the range of possible values, and the resulting operations can overflow.
we can also seen that the two's-complement representation provides a clever way to represent both negative and positive values, while using the same bit-level implementations as are used to perform unsigned arithmetic - operations such as addition, subtraction, multiplication, and even division have either identical or very similar bit-level behaviors, whether the operands are in unsigned or two's-complement form.

<!-- PP 2.44 not finished -->

## 2.4 floating point
a floating-point representation encodes rational numbers of the form V = x * 2y.

### 2.4.1 fractional binary numbers
### 2.4.2 IEEE floating-point representation
V = (-1)^s * M * 2^E
- the sign s determines whether the number is negative or positive, where the interpretation of teh sign bit for numeric value 0 is handled as a special case.
- the significand M is a fractional binary number that ranges either between 1 and 2-e or between 0 and 1-e
- the exponent E weights the value by a power of 2.

the bit representation of a floating-point number is divided into three fields to encode these values:
- the single sign bit s directly encodes the sign s.
- the k-bit exponent field exp = ek-1 ...encodes the exponent E
- the n-bit fraction field frac = encodes the significand M, but the value encoded also depends on whether or not the exponent field equals 0.

#### case 1: normalized values
this is the most common case. it occurs when the bit pattern of exp is neither all zeros(numeric value 0) nor all ones(numeric value 255 for single precision, 2047 for double).
in this case,
the exponent field is interpreted as representing a signed integer in biased form.
the exponent value is E=e-bias, where e is the unsigned number having bit representation ek-1 ... e1e0 and bias is a bias value equal to 2k-1 - 1.

why set the bias this way for denormalized values?
having the exponent value be 1-Bias rather that simply -Bias might seem counterintuitive.
we will see shortly that it provides for smooth transition from denormalized to normalized values.

the fraction field frac is interpreted as representing the fractional value f, where 0<=f <1,
with the binary point to the left of the most significant bit.
the significand is defined to be `M = 1+f`.
this is sometimes called an implied leading 1 representation, because we can view M to be the number with binary representation 1.
this representation is a trick for getting an additional bit of precision for free, since we can always adjust the exponent E so that significand M is in the range 1 <=M < 2

#### case 2: denormalized values
when the exponent field is all zeros, the represented number is in denormalized form.
in this case, the exponent value is E=1-bias, and the significand value is M = f, the value of the fraction field without an implied leading 1.
Denormalized numbers serve two purposes. 
- First, they provide a way represent numberic value 0,
    since with a normalized number we must always have M >=1, and hence we cannot represent 0.
    In fact, the floating-point representation of +0.0 has a bit pattern of all zeros: 
    the sign bit is 0. the exponent field is all zeros, and the fraction field is all zeros, giving M =f - 0.
    curiously, when the sign bit is 1, but the other fields are all zeros, we get the value -0.0.
- a second function of denormalized numbers is to represent numbers that are very close to 0.0. they provide a property known as gradual underflow in which possible numeric value are spaced evenly near 0.0.

#### case 3: special values
when exponent field is all ones.
when the fraction field is all zeros, the resulting values represent infinity, either +oo when
infinity can represent results that overflow, as when we multiply two very large numbers, or when we divide by zero.
when the fraction field is nonzero, the resulting value is called a NaN, short for "not a number".
such 