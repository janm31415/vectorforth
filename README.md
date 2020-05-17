# vectorforth
SIMD vectorized Forth compiler with CPU based shader application

## Glossary

### Core vectorforth
`@`    ( #a -- v )  Read the value v at memory address #a and put it on the stack.

`!`    ( v #a -- )  Store value v in memory address #a.

`(`    ( -- )  A multiline comment until the corresponding ).

`\`    ( -- )  A comment until the end of the line.

`>r`   ( v -- )  Move a value v from the stack to the return stack.

`r>`   ( -- v )  Pop a value v from the return stack and push it on the regular stack.

`push`   ( v -- )  Move a value v from the stack to the return stack.

`pop`   ( -- v )  Pop a value v from the return stack and push it on the regular stack.

`rt@`    ( -- #v ) Read the address of the return stack top and put it on the stack.

`rp@`    ( -- #v ) Read the address of the return stack pointer and put it on the stack.

`st@`    ( -- #v ) Read the address of the stack top and put it on the stack.

`sp@`    ( -- #v ) Read the address of the stack pointer and put it on the stack.

`dup`    ( v -- v v )  Duplicate the value on the top of the stack.

`over`    ( v w -- v w v )  Duplicate the element under the top stack element.

`drop`    ( v -- )  Pop the top element of the stack.

`swap`    ( v w -- w v ) Swap the top two elements on the stack.

`rot`    ( a b c -- b c a )  Rotate the top three elements on the stack.

`-rot`    ( a b c -- c a b )  Reverse rotate the top three elements on the stack.

`2drop`    ( a b -- )  Pop the top two elements of the stack.

`2dup`    ( a b -- a b a b ) Duplicate the top two elements on the stack.

`2swap`    ( a b c d --- c d a b )  Swap the top two pairs on the stack.

`+`    ( a b -- a+b )  Pop the top two values from the stack, and push their sum to the stack.

`-`    ( a b -- a-b )  Pop the top two values from the stack, and push their difference to the stack.

`*`    ( a b -- a\*b )  Pop the top two values from the stack, and push their multiplication to the stack.

`/`    ( a b -- a/b )  Pop the top two values from the stack, and push their quotient to the stack.

`z*`    ( a b c d -- a\*c-b\*d a\*d+b\*c )  Pop the top four values from the stack, treat the pairs as complex numbers, and store their multiplication on the stack.

`z+`    ( a b c d -- a+c b+d )  Pop the top four values from the stack, treat the pairs as complex numbers, and store their sum on the stack.

`z-`    ( a b c d -- a-c b-d )  Pop the top four values from the stack, treat the pairs as complex numbers, and store their difference on the stack.

`min`    ( a b -- v )  Select the smaller value of the top two stack elements.

`max`    ( a b -- v )  Select the larger value of the top two stack elements.

`sin`    ( a -- v )  Pop the top element of the stack, and put its sine on the stack.

`cos`    ( a -- v )  Pop the top element of the stack, and put its cosine on the stack.

`tan`    ( a -- v )  Pop the top element of the stack, and put its tangent on the stack.

`log`    ( a -- v )  Pop the top element of the stack, and put its logarithm with base e on the stack.

`exp`    ( a -- v )  Pop the top element of the stack, and put its exponential on the stack.

`sqrt`    ( a -- v )  Pop the top element of the stack, and put its square root on the stack.

`abs`    ( a -- v )  Pop the top element of the stack, and put its absolute value on the stack.

`negate`    ( a -- -a )  Pop the top element of the stack, and put its negative value on the stack.

`pow`    ( a b -- a^b )  Pop the top two elements of the stack, and push a to the power b on the stack.

`**`    ( a b -- a^b )  Pop the top two elements of the stack, and push a to the power b on the stack.

`atan2`    ( a b -- v )  Pop the top two elements of the stack and push the arctangent of a/b on the stack.

`fm/mod`    ( a b -- rem quot )  Pop the top two elements a and b of the stack. Divide a by b, giving the floored quotient quot and the remainder rem. Put rem and quot on the stack.

`floor`    ( v -- v )  Round the top element of the stack downward.

`ceil`    ( v -- v )  Round the top element of the stack upward.

`round`    ( v -- v )  Round the top element of the stack to the nearest integer.

`trunc`    ( v -- v )  Truncate the top element of the stack (i.e. round towards zero).

`=`    ( a b -- v ) 0xffffffff if the top elements are equal, else 0x00000000.

`<>`    ( a b -- v ) 0xffffffff if the top elements are not equal, else 0x00000000.

`<`    ( a b -- v ) 0xffffffff if a < b, else 0x00000000.

`>`    ( a b -- v ) 0xffffffff if a > b, else 0x00000000.

`<=`    ( a b -- v ) 0xffffffff if a <= b, else 0x00000000.

`>=`    ( a b -- v ) 0xffffffff if a >= b, else 0x00000000.

`f=`    ( a b -- v )  1.0 if the top elements are equal, else 0.0.

`f<>`    ( a b -- v )  1.0 if the top elements are not equal, else 0.0.

`f<`    ( a b -- v )  1.0 if a < b, else 0.0.

`f>`    ( a b -- v )  1.0 if a > b, else 0.0.

`f<=`    ( a b -- v )  1.0 if a <= b, else 0.0.

`f>=`    ( a b -- v )  1.0 if a >= b, else 0.0.

`#+`    ( #a #b -- #a+#b )  Pop two addresses from the stack, and push their sum on the stack.

`#-`    ( #a #b -- #a-#b )  Pop two addresses from the stack, and push their difference on the stack.

`#*`    ( #a #b -- #a*#b )  Pop two addresses from the stack, and push their multiplication on the stack.

`#/`    ( #a #b -- #a*#b )  Pop two addresses from the stack, and push their quotient on the stack.

`#1+`    ( #a -- #a+1)  Pop the top item (an address typically) from the stack and add one. Push the result on the stack.

`#1-`    ( #a -- #a-1)  Pop the top item (an address typically) from the stack and subtract one. Push the result on the stack.

`#32+`    ( #a -- #a+32)  Pop the top item (an address typically) from the stack and add 32. Push the result on the stack.

`#32-`    ( #a -- #a-32)  Pop the top item (an address typically) from the stack and subtract 32. Push the result on the stack.

`#64+`    ( #a -- #a+64)  Pop the top item (an address typically) from the stack and add 64. Push the result on the stack.

`#64-`    ( #a -- #a-64)  Pop the top item (an address typically) from the stack and subtract 64. Push the result on the stack.

`#96+`    ( #a -- #a+96)  Pop the top item (an address typically) from the stack and add 96. Push the result on the stack.

`#96-`    ( #a -- #a-96)  Pop the top item (an address typically) from the stack and subtract 96. Push the result on the stack.

`#128+`    ( #a -- #a+128)  Pop the top item (an address typically) from the stack and add 128. Push the result on the stack.

`#128-`    ( #a -- #a-128)  Pop the top item (an address typically) from the stack and subtract 128. Push the result on the stack.

`#160+`    ( #a -- #a+160)  Pop the top item (an address typically) from the stack and add 160. Push the result on the stack.

`#160-`    ( #a -- #a-160)  Pop the top item (an address typically) from the stack and subtract 160. Push the result on the stack.

`#192+`    ( #a -- #a+192)  Pop the top item (an address typically) from the stack and add 192. Push the result on the stack.

`#192-`    ( #a -- #a-192)  Pop the top item (an address typically) from the stack and subtract 192. Push the result on the stack.

`#224+`    ( #a -- #a+224)  Pop the top item (an address typically) from the stack and add 224. Push the result on the stack.

`#224-`    ( #a -- #a-224)  Pop the top item (an address typically) from the stack and subtract 224. Push the result on the stack.

`#256+`    ( #a -- #a+256)  Pop the top item (an address typically) from the stack and add 256. Push the result on the stack.

`#256-`    ( #a -- #a-256)  Pop the top item (an address typically) from the stack and subtract 256. Push the result on the stack.

`#+!`    ( #v #a -- )  Assumes that address #a points to another address, and adds value #v to this address pointed to by #a. The top two elements on the stack are dropped.

`#-!`    ( #v #a -- )  Assumes that address #a points to another address, and subtracts value #v from this address pointed to by #a. The top two elements on the stack are dropped.

`pi`    ( -- 3.1415926535897 )  Puts pi on the stack.

`here`    ( -- #a )  Puts the address of the data space (heap) pointer on the stack.

`cells`    ( #n -- 32*#n)  Puts the size in bytes on the stack of #n cells.

`,`    ( v -- )  Moves the top stack element to the address pointed to by the data space pointer, and updates the data space pointer so that it points to the next available location.

### Specific shader forth definitions
`x`    ( -- x )  Put the current x-coordinate on the stack.
    
`y`    ( -- y )  Put the current y-coordinate on the stack.
    
`rx`   ( -- rx )  Put the x-component of the screen resolution on the stack.
  
`ry`   ( -- ry )  Put the y-component of the screen resolution on the stack.
  
`u`    ( -- u )  Put the normalized (between 0 and 1) x-coordinate on the stack.
  
`v`    ( -- v )  Put the normalized (between 0 and 1) y-coordinate on the stack.
  
`t`    ( -- t )  Put the current time in seconds since the start of the shader on the stack.
  
`dt`    ( -- dt )  Put the change in time since the last frame in seconds on the stack.
  
`frame`    ( -- frame )  Put the frame index on the stack.
  
`mx`    ( -- mx )  Put the mouse x-coordinate on the stack.
  
`my`    ( -- my )  Put the mouse y-coordinate on the stack.

`mz`    ( -- mz )  Is negative mx when a mouse button is pressed, otherwise positive mx.

`mw`    ( -- mw )  Is negative my when a mouse button is pressed, otherwise positive my.
