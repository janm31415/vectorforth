(Vectrex fix - Manwe)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkchILEgVIYWlrdRiAgICAiOiUCww)

( inspired by "Web Wars" game on Vectrex console )
( let's discuss Forth Haiku on demoscene.ru forum )

: t t 11 + ;
: d dup ;
: fract d floor - ;
: n 6 ;
: x u;
: y v;
: xx x .5 - ;
: yy y .5 - ;
: xw x .5 - 10 * ;
: yw y .37 - t 2.5 * 1.1 - sin 30 / - 12 * ;
: dx t n / floor sin 2 * ;
: dy t n / floor cos 2 * ;
: zoom 1 1 t n / fract - 50 * 1 + / ;
: phase t fract pi * 2 * ;
: wings
1
xw cos t 2.5 * sin *
xx abs .5 + *
yw - abs
-
1 xx 2.3 * abs - sqrt * 0 max
8 **
d >r
20 * 4 ** 1 min
max
r> 9 * 4 ** 1 min -
;

5
xx d * yy d * + sqrt
d -rot
/
phase + sin abs
over 9 * 4 ** **

swap .15 max .15 - * 4 *

1
zoom
dx over * xx - d *
over dy * yy - d *
+ sqrt swap 2dup >r >r
2 / -
-
abs 200 ** 1 min
max
1 r> r> 2.2 / - -
abs 200 **
- 0 max 1 min
wings
d d