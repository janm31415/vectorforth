(blue ball interactive - anonymous)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkchILEgVIYWlrdRiAgICA67aACQw)

: x u rx ry / *;
: y v ;
: mx mx ry / ;
: my my ry / ;

: d dup ;
: x0 x 2 * 1 - mx 0.5 - 1.8 * - ;
: y0 y 2 * 1 - ;
: n x0 d * y0 d * + 4 + sqrt ;
: m swap / * d * -rot ;
: l 2dup
2dup x0 m 2dup y0 m -2 my .5 - 3 * - swap / * 3 + d *
+ + sqrt 1 - y0 1 + min
rot + swap ;
0
n
l l l l l l l l l l l l l l
drop
5 / d .3 * d rot
