// BbMm
#include "bbmm-pbb.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmValue_init)
{
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmValue(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmValue_init);
    
    return tc;
}