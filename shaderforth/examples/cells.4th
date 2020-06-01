  (https://www.shadertoy.com/view/XllSzS)

  : x u ;
  : y v ;
  : s - 2 pow ;
  : d y s swap x s + sqrt ;
  : h t 0.01 * * sin 1 mod ;
  : r r> 1 + dup >r h ;
  : i r r d min ;
  : 8i i i i i i i i i i i ;
  1 0 >r 8i 8i r> drop 0.5 swap - 2 * 0.2 over 3 * -1 + sin 0.2 * 0.8 +
