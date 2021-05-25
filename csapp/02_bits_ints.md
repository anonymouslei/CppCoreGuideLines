
&, |, ^, ~ (ampersand, vertival bar, caret and tilde)
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