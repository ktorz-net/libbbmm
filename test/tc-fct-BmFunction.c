// BbMm
#include "bbmm.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmFunction_init)
{
}

START_TEST(test_BmFunction_construction)
{
}

START_TEST(test_BmFunction_switch)
{
}

START_TEST(test_BmFunction_asBench)
{
}

START_TEST(test_BmFunction_print)
{
}


/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmFunction(void)
{
    /* BmFunction test case */
    TCase *tc= tcase_create("BmFunction");

    tcase_add_test(tc, test_BmFunction_init);
    tcase_add_test(tc, test_BmFunction_construction);
    tcase_add_test(tc, test_BmFunction_switch);
    tcase_add_test(tc, test_BmFunction_asBench);
    tcase_add_test(tc, test_BmFunction_print);

    return tc;
}
