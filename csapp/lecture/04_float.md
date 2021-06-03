# floating point

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

