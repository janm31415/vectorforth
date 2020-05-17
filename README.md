# vectorforth
SIMD vectorized Forth compiler with CPU based shader application

## Glossary

### Core vectorforth
`@`    ( a -- v )  Read the value v at memory address a and put it on the stack.

`!`    ( v a -- )  Store value v in memory address a.

`(`    ( -- )  A multiline comment until the corresponding ).

`\`    ( -- )  A comment until the end of the line.

`>r`   ( v -- )  Move a value v from the stack to the return stack.

`r>`   ( -- v )  Pop a value v from the return stack and push it on the regular stack.

`push`   ( v -- )  Move a value v from the stack to the return stack.

`pop`   ( -- v )  Pop a value v from the return stack and push it on the regular stack.

`rt@`    ( -- v ) Read the address of the return stack top and put it on the stack.

`rp@`    ( -- v ) Read the address of the return stack pointer and put it on the stack.

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

`mz`    ( -- mz )  Is negative mx when a mouse button is pressed, otherwise positive mx

`mw`    ( -- mw )  Is negative my when a mouse button is pressed, otherwise positive my
