// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**@file
@brief Static assertion macro for workaround for #if not allowing sizeof()

You want to do
@code
#if !(sizeof(int)==4 || sizeof(int)==8)
#error only 32 or 64 bit atm
#endif
@endcode

but you cant because #if can't accept sizeof(), so instead you can do this:

@code
STATIC_ASSERT(sizeof(int)==32||sizeof(int)==64,"only 32 or 64bit atm")
@endcode

And that is not quite as good but at least it will actually work i.e. throw
a compile time error if the case is not met, but you don't actually get
to see your message until clicking the error and reading it from the source
line unless your compiler actually supports static_assert natively

*/
#ifndef StaticAssert_h
#define StaticAssert_h


#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
/** These can't be used after statements in c89. */
#ifdef __COUNTER__
#define STATIC_ASSERT(e,m) \
    ;enum { ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/(int)(!!(e)) }
#else
/** This can't be used twice on the same line so ensure if using in headers
that the headers are not included twice (by wrapping in #ifndef...#endif)
Note it doesn't cause an issue when used on same line of separate modules
compiled with gcc -combine -fwhole-program.  */
#define STATIC_ASSERT(e,m) \
    ;enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(int)(!!(e)) }
#endif


#endif
