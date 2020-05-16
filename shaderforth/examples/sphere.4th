: px x 2 * rx - ry / ;
: py y 2 * ry - ry / ;
: e 0.0001 ;

vec3 ro    vec3 rd        vec3 pos
vec3 e1    vec3 e2        vec3 e3
vec3 norm  vec3 sun_dir
vec3 col   vec3 sun_col   vec3 sky_col

e 0 0 e1 vec3!
0 e 0 e2 vec3!
0 0 e e3 vec3!

0.8 0.4 0.2 sun_dir vec3!
sun_dir sun_dir normalize3

1 0.7 0.5 sun_col vec3!
0 0.2 0.4 sky_col vec3!

0 0 0 col vec3!

: map length3 0.5 - ;

: calcnormal 
dup e3 norm sub3 norm map >r
dup e3 norm add3 norm map r> - >r
dup e2 norm sub3 norm map >r
dup e2 norm add3 norm map r> - >r
dup e1 norm sub3 norm map >r
    e1 norm add3 norm map r> - r> r> norm vec3! 
norm norm normalize3
;

0.0 0.0 2.0 ro vec3!
px py -1.5 rd vec3!
rd rd normalize3

0 value s
0 value i
100 value h

begin

i 100 <  (test)
h 0.001 >
s 20.0 <
and and

while

s rd pos scalarmul3  (pos = s*rd)
ro pos pos add3 (pos = pos + ro)

pos map to h
h s + to s
i 1 + to i

repeat

s 20 < if

s rd pos scalarmul3  (pos = s*rd)
ro pos pos add3 (pos = pos + ro)

pos calcnormal

norm sun_dir dot3 0 1 clamp   \ sun diffuse
sun_col col scalarmul3

norm #32+ @                   \ sky diffuse
sky_col norm scalarmul3       \ reusing norm now as temporary
col norm col add3


else

\ do nothing

then

col @ col #32+ @ col #64+ @


