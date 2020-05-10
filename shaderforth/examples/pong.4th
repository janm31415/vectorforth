(PONG game - manwe)
(https://forthsalon.appspot.com/haiku-view/ahBzfmZvcnRoc2Fsb24taHJkcg0LEgVIYWlrdRjG_xQM)

: x u rx ry / * rx ry / 1 - 2 / -;
: y v ;

: d dup ;
: m 0.4 * 0.5 + - ;
: a y - abs 0.1 f< x 0 f> * x 1 f< * x;
: r t swap / d floor - 1.96 * 0.98 - d 0 f> 2 * 1 - * 0.01 + ;
: e 2.9 r 0.94 * 0.03 + ;
: f 3.7 r ;
: p f 0.9 * 0.05 + d t ;
e x - d * f y - d * + sqrt 0.02 f<
p 3 * sin m e * - a 0.03 f< * 
p 2 * cos m 1 e - * - a 0.97 f> * 
+ + d d