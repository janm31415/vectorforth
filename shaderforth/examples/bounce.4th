: px x 2 * rx - ry / ;
: py y 2 * ry - ry / ;
: e 0.0001 ;

vec3 ro    
vec3 rd        
vec3 pos
vec3 e1    
vec3 e2        
vec3 e3
vec3 norm  
vec3 sun_dir
vec3 col   
vec3 sun_col   
vec3 sky_col 
vec3 bounce_col
vec3 tmp1  
vec3 tmp2
vec3 tmp3
vec3 tmp4
vec3 basecol
vec3 ta
vec3 uu
vec3 vv
vec3 ww
vec3 rad

0 value s 
0 value i
100 value h

e 0 0 e1 vec3!
0 e 0 e2 vec3!
0 0 e e3 vec3!

0.8 0.4 0.2 sun_dir vec3!
sun_dir sun_dir normalize3

7 4.5 3 sun_col vec3!
0.5 0.8 0.9 sky_col vec3!
0.7 0.3 0.2 bounce_col vec3!

: fract dup floor - ;

: sdEllipsoid (in vec3 rad, in vec3 pos)
over (rad pos rad)
tmp4 div3  (rad)
tmp4 length3 (rad k0)
swap tmp4 swap (k0 tmp4 rad)
tmp4 div3 tmp4 length3 (k0 k1)
swap dup (k1 k0 k0)
1 - * swap /
;

: sdguy (in vec3 pos)
0
t fract dup 1 - negate 4 * *
dup -rot  (save y on the stack)
0
tmp3 vec3!
swap tmp3 tmp3 sub3

\0.25 0.25 0.25 rad vec3!
0.25 
swap 0.5 * 0.5 + 0.25 *
dup 1.0 swap / 0.0625 *
rad vec3!

rad
tmp3
sdEllipsoid
;

: map (in vec3 pos)
  dup
  sdguy
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


(todo: replace if test with floating point test and check speed diff)
s 20 > (put result on the stack)
if
-1
else
s
then
;

mx 10 * rx /

dup sin 1.5 * 0 rot cos 1.5 * ro vec3!

0 0.5 0 ta vec3!

ta ro ro add3

ta ro ww sub3 ww ww normalize3

ww here @ 0 , 1 , 0 , uu cross3 uu uu normalize3
uu ww vv cross3 vv vv normalize3

px uu tmp1 scalarmul3
py vv tmp2 scalarmul3

tmp1 tmp2 tmp1 add3

1.8 ww tmp2 scalarmul3

tmp1 tmp2 rd add3

rd rd normalize3

rd #32+ @ dup -0.7 * swap -10 * exp   ( -0.7*rd.y  exp(-10*rd.y)  )
     over 0.4 +  over 0.7 swap mix 
-rot over 0.75 + over 0.75 swap mix
-rot swap 1 + swap 0.8 swap mix  basecol vec3!


ro rd castray

dup 0 > if

dup rd pos scalarmul3  (castray value is stack top, need to duplicate to validate balancing in if statement, pos = s*rd)
ro pos pos add3 (pos = pos + ro)

pos calcnormal

norm sun_dir dot3 0 1 clamp over 0 f> *            \ sun diffuse

0.001 norm tmp2 scalarmul3 pos tmp2 tmp2 add3 
tmp2 sun_dir castray 0 step *                        \ shadow

0.18 * \ material color

sun_col col scalarmul3

norm #32+ @ 0.5 * 0.5 + 0 1 clamp over 0 f> *      \ sky diffuse

0.18 * \ material color

sky_col tmp2 scalarmul3                            
col tmp2 col add3

norm #32+ @ -0.5 * 0.5 + 0 1 clamp over 0 f> *      \ bounce diffuse

0.18 * \ material color

bounce_col tmp2 scalarmul3                            
col tmp2 col add3

else

\ do nothing, use basecol

then

(merge basecol and col)

dup 0 f> col col scalarmul3
dup 0 f<= basecol basecol scalarmul3
basecol col col add3


col @ 0.4545 **
col #32+ @ 0.4545 ** 
col #64+ @ 0.4545 **


