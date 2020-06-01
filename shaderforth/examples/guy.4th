( Based on Step #2 of the LIVE Shade Deconstruction tutorial for "Happy Jumping"
  by inigo quilez
  License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
  
  https://www.youtube.com/watch?v=Cfe5UQ-1L9Q

  Step 2: https://www.shadertoy.com/view/3ljSzw
)


: px x 2 * rx - ry / ;
: py y 2 * ry - ry / ;
: e 0.0005 ;

vec3 ta
vec3 ro

vec3 cw
vec3 cu
vec3 cv
vec3 rd
vec3 col
vec3 basecol
vec3 tmp1
vec3 tmp2
vec3 tmp3
vec3 tmp4
vec3 tmp5
vec3 tmp6
vec3 cen
vec3 q
vec3 r
vec3 rad
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
vec3 body_col
vec3 terrain_col
vec3 iris_col
vec3 eyeball_col

vec2 mapres
vec2 castres
vec2 tmpres
vec2 uu
vec2 vv

e 0 0 e1 vec3!
0 e 0 e2 vec3!
0 0 e e3 vec3!

0.6 0.35 0.5 sun_lig vec3!
sun_lig sun_lig normalize3
8.1 6 4.2 sun_col vec3!
0.5 0.7 1 sky_col vec3!
0.6 0.4 0.3 bounce_col vec3!
0.5 0.7 0.9 mix_col vec3!
0.2 0.05 0.02 body_col vec3!
0.05 0.09 0.02 terrain_col vec3!
0.4 0.4 0.4 iris_col vec3!
0 0 0 eyeball_col vec3!

0 value s 
0 value i
100 value h
0 value m

t 0.9 * value time

: fract 
dup floor -
;

: smin \ (a b k)
-rot 2dup \ (k a b a b)
- abs \ (k a b |a-b|)
-rot \ (k |a-b| a b)
min \ (k |a - b| min(a,b))
-rot \ ( min(a,b) k |a-b|)
over -rot - 0 max \ ( min(a,b) k  max(k-|a-b|, 0) )
dup 0.25 * * swap / -
;

: sdEllipsoid (in vec3 pos, in vec3 rad)
tuck (rad pos rad)
tmp4 div3  (rad)
tmp4 length3 (rad k0)
swap tmp4 swap (k0 tmp4 rad)
tmp4 div3 tmp4 length3 (k0 k1)
swap dup (k1 k0 k0)
1 - * swap /
;

: sdSphere (in vec3 p, in float s)
swap length3 swap -
;

: map (in vec3 pos)

dup
time fract dup dup 1 swap - * 4 * value p
2 * 1 - -4 * value pp

0 p 0.1 + 0 cen vec3!

\ body

1 pp negate uu vec2!
uu uu normalize2
uu #1 cells #+ @ negate uu @ vv vec2!

0 0.4 p smoothstep \ smoothstep(0.0,0.4,p)
p 0.5 * 0.5 + \ 0.5 + 0.5*p
over * swap 1 - - \  smoothstep(0.0,0.4,p) * (0.5 + 0.5p) + (1 - smoothstep(0.0,0.4,p))
value sy
1 sy / value sz


cen r sub3

r @   uu r #1 cells #+ dot2  vv r #1 cells #+ dot2 q vec3!

0.25 0.25 sy * 0.25 sz * rad vec3!

q rad sdEllipsoid 2 mapres vec2!

time 0.8 + fract 6.2831 * cos -0.5 * 0.5 + -0.2 * 0.05 + r #2 cells #+ @ + r #2 cells #+ !
0.2 sy * 0.2 - r #1 cells #+ @ + r #1 cells #+ !

\ head

0 0.2 0.02 tmp5 vec3!
r tmp5 tmp6 sub3
0.08 0.2 0.15 rad vec3!
tmp6 rad sdEllipsoid
0 0.21 -0.1 tmp5 vec3! 
r tmp5 tmp6 sub3
0.2 0.2 0.2 rad vec3!
tmp6 rad sdEllipsoid
0.1 smin
mapres @ 0.1 smin mapres !


\ ground
#1 cells #+ @ 0.1 +  (spheredist pos #32 @ 0.1 +)
1 tmpres vec2!

mapres tmpres tmpres @ mapres @ f< mapres mix2

mapres
;

: calcnormal (in vec3 pos)
dup e3 norm sub3 norm map @ >r
dup e3 norm add3 norm map @ r> - >r
dup e2 norm sub3 norm map @ >r
dup e2 norm add3 norm map @ r> - >r
dup e1 norm sub3 norm map @ >r
    e1 norm add3 norm map @ r> - r> r> norm vec3! 
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

tmp1 map dup @ to h #1 cells #+ @ to m
h 0.001 f>= h * s + to s
i 1 + to i


repeat

2drop (drop ro and rd)

s m castres vec2!
-1 -1 tmpres vec2!

castres tmpres s 20 f> castres mix2

castres
;


: render

\ sky dome
rd #1 cells #+ @ 0 max -0.5 *
dup dup
0.5 + -rot 0.8 + swap 0.9 +
basecol vec3!

ro rd castRay dup #1 cells #+ @ swap @ \ (m s)

dup 0 > if \ if s > 0

dup rd pos scalarmul3  (castray value is stack top, need to duplicate to validate balancing in if statement, pos = s*rd)
ro pos pos add3 (pos = pos + ro)

pos calcnormal

\0.2 0.2 0.2 col vec3!

over 4 f= col eyeball_col rot col mix3
over 3 f= col iris_col rot col mix3
over 2 f= col body_col rot col mix3
over 1 f= col terrain_col rot col mix3

\ lighting
norm sun_lig dot3 0 1 clamp  \ sun_dif
value sun_dif

sun_lig rd sun_hal sub3
sun_hal sun_hal normalize3

0.001 norm tmp2 scalarmul3 pos tmp2 tmp2 add3 
tmp2 sun_lig castRay @ 0 step \ sun_sha
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


