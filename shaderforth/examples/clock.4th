\ https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkchILEgVIYWlrdRiAgIDAuKGMCgw

\ clock1

: x u  rx * ry / 0.4 -;
: y v ;

variable zero
variable one

\ angle range changed from [0,2pi] to [0,1]
: 2pi 2 pi * ; : ccos 2pi * cos ; : ssin 2pi * sin ;
: aatan2 atan2 2pi / 1 mod ;

\ the distance from X,Y to any point x,y
: r' ( X Y -- radius ) x rot - 2 ** y rot - 2 ** + .5 ** ;
: r .5 .5 r' ;

\ the angle from X,Y to any point x,y
: a' ( X Y -- angle ) y swap - x rot - aatan2 ;
: a .5 .5 a' ;

\ the angle for n seconds
: s ( #seconds -- angle ) globaltime floor swap / ;

\ the angle for n minutes
: m ( #minutes -- angle ) 60 s swap / ;

\ the angle for n hours
: h ( #hours -- angle ) 60 m swap / ;

\ the virtual needle for given angle
: v_needle ( angle -- needle ) a + .25 - 1 mod 1 swap - ;

\ the real needle for given angle
: r_needle ( angle -- needle ) a +
dup 1 mod zero ! dup ccos r * abs 1 swap - 150 **
swap ssin r * -.08 f> *
;

\ the needle for given angle and radius
: within ( n a b -- a<n<b ) push over pop f< push f> pop * ;
: n ( angle radius -- needle ) dup one !
r .008 rot within swap r_needle *
r zero @ .25 - ccos * one @ .05 - - 2 **
r zero @ .25 - ssin * 2 ** + sqrt
.025 - abs 1 swap - 499 ** +
;

\ the n ticks to form a whole ring
: t ( #ticks/circle -- ticks ) 2 / a * ccos 199 ** r .41 .48 within * ;

\ the red needle for the hours and seconds and 4 ticks
12 h .23 n 60 s .37 n + 4 t +
\ the green needle for the hours and minutes and 12 ticks and 60 ticks
12 h .23 n 60 m .30 n + 12 t + 60 t +
\ the blue 60 ticks and background
60 t r 8 * floor 4 / +

\ cut out the clock
r .5 f< *