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

TCase * test_case_BmBasicPolicy(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmBasicPolicy");
    
    tcase_add_test(tc, test_true);
    
    return tc;
}