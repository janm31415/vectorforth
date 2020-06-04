 \  Based on Step #3 of the LIVE Shade Deconstruction tutorial for "Happy Jumping"
 \  by inigo quilez
 \  License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
  
 \  https://www.youtube.com/watch?v=Cfe5UQ-1L9Q

 \  Step 3: https://www.shadertoy.com/view/ttjXDz



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
vec3 hq
vec3 rad
vec3 e1    
vec3 e2        
vec3 e3
vec3 e4
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
vec3 terrain_col2
vec3 iris_col
vec3 eyeball_col
vec3 ear_a
vec3 ear_b
vec3 eye_center
vec3 eyelid_center
vec3 eyelid_rad
vec3 cq
vec3 iris_center
vec3 eyeball_center
mat2x2 rotation

vec2 mapres
vec2 castres
vec2 tmpres
vec2 uu
vec2 vv

0.5773 e * -0.5773 e * -0.5773 e * e1 vec3! 
-0.5773 e * -0.5773 e * 0.5773 e * e2 vec3! 
-0.5773 e * 0.5773 e * -0.5773 e * e3 vec3! 
0.5773 e * 0.5773 e * 0.5773 e * e4 vec3! 

0.6 0.35 0.5 sun_lig vec3!
sun_lig sun_lig normalize3
8.1 6 4.2 sun_col vec3!
0.5 0.7 1 sky_col vec3!
0.6 0.4 0.3 bounce_col vec3!
0.5 0.7 0.9 mix_col vec3!
0.2 0.05 0.02 body_col vec3!
0.05 0.09 0.02 terrain_col vec3!
0.06 0.06 0.02 terrain_col2 vec3!
0.4 0.4 0.4 iris_col vec3!
0 0 0 eyeball_col vec3!

0.15 0.32 -0.05 ear_a vec3!
0.08 0.27 0.06 eye_center vec3!
0.1 0.34 0.08 eyelid_center vec3!
0.06 0.03 0.03 eyelid_rad vec3!
0.08 0.28 0.08 iris_center vec3!
0.075 0.28 0.102 eyeball_center vec3!

0 value s 
0 value i
100 value h
0 value m

t 0.9 * value time

: fract 
dup floor -
;

t 0.9 * fract value t1
t 0.9 * 0.5 * fract 0.5 - abs 0.5 / value t4

t1 1 t1 - * 4 * value p
1 2 t1 * - 4 * value pp

1 pp negate uu vec2!
uu uu normalize2
uu #1 cells #+ @ negate uu @ vv vec2!

0 0.4 p smoothstep \ smoothstep(0.0,0.4,p)
p 0.5 * 0.5 + \ 0.5 + 0.5*p
over * swap 1 - - \  smoothstep(0.0,0.4,p) * (0.5 + 0.5p) + (1 - smoothstep(0.0,0.4,p))
value sy
1 sy / value sz

t4 2 * 1 - -0.25 *
dup
cos
swap
sin
2dup
negate
swap
rotation mat2x2!



: map \ (in vec3 pos)

dup

-1 2 t4 * + 0.5 * 
p 2 p - ** 0.1 +
time floor t1 0.7 ** 1 - + 
cen vec3!

 \ body


cen r sub3



rotation r r mul2x2

r @   uu r #1 cells #+ dot2  vv r #1 cells #+ dot2 q vec3!

0.25 0.25 sy * 0.25 sz * rad vec3!

q rad sdellipsoid 2 mapres vec2!

time 0.8 + fract 6.2831 * cos -0.5 * 0.5 + -0.2 * 0.05 + r #2 cells #+ @ + r #2 cells #+ !
0.2 sy * 0.2 - r #1 cells #+ @ + r #1 cells #+ !

r @ abs r #1 cells #+ @ r #2 cells #+ @ hq vec3!

 \ head

0 0.2 0.02 tmp5 vec3!
r tmp5 tmp6 sub3
0.08 0.2 0.15 rad vec3!
tmp6 rad sdellipsoid
0 0.21 -0.1 tmp5 vec3! 
r tmp5 tmp6 sub3
0.2 0.2 0.2 rad vec3!
tmp6 rad sdellipsoid
0.1 smin
mapres @ 0.1 smin mapres !

 \ belly wrinkles
 
r #1 cells #+ @ 0.02 - r @ dup * 2.5 * - dup >r
120 * sin 0.001 * 1 0 0.1 r> abs smoothstep - * mapres @ + mapres !

 \ ear
time 0.9 + fract dup negate 1 + * 4 * \ p3
dup 0.05 * 0.2 + swap 0.2 * 0.2 + -0.07 ear_b vec3!
hq ear_a ear_b 0.01 0.04 sdstick

mapres @ 0.01 smin mapres !


 \ mouth
0 r @ abs dup * 4 * 0.15 +  0.15 tmp5 vec3!
r tmp5 tmp6 sub3
0.1 0.04 0.2 rad vec3!
tmp6 rad sdellipsoid
negate mapres @ 0.03 smax mapres !

 \ eye
hq eye_center tmp3 sub3 
tmp3 t 2.1 * sin 0.5 * 0.5 + 20 ** 0.02 * 0.065 + sdsphere
mapres @ 0.01 smin mapres !

 \ eyelid
hq eyelid_center cq sub3
cq @ cq #1 cells #+ @  \ (x y)
2dup  \ (x y x y)
0.6 * swap 0.8 * - -rot 0.8 * swap 0.6 * + cq #2 cells #+ @ cq vec3!
cq eyelid_rad sdellipsoid
mapres @ 0.03 smin mapres !

 \ iris
hq iris_center cq sub3
cq 0.06 sdsphere 3 tmpres vec2!
mapres tmpres tmpres @ mapres @ f< mapres mix2

 \eyeball
hq eyeball_center cq sub3
cq 0.0395 sdsphere 4 tmpres vec2!
mapres tmpres tmpres @ mapres @ f< mapres mix2

 \ ground
#1 cells #+ @ 0.1 + \ (spheredist pos #32 @ 0.1 +)
1 tmpres vec2!

mapres tmpres tmpres @ mapres @ f< mapres mix2

mapres
;

: calcnormal \ (in vec3 pos)
dup e1 norm add3 norm map @ >r
dup e2 norm add3 norm map @ >r
dup e3 norm add3 norm map @ >r
    e4 norm add3 norm map @ >r
0 0 0 norm vec3!
r> e4 norm scalarmul3
r> e3 tmp4 scalarmul3
tmp4 norm norm add3
r> e2 tmp4 scalarmul3
tmp4 norm norm add3
r> e1 tmp4 scalarmul3
tmp4 norm norm add3
norm norm normalize3
;

: castRay \ (in vec3 ro, vec3 rd)
0.5 to s
0 to i
100 to h

begin

i 256 < \ test
h abs 0.0005 s * >=
s 20.0 <
and and

while

2dup \ duplicate ro and rd
s swap tmp1 scalarmul3  \ rd is top item on stack, swap with s, tmp1 = s*rd
tmp1 tmp1 add3 \ ro is top item on stack, tmp1 = tmp1 + ro

tmp1 map dup @ to h #1 cells #+ @ to m
h abs 0.0005 s * f>= h * s + to s
i 1 + to i


repeat

2drop \ drop ro and rd

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

dup -0.5 > if \ if s > -0.5

dup rd pos scalarmul3 \ castray value is stack top, need to duplicate to validate balancing in if statement, pos = s*rd
ro pos pos add3 \ pos = pos + ro

pos calcnormal

1 value ks

over 4 f= col eyeball_col rot col mix3
over 3 f= col iris_col rot col mix3
over 2 f= col body_col rot col mix3

-0.2 0.2 pos @ 18 * sin pos #1 cells #+ @ 18 * sin pos #2 cells #+ @ 18 * sin + + smoothstep 2 * -1 + 0.2 * 
terrain_col2 terrain_col2 scalarmul3 terrain_col2 terrain_col terrain_col add3

over 1 f= col terrain_col rot col mix3

over 1 f= ks pos #1 cells #+ @ 0.15 * 0.5 + mix to ks

 \ lighting
norm sun_lig dot3 0 1 clamp  \ sun_dif
value sun_dif

sun_lig rd sun_hal sub3
sun_hal sun_hal normalize3

0.001 norm tmp2 scalarmul3 pos tmp2 tmp2 add3 
tmp2 sun_lig castRay @ 0 step \ sun_sha
value sun_sha



norm sun_hal dot3 0 1 clamp 8 ** sun_dif * sun_hal rd dot3 1 + 0 1 clamp 5 ** 0.96 * 0.04 + * ks * \sun_spe
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

\ merge basecol and col

dup 0 f> col col scalarmul3
dup 0 f<= basecol basecol scalarmul3
basecol col col add3
;


: main


 \ camera
time 0.15 * sin 0.7 * 1.57 +
dup cos 1.3 * -0.25 rot sin 1.3 * ro vec3!
0 0.65 -0.6 time + 0.4 time 0.5 * sin * - ta vec3!
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


