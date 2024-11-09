// BbMm
#include "bbmm-test.h"
#include "bench.h"

START_TEST(test_bench4_init)
{
    bench4* ben= newBench4();

    ck_assert_uint_eq( ben->start, 0 );
    ck_assert_uint_eq( ben->size, 0 );

    ck_assert_uint_eq( sizeof(ben->_dsc), sizeof(void*)*16 );

    for(uint i=0 ; i < 16 ; ++i)
        ck_assert_uint_eq( (hash)(ben->_dsc[i]), 0 );

    ck_assert_uint_eq( bench4_size(ben), 0 );

    deleteBench4(ben);
    ck_assert( 1 );
}

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_bench(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("CofeeRobot");

    tcase_add_test(tc, test_bench4_init);

    return tc;
}
