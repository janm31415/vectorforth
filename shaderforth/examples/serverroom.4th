(server room - www.manwe.ru)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkchILEgVIYWlrdRiAgICA0JCRCQw)

: x u ;
: y v ;
: fract dup floor - ;
: time t 7 / 8.96 + sin .8 * ;
: q dup * ;
: sun 1 x .5 - q y .7 - q + sqrt - 1.2 * ;
: flat
x .5 - * time r> / +
* pi * 2 * cos 0 max 8 * ;
: begin
dup 2dup >r >r >r
.2 swap / 1 + flat ;
: server
dup
begin
dup -rot - 0 max swap 1 min + .8 *
r> 1 flat - 0 max
;
: lamps
( mask )
dup 2dup >r >r
begin
r> 1 flat +
dup -rot - 0 max -rot - 0 max
r> dup
time over / x .5 - +
* fract .5 - abs .34 f>
( lamps )
r> dup dup 2dup >r >r >r
x + * 1 + time + 44 * floor
y r> * 44 * 1 + floor 199.8347 / **
t 1125.4 + 417.731 / * sin 237.371 * dup floor - q
x r> * time + 44 * pi * sin abs
y r> * 44 * pi * sin abs * *
* q
dup >r >r
y * 44 * floor
dup 9 mod 2 f< r> *
swap 9 mod 4 f= r> *
-rot + -rot +
;
0 0
2 lamps
1.5 lamps
1 lamps
.5 lamps
over q + swap
.8
3 server
2.5 server
2 server
1.5 server
1 server
.5 server
sun *
dup q rot + swap