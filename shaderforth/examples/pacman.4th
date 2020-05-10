(PACMAN - wwww.manwe.ru)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkcg0LEgVIYWlrdRiYlRMM)

: x u rx * ry /;
: y v ;
: d dup ;
: m 1 min ;
: f d floor - ;
: c cos abs ;
: j t 4 + 2 * x 8 * floor 8 / + 4 * c 2 / t 4 + 2 / c 4 ** * - ;
: a 1 x x 8 * floor 0.5 + 8 / - d * y ;
: b - d * + sqrt 50 * 8 ** ;
: p x t 4 + pi / f 2.2 * - 0.2 + ;
: v t 4 + pi 2 * / f ;
a j 0.5 b -
v d 0.5 f< * 4 * m *
1 p d * y 0.5 - d * + 36 * 30 ** m -
y 0.5 - p atan2 abs t 10 * c 0.8 * - 16 * m * 0 max
a 0.5 b - 0 max d p 16 * f< * +
p d * y 0.58 b m *
v 0.5 f>= *
+ d 0.2