
#include "BitTable.h"
void tBitTable_Supported(CuTest *qt)
{
	CuAssertIntEquals(qt, 1, unsigned_t_bits == 32 || unsigned_t_bits == 64 || unsigned_t_bits == 16);
}
void tBitTable_TableSize(CuTest *qt)
{
	BITTABLE_DECLARE(tt, unsigned_t_bits);
	CuAssertIntEquals(qt, sizeof(unsigned_t), (int)sizeof(tt));
}
void tBitTable_InitedToZero(CuTest *qt)
{
	BITTABLE_DECLARE(tt, 32);

	BITTABLE_INIT(tt, 0);

	CuAssertIntEquals(qt, 0, (int)tt[0]);
	
}
void tBitTable_InitedToOne(CuTest *qt)
{
	BITTABLE_DECLARE(tt, 32);

	BITTABLE_INIT(tt, 1);

	CuAssertIntEquals(qt, (int)(0xffffffff), (int)tt[0]);
}
void tBitTable_InitedToOneB(CuTest *qt)
{
	BITTABLE_DECLARE(tt, 24);

	BITTABLE_INIT(tt, 1);

	CuAssertIntEquals(qt, (int)(0xffffff00), (int)tt[0] & 0xffffff00);
}
void tBitTable_BT_uNEEDEDFORBITS(CuTest *qt)
{
	CuAssertIntEquals(qt, 1, (int)BT_uNEEDEDFORBITS(0));
	CuAssertIntEquals(qt, 1, (int)BT_uNEEDEDFORBITS(1));
	CuAssertIntEquals(qt, 1, (int)BT_uNEEDEDFORBITS(31));
	CuAssertIntEquals(qt, 1, (int)BT_uNEEDEDFORBITS(32));
	CuAssertIntEquals(qt, 2, (int)BT_uNEEDEDFORBITS(33));
	CuAssertIntEquals(qt, 2, (int)BT_uNEEDEDFORBITS(34));
}

void tBT_NumToBitNum(CuTest *qt)
{
	CuAssertIntEquals(qt, 0, BT_NumToBitNum(0));
	CuAssertIntEquals(qt, 1, BT_NumToBitNum(1));
	CuAssertIntEquals(qt, 31, BT_NumToBitNum(31));
	CuAssertIntEquals(qt, 0, BT_NumToBitNum(32));
	CuAssertIntEquals(qt, 1, BT_NumToBitNum(33));
}
void tBT_ShiftDownAmount(CuTest *qt)
{
	CuAssertIntEquals(qt, 31, BT_ShiftDownAmount(0));
	CuAssertIntEquals(qt, 30, BT_ShiftDownAmount(1));
	CuAssertIntEquals(qt, 0, BT_ShiftDownAmount(31));
}
void tBITTABLE_GET(CuTest *qt)
{
	unsigned tt[1];
	tt[0] = 0x02;
	CuAssertIntEquals(qt, 0, BITTABLE_GET(tt, 29));
	CuAssertIntEquals(qt, 1, BITTABLE_GET(tt, 30));
	CuAssertIntEquals(qt, 0, BITTABLE_GET(tt, 31));
	tt[0] = 0x40000000;
	CuAssertIntEquals(qt, 0, BITTABLE_GET(tt, 0));
	CuAssertIntEquals(qt, 1, BITTABLE_GET(tt, 1));
	CuAssertIntEquals(qt, 0, BITTABLE_GET(tt, 2));
}
void tBT_Set(CuTest *qt)
{
	CuAssertHex32Equals(qt, 0x80000400, BT_Set(0x00000400, 0));
	CuAssertHex32Equals(qt, 0x00000401, BT_Set(0x00000400, 31));
}
void tBT_Clear(CuTest *qt)
{
	CuAssertHex32Equals(qt, 0x70ffff0f, BT_Clear(0xf0ffff0f, 0));
	CuAssertHex32Equals(qt, 0xf0ffff0e, BT_Clear(0xf0ffff0f, 31));
}
void tBITTABLE_GET_SET(CuTest *qt)
{
	BITTABLE_DECLARE(tt, 24);
	BITTABLE_INIT(tt, 0);

	BITTABLE_SET(tt, 20, -1);
	BITTABLE_SET(tt, 0, 123123);
	BITTABLE_SET(tt, 16, 1);
	BITTABLE_SET(tt, 17, 1);
	BITTABLE_SET(tt, 18, 1);
	BITTABLE_SET(tt, 17, 0);

	CuAssertIntEquals(qt, 1, BITTABLE_GET(tt, 0));
	CuAssertIntEquals(qt, 0, BITTABLE_GET(tt, 1));
	CuAssertIntEquals(qt, 0, BITTABLE_GET(tt, 15));
	CuAssertIntEquals(qt, 1, BITTABLE_GET(tt, 16));
	CuAssertIntEquals(qt, 0, BITTABLE_GET(tt, 17));
	CuAssertIntEquals(qt, 1, BITTABLE_GET(tt, 18));
	CuAssertIntEquals(qt, 0, BITTABLE_GET(tt, 19));
	CuAssertIntEquals(qt, 1, BITTABLE_GET(tt, 20));
	CuAssertIntEquals(qt, 0, BITTABLE_GET(tt, 21));
}
CuSuite* tBitTable_GetSuite(void)
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, tBITTABLE_GET_SET);
	SUITE_ADD_TEST(suite, tBT_Set);
	SUITE_ADD_TEST(suite, tBT_Clear);
	SUITE_ADD_TEST(suite, tBT_ShiftDownAmount);
	SUITE_ADD_TEST(suite, tBT_NumToBitNum);
	SUITE_ADD_TEST(suite, tBITTABLE_GET);
	SUITE_ADD_TEST(suite, tBitTable_Supported);
	SUITE_ADD_TEST(suite, tBitTable_TableSize);
	SUITE_ADD_TEST(suite, tBitTable_InitedToZero);
	SUITE_ADD_TEST(suite, tBitTable_InitedToOne);
	SUITE_ADD_TEST(suite, tBitTable_InitedToOneB);
	SUITE_ADD_TEST(suite, tBitTable_BT_uNEEDEDFORBITS);
	return suite;
}
