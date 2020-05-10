(Planet of Zoom - www.manwe.ru)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkchILEgVIYWlrdRiAgICAoJnwCww)

( greetings to BradN, Boomlinde, Digimind, DarkstarAG, Ivanq, Vort, Frag_, Stainless, demoscene.ru )

: x u rx * ry / ;
: y v;
: random t v8 123 234523 1230 23 1324 34069 9854 345 * sin y * 341234.234 * dup floor -  ;
: zoom t 5 / sin 10 / ;
: xx x .85 - .8 zoom - * t cos 5 / negate + ;
: yy y .3 - .8 zoom - * zoom + ;
: a t sin 3 / + ;
: line
2dup sin * negate xx + swap a tan * swap yy + swap - 160 * 1 min 0 max ;
: wings
.025 .12 line .025 -.12 line *
-.025 .12 line - -.025 -.12 line - ;
: flaps
0.045 .8 line 0.006 .4 line -
0.045 -.8 line 0.006 -.4 line -
+ + 0 max ;
: tail
-0.057 0 line
xx .065 - yy atan2 a 0 f> -
xx .065 + yy atan2 a 0 f< -
0 max +
xx .0065 + yy atan2 a 55 * 1 min 0 max
xx .0065 - yy atan2 a 55 * 1 min 0 max -
+ ;
: cut
1 -0.064 0 line - *
0.019 0 line * ;
: circle
dup 0 a cos * xx - 2 ** swap 0 a sin * yy - 2 ** + - 3999 * 0 max 1 min ;
: engine
0.0008 .05 circle + 0.0008 -.05 circle + ;
: fire
0.0002 random 8000 / + dup >r
.05 circle
r> -.05 circle + 5 * ;
: run
4 1 y 1.3 * - /
t dup floor - 6 * +
floor 2 mod
dup y 1.6 * + 1 min .6 *
swap 0 f= y 1.6 * + 1 min .5 * ;
: sun
1 x .9 - 2 ** y .94 - 2 ** + .2 ** - ;

run sun +
wings flaps tail cut engine

dup -rot - -rot - dup 0 f>

fire dup dup >r >r + rot r> + rot r> .65 * +

0 max .27 ** dup .05 f< .36 * +