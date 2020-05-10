(heart - rafuck)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkchILEgVIYWlrdRiAgICAwOHKCww)

: x u ;
: y v ;

: ' .5 - 5 * ;
: sq dup * ;
: ^ ' t pi * .7 * sin sq -.5 * exp * ;
: quad dup sq * ;
: heart x ^ sq y ^ 3 * 2.5 1 - + 3 / x ^ abs sqrt - sq + 1 - ;
: ish heart 0.0000001 f< ;
: noh heart 0.0000001 f>= ;
: dh heart abs ;
noh -3 dh * exp * ish +