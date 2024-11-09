// BbMm
#include "bbmm-test.h"
#include "bbmm.h"

#include <stdio.h>

START_TEST(test_sizeof)
{
    ck_assert_uint_eq( sizeof(digit), 2 );
    ck_assert_uint_eq( sizeof(hash), 8 );
}

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_basis(void)
{
    /* BmBasis test case */
    TCase *tc= tcase_create("basis");

    tcase_add_test(tc, test_sizeof);

    return tc;
}
