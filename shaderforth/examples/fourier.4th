(Fourier series - DarkstarAG)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkchILEgVIYWlrdRiAgICA8JOYCgw)

: x u ;
: y v ;

( Make harmonique )
: harm ( a b c d x - a+b*sin[x*d+c] ) * + sin * + ;

( Fourier Series )
0
1 3 t 2 / + 8 t x + harm
.7 2 t 3 * + 3 t 4 / x + harm
.5 3 t 7 * + 17 t x + harm
.7 6 t 11 * + 13 t 13 / x + harm
5 / .5 + y - abs 0.01 - 1 - negate abs dup 4 ** swap 20 **