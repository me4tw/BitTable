// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/**
@file
@brief Dynamic bit tables build on top of standard bit tables

These may malloc memory but only if need be, they will use an integer member of the union if possible.
*/

#ifndef DynamicBitTable_h
#define DynamicBitTable_h

#include <stddef.h> //size_t
#include "BitTable.h"
#include "StdInt.h" //SIZE_MAX

/** The number of unsigned_t that can fit within the same space as a size_t and a pointer */
#if SIZE_MAX > UINT_MAX && UINT_MAX > USHRT_MAX
/** 64bit size_t 32bit int */
#define DYBITA_NUM_STATIC 4
#elif SIZE_MAX == UINT_MAX && UINT_MAX > USHRT_MAX
/** 32bit size_t 32bit int */
#define DYBITA_NUM_STATIC 2
#else
#error "unknown scenario"
#endif


#define DYBITA_MAXNUMBITS_STOREDINSTATIC (DYBITA_NUM_STATIC * unsigned_t_bits)-1

struct Static_BitTable
{
	/**The first bit will be set to one if we are using static*/
	unsigned_t ss[DYBITA_NUM_STATIC];
};

struct Dynamic_BitTable
{
	/**
	Holds the number of items in allocated.
	The high bit of this field must be zero or else it will be mis-interpreted
	as using static storage
	*/
	size_t num;
	unsigned_t *allocated;
};
/**
To find out if we are using static storage, check the first bit of ss.ss, if set then we are using static, else
use dy.allocated instead.
*/
union DynamicBitTable
{
	struct Static_BitTable ss;
	struct Dynamic_BitTable dy;
};

/**
will not use malloc() straight away but further inserts might use malloc()
*/
union DynamicBitTable DynamicBitTable_Create(void);

/**
@warning must always be called in case malloc() was used so that the RAM can be free()'d
*/
void DynamicBitTable_Destroy(union DynamicBitTable *bt);

/**
Will expand the memory if need be to cover that range, in the case of fresh expansion
the default value will always return 0.
@param bt       the dynamic bit table
@parm num       the bit number to perform the lookup on
@return the binary value found at that position, will either be a 0 or a 1 (not -1)
*/
int DynamicBitTable_Get(union DynamicBitTable *bt, size_t num);

/**
Will expand the memory if need be to cover up to that range.
@param bt      the dynamic bit table
@param num       the bit number to set
@param binaryValue     the value to set, will be reduced to either a 0 or a 1
*/
void DynamicBitTable_Set(union DynamicBitTable *bt, size_t num, int binaryValue);

#ifndef NOTESTING
#include "CuTest.h"
CuSuite* tDynamicBitTable_GetSuite(void);
#define ADD_DYNAMIC_BIT_TABLE_TESTS(SUITE) \
CuSuiteAddSuite(SUITE, tDynamicBitTable_GetSuite())
#endif

#endif DynamicBitTable_h


