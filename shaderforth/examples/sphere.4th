: px x 2 * rx - ry / ;
: py y 2 * ry - ry / ;
: e 0.0005 ;

vec3 ta
vec3 ro

vec3 cw
vec3 cp
vec3 cu
vec3 cv
vec3 rd
vec3 col
vec3 basecol
vec3 tmp1
vec3 tmp2
vec3 cen
vec3 tmp3
vec3 e1    
vec3 e2        
vec3 e3
vec3 pos
vec3 norm
vec3 sun_lig   
vec3 sun_hal 
vec3 sun_col
vec3 sky_col
vec3 bounce_col
vec3 lin
vec3 mix_col

\vec3 ref

e 0 0 e1 vec3!
0 e 0 e2 vec3!
0 0 e e3 vec3!

0.6 0.35 0.5 sun_lig vec3!
sun_lig sun_lig normalize3
8.1 6 4.2 sun_col vec3!
0.5 0.7 1 sky_col vec3!
0.6 0.4 0.3 bounce_col vec3!
0.5 0.7 0.9 mix_col vec3!

0 value s 
0 value i
100 value h

t 0.9 * value time

: fract 
dup floor -
;

: sdSphere (in vec3 p, in float s)
swap length3 swap -
;

: map (in vec3 pos)
\ ball
dup
time fract dup 1 swap - * 4 * 0.1 + 
0 swap 0 cen vec3!
cen tmp3 sub3

tmp3 0.25 sdSphere

\ ground
swap #1 cells #+ @ 0.1 +  (spheredist pos #32 @ 0.1 +)
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

: castRay (in vec3 ro, vec3 rd)
0.5 to s
0 to i
100 to h

begin

i 100 < (test)
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

s 20 f>
negate dup 1 + s * +
;


: render

\ sky dome
rd #1 cells #+ @ 0 max -0.5 *
dup dup
0.5 + -rot 0.8 + swap 0.9 +
basecol vec3!

ro rd castRay

dup 0 > if

dup rd pos scalarmul3  (castray value is stack top, need to duplicate to validate balancing in if statement, pos = s*rd)
ro pos pos add3 (pos = pos + ro)

pos calcnormal

\rd norm ref reflect3

0.15 0.15 0.15 col vec3!

\ lighting
norm sun_lig dot3 0 1 clamp  \ sun_dif
value sun_dif

sun_lig rd sun_hal sub3
sun_hal sun_hal normalize3

0.001 norm tmp2 scalarmul3 pos tmp2 tmp2 add3 
tmp2 sun_lig castRay 0 step \ sun_sha
value sun_sha



norm sun_hal dot3 0 1 clamp 8 ** sun_dif * sun_hal rd dot3 1 + 0 1 clamp 5 ** 0.96 * 0.04 + * \sun_spe
value sun_spe

norm #1 cells #+ @ 0.5 * 0.5 + 0 1 clamp sqrt \ sky_dif
value sky_dif

norm #1 cells #+ @ -0.9 * 0.1 + 0 1 clamp sqrt pos #1 cells #+ @ -0.1 * 1 + 0 1 clamp * \bounce_dif
value bounce_dif

sun_dif sun_sha * sun_col lin scalarmul3
sky_dif sky_col tmp2 scalarmul3
lin tmp2 lin add3
bounce_dif bounce_col tmp2 scalarmul3
lin tmp2 lin add3

col lin col mul3

sun_spe sun_sha * sun_col tmp2 scalarmul3
col tmp2 col add3

1 over 3 ** -0.0001 * exp -

col mix_col rot col mix3



else
\ do nothing, use basecol
then

(merge basecol and col)

dup 0 f> col col scalarmul3
dup 0 f<= basecol basecol scalarmul3
basecol col col add3
;


: main


\ camera
mz abs rx / 10.57 *
dup cos 1.3 * -0.25 rot sin 1.3 * ro vec3!
0 0.65 0.4 ta vec3!
ta ro ro add3

\ frame
ta ro cw sub3
cw cw normalize3
\0 1 0 cp vec3!
cw here @ 0 , 1 , 0 , cu cross3
cu cu normalize3
cu cw cv cross3

px cu tmp1 scalarmul3
py cv tmp2 scalarmul3
tmp1 tmp2 tmp1 add3
1.8 cw tmp2 scalarmul3
tmp1 tmp2 rd add3 
rd rd normalize3

render

col @ 0.4545 **
col #1 cells #+ @ 0.4545 ** 
col #2 cells #+ @ 0.4545 **
;




main


