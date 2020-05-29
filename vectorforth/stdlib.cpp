#include "stdlib.h"
#include "dictionary.h"
#include "tokenize.h"
#include <algorithm>
#include <cassert>


VF_BEGIN

namespace
  {
  void register_definition(dictionary& d, const std::string& def)
    {
    std::vector<token> words = tokenize(def);
    std::reverse(words.begin(), words.end());
    std::vector<token> definition_words;
    if (words.empty())
      throw std::runtime_error("error in stdlib: empty definition");
    if (words.back().type != token::T_COLON)
      throw std::runtime_error("error in stdlib: definition should start with a colon");
    words.pop_back();
    while (!words.empty())
      {
      definition_words.push_back(words.back());
      words.pop_back();
      if (definition_words.back().type == token::T_SEMICOLON)
        break;
      }
    if (definition_words.back().type != token::T_SEMICOLON)
      throw std::runtime_error("error in stdlib: definition should end with semicolon");
    if (!words.empty())
      throw std::runtime_error("error in stdlib: didn't process the full definition");
    definition_words.pop_back(); // last item is semicolon, we don't need that for the definition
    register_definition(d, definition_words);
    }
  }

void add_stdlib_to_dictionary(dictionary& d)
  {
  register_definition(d, ": pi 3.14159265359 ;");

  register_definition(d, ": #1+ #1 #+ ;");
  register_definition(d, ": #32+ #32 #+ ;");
  register_definition(d, ": #64+ #64 #+ ;");
  register_definition(d, ": #96+ #96 #+ ;");
  register_definition(d, ": #128+ #128 #+ ;");
  register_definition(d, ": #160+ #160 #+ ;");
  register_definition(d, ": #192+ #192 #+ ;");
  register_definition(d, ": #224+ #224 #+ ;");
  register_definition(d, ": #256+ #256 #+ ;");

  register_definition(d, ": #1- #1 #- ;");
  register_definition(d, ": #32- #32 #- ;");
  register_definition(d, ": #64- #64 #- ;");
  register_definition(d, ": #96- #96 #- ;");
  register_definition(d, ": #128- #128 #- ;");
  register_definition(d, ": #160- #160 #- ;");
  register_definition(d, ": #192- #192 #- ;");
  register_definition(d, ": #224- #224 #- ;");
  register_definition(d, ": #256- #256 #- ;");

  register_definition(d, ": mod fm/mod drop ;");
  register_definition(d, ": div fm/mod swap drop ;");
  register_definition(d, ": nip ( x y -- y ) swap drop ;");
  register_definition(d, ": tuck ( x y -- y x y ) swap over ;");
#ifdef AVX512
  register_definition(d, ": pick (  x_u ... x_1 x_0 #u -- x_u ... x_1 x_0 x_u ) #1+ #64 #* sp@ #+ @;");
#else
  register_definition(d, ": pick (  x_u ... x_1 x_0 #u -- x_u ... x_1 x_0 x_u ) #1+ #32 #* sp@ #+ @;");
#endif

#ifdef AVX512
  register_definition(d, ": cells #6 #<< ;");
#else
  register_definition(d, ": cells #5 #<< ;");
#endif

#ifdef AVX512
  register_definition(d, ": true v16 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1;");
#else
  register_definition(d, ": true v8 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 ;");
#endif
  register_definition(d, ": false 0 ;");

  register_definition(d, ": allot here @ swap here #+! ;");

  register_definition(d, ": variable #1 cells allot create ;");

  register_definition(d, ": value create ;");


  /*
  // old definition of within, uses if and thus branching, which is not recommended
  register_definition(d, R"(
    : within_old (  c a b within returns true if a <= c and c < b) 
        -rot     ( b c a )
        over     ( b c a c )
        <= if     ( b c )
           2dup >  (b c b c -> b c result )
           else
           0       (b c 0 )
           then
         -rot    (b c result -> result b c)
         2drop  (result b c - < result)
   ; )");
   */
  register_definition(d, R"(
    : within (  c a b within returns true if a <= c and c < b) 
        -rot     ( b c a )
        over     ( b c a c )
        f<=      ( b c (a<=c))
        -rot     ((a<=c) b c)
        f>       ((a<=c) (b>c))
        * negate icast 
   ; )");

#ifdef AVX512
  register_definition(d, ": depth st@ sp@ #- #64- #64 #/;");
  register_definition(d, ": rdepth rt@ rp@ #- #64 #/;");
#else
  register_definition(d, ": depth st@ sp@ #- #32- #32 #/;");
  register_definition(d, ": rdepth rt@ rp@ #- #32 #/;");
#endif

  register_definition(d, R"(
  : clamp2 (  c a b clamp returns c if a < c and c < b, or a if c < a, or b if c > b )
      -rot     ( b c a )
      over     ( b c a c )
      over     ( b c a c a )
      swap     ( b c a a c )
      2dup     ( b c a a c a c )      
      f<       ( b c a a c [a < c] )
      -rot     ( b c a [a < c] a c )
      swap     ( b c a [a < c] c a )
      -        ( b c a [a < c] c-a )
      *        ( b c a [a < c]*[c - a] )
      +        ( b c a+[a < c]*[c - a] )
      rot      ( c a+[a < c]*[c - a] b)
      dup      ( c a+[a < c]*[c - a] b b)
      -rot     ( c b a+[a < c]*[c - a] b)
      -        ( c b [a+[a < c]*[c - a]-b])
      -rot     ( [a+[a < c]*[c - a]-b] c b)
      dup      ([a+[a < c]*[c - a]-b] c b b)
      -rot     ( [a+[a < c]*[c - a]-b] b c b)
      f<       ([a+[a < c]*[c - a]-b] b [c< b])
      rot      (b [c< b] [a+[a < c]*[c - a]-b])
      * +
  ; )");

  /*
                float smoothstep(float edge0, float edge1, float x)
                {
                float t = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
                return t * t * (3.f - 2.f * t);
                }
  */
  register_definition(d, R"(
  : smoothstep (  e0 e1 x smoothstep  )
    rot   (e1 x e0)
    dup   (e1 x e0 e0)
    -rot  (e1 e0 x e0)
    -     (e1 e0 [x-e0])
    -rot  ([x-e0] e1 e0)
    -     ([x-e0] [e1-e0])
    /     ([x-e0] / [e1-e0])
    0 1   clamp   (t)
    dup dup       (t t t)
    2 3 -rot      (t t 3 t 2)
    * - * *       (t*t*[3 - 2t])
  ;)");

  /*
  float mix(float x, float y, float a)
                {
                return x * (1 - a) + y * a;
                }
  */
  register_definition(d, R"(
  : mix    ( x y a mix )
  tuck  (x a y a)
  *          (x a y*a)
  -rot       (y*a x a)
  1 -        (y*a x a-1)
  * -        (y*a-x*[a-1])
  ;
)");

  register_definition(d, R"(
  : step    ( edge x step, returns 0 if x < edge, and 1 otherwise )
  f<=
  ;
)");

  register_definition(d, ": vec3 #3 cells allot create ;");

#ifdef AVX512


  register_definition(d, R"(
: vec3! 
tuck #128+ !
tuck #64+ !
!
;
)");

  register_definition(d, R"(
(dot product &a &b => x, where x is a*b with a,b of type vec3)

: dot3   (&a &b)
2dup     (&a &b &a &b)
@ >r @ r> *   (&a &b  a_x*b_x)
>r 2dup       (&a &b &a &b)
#64+ @ >r #64+ @ r> * (&a &b a_y*b_y)
>r            (&a &b)
#128+ @ >r #128+ @ r> * (a_z*b_z)
r> r> + +  (a_x*b_x + a_y*b_y + a_z*b_z)
;
)");

  register_definition(d, R"(
(cross product (&a &b &result => -), where result contains axb)

: cross3   (&a &b &result)
>r
2dup       (&a &b &a &b)
@ >r #64+ @ r> * (&a &b a_y*b_x)
>r 2dup    (&a &b &a &b)
#64+ @ >r @ r> * (a_x*b_y)
r> -       (&a &b a_x*b_y - a_y*b_x)
>r 2dup    (&a &b &a &b)
#128+ @ >r @ r> * (&a &b a_x*b_z)
>r 2dup    (&a &b &a &b)
@ >r #128+ @ r> * (&a &b a_z*b_x)
r> -       (&a &b a_z*b_x-a_x*b_z)
>r 2dup    (&a &b &a &b)
#64+ @ >r #128+ @ r> * (&a &b a_z*b_y)
>r         (&a &b)
#128+ @ >r #64+ @ r> * (&a &b a_y*b_z)
r> -       (a_y*b_z-a_z*b_y)
r> r>      (a_y*b_z-a_z*b_y  a_z*b_x-a_x*b_z   a_x*b_y - a_y*b_x)
r>         (a_y*b_z-a_z*b_y  a_z*b_x-a_x*b_z   a_x*b_y - a_y*b_x   &result)
vec3!
;
)");

  register_definition(d, R"(
(add 2 vec3s (&a &b &result => -), where result contains a+b)

: add3   (&a &b &result)
>r
2dup     (&a &b &a &b)
#128+ @ >r #128+ @ r> + (&a &b a_z+b_z)
>r 2dup       (&a &b &a &b)
#64+ @ >r #64+ @ r> + (&a &b a_y+b_y)
>r            (&a &b)
@ >r @ r> +   (a_x+b_x)
r> r>         (a_x+b_x  a_y+b_y  a_z+a_z )
r>            (a_x+b_x  a_y+b_y  a_z+a_z &result)
vec3!
;
)");

  register_definition(d, R"(
(subtract 2 vec3s (&a &b &result => -), where result contains a-b)

: sub3   (&a &b &result)
>r
2dup     (&a &b &a &b)
#128+ @ >r #128+ @ r> - (&a &b a_z-b_z)
>r 2dup       (&a &b &a &b)
#64+ @ >r #64+ @ r> - (&a &b a_y-b_y)
>r            (&a &b)
@ >r @ r> -   (a_x-b_x)
r> r>         (a_x-b_x  a_y-b_y  a_z-a_z )
r>            (a_x-b_x  a_y-b_y  a_z-a_z &result)
vec3!
;
)");

  register_definition(d, R"(
(multiply 2 vec3s componentwise (&a &b &result => -), where result contains a*b)

: mul3   (&a &b &result)
>r
2dup     (&a &b &a &b)
#128+ @ >r #128+ @ r> * (&a &b a_z*b_z)
>r 2dup       (&a &b &a &b)
#64+ @ >r #64+ @ r> * (&a &b a_y*b_y)
>r            (&a &b)
@ >r @ r> *   (a_x*b_x)
r> r>         (a_x*b_x  a_y*b_y  a_z*a_z )
r>            (a_x*b_x  a_y*b_y  a_z*a_z &result)
vec3!
;
)");

  register_definition(d, R"(
(divide 2 vec3s componentwise (&a &b &result => -), where result contains a/b)

: div3   (&a &b &result)
>r
2dup     (&a &b &a &b)
#128+ @ >r #128+ @ r> / (&a &b a_z/b_z)
>r 2dup       (&a &b &a &b)
#64+ @ >r #64+ @ r> / (&a &b a_y/b_y)
>r            (&a &b)
@ >r @ r> /   (a_x/b_x)
r> r>         (a_x/b_x  a_y/b_y  a_z/a_z )
r>            (a_x/b_x  a_y/b_y  a_z/a_z &result)
vec3!
;
)");

  register_definition(d, R"(
(multiply a vec3 with a scalar (t &v &result => -), where result contains t*v)

: scalarmul3   (t &v &result)
>r
2dup        (t &v t &v)
#128+ @  *   (t &v t*v_z)
>r 2dup     (t &v t &v)
#64+ @  *   (t &v t*v_y)
>r          (t &v)
@ *         (t*v_x)
r> r>       (t*v_x t*vy t*vz)
r> vec3!
;
)");

#else

  register_definition(d, R"(
: vec3! 
tuck #64+ !
tuck #32+ !
!
;
)");

  register_definition(d, R"(
(dot product &a &b => x, where x is a*b with a,b of type vec3)

: dot3   (&a &b)
2dup     (&a &b &a &b)
@ >r @ r> *   (&a &b  a_x*b_x)
>r 2dup       (&a &b &a &b)
#32+ @ >r #32+ @ r> * (&a &b a_y*b_y)
>r            (&a &b)
#64+ @ >r #64+ @ r> * (a_z*b_z)
r> r> + +  (a_x*b_x + a_y*b_y + a_z*b_z)
;
)");

  register_definition(d, R"(
(cross product (&a &b &result => -), where result contains axb)

: cross3   (&a &b &result)
>r
2dup       (&a &b &a &b)
@ >r #32+ @ r> * (&a &b a_y*b_x)
>r 2dup    (&a &b &a &b)
#32+ @ >r @ r> * (a_x*b_y)
r> -       (&a &b a_x*b_y - a_y*b_x)
>r 2dup    (&a &b &a &b)
#64+ @ >r @ r> * (&a &b a_x*b_z)
>r 2dup    (&a &b &a &b)
@ >r #64+ @ r> * (&a &b a_z*b_x)
r> -       (&a &b a_z*b_x-a_x*b_z)
>r 2dup    (&a &b &a &b)
#32+ @ >r #64+ @ r> * (&a &b a_z*b_y)
>r         (&a &b)
#64+ @ >r #32+ @ r> * (&a &b a_y*b_z)
r> -       (a_y*b_z-a_z*b_y)
r> r>      (a_y*b_z-a_z*b_y  a_z*b_x-a_x*b_z   a_x*b_y - a_y*b_x)
r>         (a_y*b_z-a_z*b_y  a_z*b_x-a_x*b_z   a_x*b_y - a_y*b_x   &result)
vec3!
;
)");

  register_definition(d, R"(
(add 2 vec3s (&a &b &result => -), where result contains a+b)

: add3   (&a &b &result)
>r
2dup     (&a &b &a &b)
#64+ @ >r #64+ @ r> + (&a &b a_z+b_z)
>r 2dup       (&a &b &a &b)
#32+ @ >r #32+ @ r> + (&a &b a_y+b_y)
>r            (&a &b)
@ >r @ r> +   (a_x+b_x)
r> r>         (a_x+b_x  a_y+b_y  a_z+a_z )
r>            (a_x+b_x  a_y+b_y  a_z+a_z &result)
vec3!
;
)");

  register_definition(d, R"(
(subtract 2 vec3s (&a &b &result => -), where result contains a-b)

: sub3   (&a &b &result)
>r
2dup     (&a &b &a &b)
#64+ @ >r #64+ @ r> - (&a &b a_z-b_z)
>r 2dup       (&a &b &a &b)
#32+ @ >r #32+ @ r> - (&a &b a_y-b_y)
>r            (&a &b)
@ >r @ r> -   (a_x-b_x)
r> r>         (a_x-b_x  a_y-b_y  a_z-a_z )
r>            (a_x-b_x  a_y-b_y  a_z-a_z &result)
vec3!
;
)");

  register_definition(d, R"(
(multiply 2 vec3s componentwise (&a &b &result => -), where result contains a*b)

: mul3   (&a &b &result)
>r
2dup     (&a &b &a &b)
#64+ @ >r #64+ @ r> * (&a &b a_z*b_z)
>r 2dup       (&a &b &a &b)
#32+ @ >r #32+ @ r> * (&a &b a_y*b_y)
>r            (&a &b)
@ >r @ r> *   (a_x*b_x)
r> r>         (a_x*b_x  a_y*b_y  a_z*a_z )
r>            (a_x*b_x  a_y*b_y  a_z*a_z &result)
vec3!
;
)");

  register_definition(d, R"(
(divide 2 vec3s componentwise (&a &b &result => -), where result contains a/b)

: div3   (&a &b &result)
>r
2dup     (&a &b &a &b)
#64+ @ >r #64+ @ r> / (&a &b a_z/b_z)
>r 2dup       (&a &b &a &b)
#32+ @ >r #32+ @ r> / (&a &b a_y/b_y)
>r            (&a &b)
@ >r @ r> /   (a_x/b_x)
r> r>         (a_x/b_x  a_y/b_y  a_z/a_z )
r>            (a_x/b_x  a_y/b_y  a_z/a_z &result)
vec3!
;
)");

  register_definition(d, R"(
(multiply a vec3 with a scalar (t &v &result => -), where result contains t*v)

: scalarmul3   (t &v &result)
>r
2dup        (t &v t &v)
#64+ @  *   (t &v t*v_z)
>r 2dup     (t &v t &v)
#32+ @  *   (t &v t*v_y)
>r          (t &v)
@ *         (t*v_x)
r> r>       (t*v_x t*vy t*vz)
r> vec3!
;
)");
#endif

  register_definition(d, R"(
(length of a vec3 (&v => length))

: length3   (&v)
dup dot3 sqrt
;
)");

  register_definition(d, R"(
(normalize a vec3 (&v &result => -), where result contains the normalized vec3 v)

: normalize3   (&v &result)
>r             (&v)
dup length3    (&v le)
1.0 swap /     (&v 1/le)
swap r>        (1/le &v &result)
scalarmul3
;
)");

  }

VF_END