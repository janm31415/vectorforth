: qx u 0.33 - ;
: qy v 0.7 - ;

: dist-to-center qx dup * qy dup * + sqrt ;

: r 0.2 0.1 qy qx atan2 10 * qx 20 * + 1 + cos * +;

: factor r 0.01 r + dist-to-center smoothstep ;


: r2 0.01 120 qy * cos 0.002 * + -40 v * exp +;

: factor2 1 
          1 r2 0.01 r2 + qx 0.25 2 qy * sin * - abs smoothstep - 
          1 0 0.1 qy smoothstep - 
          * -  ;

factor factor2 *
dup dup
0.4 0.8 v sqrt mix * swap
0.1 0.3 v sqrt mix *
