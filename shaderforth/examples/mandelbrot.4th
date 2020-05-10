: x' u rx ry / * 2 - ;
: y' v 0.5 - ;
: iterate 2dup z* x' y' z+ ;
: fract dup floor - ;
0.0 0.0
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
iterate
2dup z* dup * swap dup * + 0.1 pow fract
dup fract swap
dup sqrt fract