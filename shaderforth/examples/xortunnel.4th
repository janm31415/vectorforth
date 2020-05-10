(xor tunnel - boomlinde)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkcg0LEgVIYWlrdRii4xMM)

: x' u 0.5 - t sin 0.2 * + ;
: y' v 0.5 - t 1.5 * cos 0.2 * + ;
: dist r> dup >r ;
: at2 r@ ;
: xor + abs 2 mod ;
: b / floor 2 mod ;
: m 256 * floor ;
: a dup rot swap b -rot b xor ;
: w dup

at2 pi / 512 * t 100 * + 256 mod
128 dist / t 500 * + 256 mod

rot a * ;


x' y' 2dup atan2 >r 
dup * swap dup * + sqrt >r  

1 w 2 w 4 w 8 w 16 w 32 w 64 w 128 w
+ + + + + + + 256 / dist * dup dup r> r> drop drop