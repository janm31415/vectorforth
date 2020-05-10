(3D wire torus - Manwe)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkchILEgVIYWlrdRiAgICAvoKDCQw)

: x u rx * ry /;
: y v ;
: q dup * ; ( was originally 2 **, but dup * is much faster)
0 0
4 push
: l
x .8 - r@ 1 + *
r@ 3 - t 1.9 / cos t 1.9 / sin z*
y .5 - r@ 1 + *
swap
t 2.7 / cos t 2.7 / sin z*
-rot
q swap q +
dup
8 *
swap
rot
q + 1.8 + q
-
abs .03 f<
r@ /
pop .2 - push
max
;
: j l l l l ; j j j j
pop drop
1.5 * dup dup * swap