(Night road - Manwe)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkchILEgVIYWlrdRiAgICAgLnUCgw)

: frac dup floor - ;
: x u ;
: y v ;

1 y 2 * -
x y 2 * 8 ** t 9 / sin * + 0.5 -
abs
2dup
f>
0.5 y - *
-rot

45 *
f>
3 1 y 1.8 * - /
t frac 6 *
+ floor 2 mod 1 y 1.8 * - * 4 /
*
+

dup 0 f= 0.5 y - * 0.75 **

y 0.5 f< *
over + over

1
x
t 9 / cos
+
12 * sin 30 / y 0.7 - f< *
+

1.5 y 1.3 * - y 0.5 f> * -