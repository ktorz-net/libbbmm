// BbMm
#include "bbmm.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_true)
{
    ck_assert(1);
}

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmChain(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmChain");

    tcase_add_test(tc, test_true);
    
    return tc;
}