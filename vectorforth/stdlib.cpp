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
  register_definition(d, ": mod fm/mod drop ;");
  register_definition(d, ": div fm/mod swap drop ;");
  register_definition(d, ": nip ( x y -- y ) swap drop ;");
  register_definition(d, ": tuck ( x y -- y x y ) swap over ;");
  register_definition(d, ": pick (  x_u ... x_1 x_0 #u -- x_u ... x_1 x_0 x_u ) #1+ #32 #* sp@ #+ @;");
  register_definition(d, ": true v8 #-1 #-1 #-1 #-1 #-1 #-1 #-1 #-1 ;");
  register_definition(d, ": false 0 ;");
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
        f<=      ( b c [a<=c])
        -rot     ([a<=c] b c)
        f>       ([a<=c] [b>c])
        * negate icast 
   ; )");

  register_definition(d, ": depth st@ sp@ #- #32- #32 #/;");
  register_definition(d, ": rdepth rt@ rp@ #- #32 #/;");

  register_definition(d, R"(
  : clamp (  c a b clamp returns c if a < c and c < b, or a if c < a, or b if c > b )
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
  swap over  (x a y a)
  *          (x a y*a)
  -rot       (y*a x a)
  1 -        (y*a x a-1)
  * -        (y*a-x*[a-1])
  ;
)");
  }

VF_END