readme.txt

#This is a simple GUI calculator made in Qt Creator 5 using the ExprTk library (C++ Mathematical Expression Library) found here: http://www.partow.net/programming/exprtk/index.html

It can be compiled in Qt Creator using qmake.

Basic instructions:

Use the keypad to type in a valid math expression and push "=" to evaluate it.

(+/-) is to change signs from pos to neg, press again to go back to pos.

pi value is derived from library math.h M_PI but ExprTk truncates it to 5 decimal places which is good enough for testing.  

For unary operators please put "(" and ")" around the operand except of ^2 which is square and used like:

2^2 which evaluates to 4.

Examples below are taken from official Github repo of ExprTk at 
https://github.com/ArashPartow/exprtk/blob/master/readme.txt

| abs | Absolute value of x. (eg: abs(x))

| avg | Average of all the inputs.|
|| (eg: avg(x,y,z,w,u,v) == (x + y + z + w + u + v) / 6) |

| log | natural log of x

| logn | Base N logarithm of x. where n is a positive integer. 
|| (eg: logn(x,8))|

| exp | e to the power of x. (eg: exp(x))

| mul| Product of all the inputs.|
|| (eg: mul(x,y,z,w,u,v,t) == (x * y * z * w * u * v * t)) |

| pow | x to the power of y. (eg: pow(x,y) == x ^ y)

| sqrt | Square root of x, where x >= 0. (eg: sqrt(x)) |


Trigonometry functions:

All are evaluated in radians.

| acos | Arc cosine of x expressed in radians. Interval [-1,+1] |
|| (eg: acos(x))

| asin | Arc sine of x expressed in radians. Interval [-1,+1] |
|| (eg: asin(x))

| atan | Arc tangent of x expressed in radians. Interval [-1,+1] |
|| (eg: atan(x)) |

| atan2 | Arc tangent of (x / y) expressed in radians. [-pi,+pi] |
|| eg: atan2(x,y)
 
| cos | Cosine of x. (eg: cos(x))

| sin | Sine of x. (eg: sin(x))

| tan | Tangent of x. (eg: tan(x))

To convert between radians and degrees.  

| deg2rad | Convert x from degrees to radians. (eg: deg2rad(x)) |

| rad2deg | Convert x from radians to degrees. (eg: rad2deg(x)) |

Remaining Issues:

Ability to save answers to memory so operations can be performed on them.
Better button layout.
Ability to switch layout from basic to scientific.
Blinking cursor.

 
 

 
 




