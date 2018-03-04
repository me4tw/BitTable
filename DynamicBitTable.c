// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "DynamicBitTable.h"
#include "String.h"//memset()

union DynamicBitTable DynamicBitTable_Create(void)
{
	union DynamicBitTable ret;
	ret.ss.ss[0] = 0;
	ret.ss.ss[1] = 0;
#if DYBITA_NUM_STATIC > 2
	ret.ss.ss[2] = 0;
	ret.ss.ss[3] = 0;
#endif
	BITTABLE_SET(ret.ss.ss, 0, 1);
	return ret;
}

void DynamicBitTable_Destroy(union DynamicBitTable *bt)
{
	if(BITTABLE_GET(bt->ss.ss, 0))
	{
		//static
		bt->ss.ss[0] = 0;
		bt->ss.ss[1] = 0;
#if DYBITA_NUM_STATIC > 2
		bt->ss.ss[2] = 0;
		bt->ss.ss[3] = 0;
#endif
	} else
	{
		//dynamic
		free(bt->dy.allocated);
		bt->dy.allocated = 0;
		bt->dy.num = 0;
		//reset back to static
		BITTABLE_SET(bt->ss.ss,0,1);
	}
}

static int is_static(union DynamicBitTable* bt)
{
	return BITTABLE_GET(bt->ss.ss, 0);
}
static void static_expansion(union DynamicBitTable *bt, size_t uNeeded)
{
	size_t i;
	//we must upgrade to dynamic
	unsigned *newU = calloc(uNeeded, sizeof(unsigned));
	//copy over any existing data
	for(i=0;i<31;++i)
	{
		int bit = BITTABLE_GET(bt->ss.ss, i+1);
		BITTABLE_SET(newU,i,bit);
	}
	//set it
	bt->dy.num = uNeeded;
	bt->dy.allocated = newU;
}
static int isOkay_dynamic_expansion(union DynamicBitTable *bt, size_t uNeeded)
{
	//expand it
	unsigned *temp = realloc(bt->dy.allocated, uNeeded * sizeof(unsigned));
	if(!temp)
	{
		return 0;
	}
	//clear the expanded region
	memset(temp+bt->dy.num,0, sizeof(unsigned) * (uNeeded - bt->dy.num));
	//assign it
	bt->dy.allocated = temp;
	bt->dy.num = uNeeded;
	return 1;
}
int DynamicBitTable_Get(union DynamicBitTable *bt, size_t num)
{
	/*+1 for the bit used up and +1 for count vs index*/
	size_t uNeeded = BT_uNEEDEDFORBITS(num+1+1);
	if(is_static(bt))
	{
		if(uNeeded > DYBITA_NUM_STATIC)
		{
			static_expansion(bt, uNeeded);
			//now take the dynamic path
			return DynamicBitTable_Get(bt, num);
		}
		return BITTABLE_GET(bt->ss.ss,num+1);
	} else
	{
		if(uNeeded > bt->dy.num)
		{
			if(!isOkay_dynamic_expansion(bt, uNeeded))
				return 0;
		}
		return BITTABLE_GET(bt->dy.allocated,num);
	}
}
void DynamicBitTable_Set(union DynamicBitTable *bt, size_t num, int binaryValue)
{
	size_t uNeeded = BT_uNEEDEDFORBITS(num+1);
	//ensure in range of 0 or 1
	binaryValue = (binaryValue == 0) ? 0 : 1;//failing is faster for common case
	if(is_static(bt))
	{
		if(uNeeded > DYBITA_NUM_STATIC)
		{
			static_expansion(bt, uNeeded);
			//now take the dynamic path
			DynamicBitTable_Set(bt, num, binaryValue);
		}
		BITTABLE_SET(bt->ss.ss, num+1, binaryValue);
	} else
	{
		if(uNeeded > bt->dy.num)
		{
			if(!isOkay_dynamic_expansion(bt, uNeeded))
				return;
		}
		BITTABLE_SET(bt->dy.allocated, num, binaryValue);
	}
}







#ifndef NOTESTING

#include "CuTest.h"

void tDynamicBitTable_NoMalloc(CuTest *qt)
{
	union DynamicBitTable bt;

	bt = DynamicBitTable_Create();

	DynamicBitTable_Set(&bt, DYBITA_MAXNUMBITS_STOREDINSTATIC, 1);
	CuAssertTrue(qt, is_static(&bt));
}

void tDynamicBitTable_SetGet(CuTest *qt)
{
	union DynamicBitTable bt;
	bt = DynamicBitTable_Create();
	DynamicBitTable_Set(&bt, 5, 1);
	CuAssertIntEquals(qt, 1, DynamicBitTable_Get(&bt, 5));
	DynamicBitTable_Set(&bt, 5, 0);
	CuAssertIntEquals(qt, 0, DynamicBitTable_Get(&bt, 5));
}

void tDynamicBitTable_SupportsHugeSize(CuTest *qt)
{
	union DynamicBitTable bt;
	bt = DynamicBitTable_Create();
	DynamicBitTable_Set(&bt, 1000, 1);
	CuAssertIntEquals(qt, 1, DynamicBitTable_Get(&bt, 1000));
	CuAssertIntEquals(qt, 0, DynamicBitTable_Get(&bt, 999));
	DynamicBitTable_Destroy(&bt);
}
CuSuite* tDynamicBitTable_GetSuite(void)
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, tDynamicBitTable_NoMalloc);
	SUITE_ADD_TEST(suite, tDynamicBitTable_SetGet);
	SUITE_ADD_TEST(suite, tDynamicBitTable_SupportsHugeSize);
	return suite;
}
#endif //end: NOTESTING



