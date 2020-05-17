: px x 2 * rx - ry / ;
: py y 2 * ry - ry / ;
: e 0.0001 ;

vec3 ro    vec3 rd        vec3 pos
vec3 e1    vec3 e2        vec3 e3
vec3 norm  vec3 sun_dir
vec3 col   vec3 sun_col   vec3 sky_col
vec3 tmp1  vec3 tmp2

0 value s \ contains output value of castray
0 value i
100 value h

e 0 0 e1 vec3!
0 e 0 e2 vec3!
0 0 e e3 vec3!

0.8 0.4 0.2 sun_dir vec3!
sun_dir sun_dir normalize3

1 0.8 0.6 sun_col vec3!
0 0.05 0.2 sky_col vec3!

0 0 0 col vec3!

: map (in vec3 pos)
  dup
  length3 0.25 - 
  swap
  #32+ @
  0.25 +
  min
;

: calcnormal (in vec3 pos)
dup e3 norm sub3 norm map >r
dup e3 norm add3 norm map r> - >r
dup e2 norm sub3 norm map >r
dup e2 norm add3 norm map r> - >r
dup e1 norm sub3 norm map >r
    e1 norm add3 norm map r> - r> r> norm vec3! 
norm norm normalize3
;


: castray (in vec3 ro, vec3 rd)
0 to s
0 to i
100 to h

begin

i 100 <  (test)
h 0.001 >=
s 20.0 <
and and

while

2dup (duplicate ro and rd)
s swap tmp1 scalarmul3  (rd is top item on stack, swap with s, tmp1 = s*rd)
tmp1 tmp1 add3 (ro is top item on stack, tmp1 = tmp1 + ro)

tmp1 map to h
h 0.001 f>= h * s + to s
i 1 + to i

repeat

2drop (drop ro and rd)

s 20 > (put result on the stack)
if
-1
else
s
then
;

0.0 0.0 1.0 ro vec3!
px py -1.5 rd vec3!
rd rd normalize3

ro rd castray

dup 0 > if

dup rd pos scalarmul3  (castray value is stack top, need to duplicate to validate balancing in if statement, pos = s*rd)
ro pos pos add3 (pos = pos + ro)

pos calcnormal

norm sun_dir dot3 0 1 clamp over 0 f> *            \ sun diffuse

0.001 norm tmp2 scalarmul3 pos tmp2 tmp2 add3 
tmp2 sun_dir castray 0 step *                        \ shadow

sun_col col scalarmul3

norm #32+ @ 0.5 * 0.5 + 0 1 clamp over 0 f> *      \ sky diffuse
sky_col norm scalarmul3                            \ reusing norm now as temporary
col norm col add3


else

\ do nothing

then

drop \ drop raycast value

col @ 0.4545 **
col #32+ @ 0.4545 ** 
col #64+ @ 0.4545 **


