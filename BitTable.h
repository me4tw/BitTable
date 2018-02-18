// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**@file
@brief A header-file only implementation of bit tables (1-dimensional arrays of 1bit elements)

These macros will only work for bit tables defined on the stack and used from the same function,
not when allocated via malloc or when passed as function arguments
*/
#ifndef BIT_TABLE_H
#define BIT_TABLE_H

#include <string.h>

#define unsigned_t                                   unsigned
#define unsigned_t_bits                              (sizeof(unsigned_t)*8)
/**Will evaluate to zero if unsigned_t is not 16, 32 or 64 bit, so if so then fix it by adding the correct check and number for the bits in unsigned_t*/
#define unsigned_t_sizemask                          (unsigned_t_bits == 32 ? 0x1f : unsigned_t_bits == 16 ? 0x0f : unsigned_t_bits == 64 ? 0x3f : 0)

/**
Returns the mask to bitwise-AND by to get the remainder
@param NUM      Give the bit width of an integer here
*/
#define INTERNAL_USE_ONLY_GET_UNSIGNED(NAME, ITEM)   NAME[(ITEM) / sizeof(unsigned)]
#define INTERNAL_USE_ONLY_SET_BIT(U, NUM, VAL)       U |= VAL << ((sizeof(unsigned)*8) - (NUM))

#define BT_uNEEDEDFORBITS_mainpart(BITS)            ((BITS) / unsigned_t_bits)
#define BT_uNEEDEDFORBITS_remainder(BITS)           (((BITS) & unsigned_t_sizemask) ? 1 : 0)
/**@return how many unsigned_t is needed for holding that many BITS*/
#define BT_uNEEDEDFORBITS(BITS)                     (!(BITS)?1:(BT_uNEEDEDFORBITS_mainpart(BITS) + BT_uNEEDEDFORBITS_remainder(BITS)))

#define BT_NumToIndex(NUM)                          ((NUM)/unsigned_t_bits)
#define BT_NumToBitNum(NUM)                         (((NUM)&unsigned_t_sizemask))
/**To protect from assigning the value and clobbering other bits*/
#define BT_MakeItNonAssignable(VAL)                 ((VAL)==1?1:0)
#define BT_ShiftDownAmount(NUM)                     ((unsigned_t_bits - BT_NumToBitNum(NUM)) - 1)

#define BT_Set(U,BIT)                               ((U) | 1 << ((unsigned_t_bits - BIT) - 1))
#define BT_Clear(U,BIT)                             ((U) & ~(1 << ((unsigned_t_bits - BIT) - 1)))

/**
Use this to declare your bit table variable
@warning You must initialize the table before you can safely use it
@param NAME     the variable name of the bit table so you can access it
@param SIZE     how many bits are in the bit table
*/
#define BITTABLE_DECLARE(NAME,SIZE)                  unsigned_t NAME[BT_uNEEDEDFORBITS(SIZE)]
#define BITTABLE_INIT(NAME,DEFAULT_VALUE)            memset((NAME), (DEFAULT_VALUE) == 0 ? 0 : 255, sizeof(NAME));
#define BITTABLE_GET(NAME, ITEM)                     BT_MakeItNonAssignable(((NAME[BT_NumToIndex(ITEM)]) >> BT_ShiftDownAmount(ITEM) )& 0x01)
#define BITTABLE_SET(NAME, ITEM, VAL)                NAME[BT_NumToIndex(ITEM)] = (VAL) ? BT_Set(NAME[BT_NumToIndex(ITEM)], BT_NumToBitNum(ITEM)) : BT_Clear(NAME[BT_NumToIndex(ITEM)], BT_NumToBitNum(ITEM))
/**
Sets the status of an item in the bit table
@param NAME         the variable name of the bit table
@param ITEM         the item number to set
@param STATUS       the status to set (either 0 or 1)
*/
#define SET_BIT_TABLE_VAL(NAME, ITEM, STATUS)        INTERNAL_USE_ONLY_GET_UNSIGNED(NAME,ITEM) (ITEM) & INTERNAL_USE_ONLY_NUMBER_TO_MASK(sizeof(unsigned))

#ifndef NOTESTING
#include "CuTest.h"
CuSuite* tBitTable_GetSuite(void);
#define ADD_BIT_TABLE_TESTS(SUITE) \
CuSuiteAddSuite(SUITE, tBitTable_GetSuite())
#endif

#include "StaticAssert.h"
STATIC_ASSERT(unsigned_t_bits == 32 || unsigned_t_bits == 64 || unsigned_t_bits == 16, "only 16, 32, 64 bit supported so far");

#endif /*BIT_TABLE_H*/
