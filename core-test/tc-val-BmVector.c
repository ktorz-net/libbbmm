// BbMm
#include "bbmm-values.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmVector_init)
{
}
END_TEST

START_TEST(test_BmVector_construction)
{
}
END_TEST

START_TEST(test_BmVector_print)
{
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmVector(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmVector_init);
    tcase_add_test(tc, test_BmVector_construction);
    tcase_add_test(tc, test_BmVector_print);
    
    return tc;
}