# BitTable
C macro Bit Table similar to Bit Field packing 32 booleans into each element of the int array with bitwise get / set methods


[![HitCount](http://hits.dwyl.io/me4tw/bittable.svg)](http://hits.dwyl.io/me4tw/bittable)

## Dynamic Bit Tables
Just added dynamic bit tables, which are even better.
Want to use a bitmap to track what blocks are free / in use? Dynamic bit table can be part of the foundation of that. It uses some of the BitTable macros on a block of memory that it allocs / reallocs as need be.

The idea why I use dynamic bit tables is as one way of keeping track of what is free / in use. It's slower than a sorted data structure or maintaining a free block stack, but it uses the least ram (32 object status per unsigned integer which is 4 bytes, as opposed to 8 or 12 bytes or so per object for other data structures).

I separately keep track of the first free block (unset bit) to make it even faster.

