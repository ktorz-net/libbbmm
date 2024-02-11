// BbMm
#include "bbmm.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmCriterion_init)
{
    // TO DO...
}

START_TEST(test_BmCriterion_construction)
{
    // TO DO...
}

START_TEST(test_BmCriterion_print)
{
    // TO DO...
}

START_TEST(test_BmCriterion_switch)
{
    // TO DO...
}

START_TEST(test_BmCriterion_asBench)
{
    // TO DO...
}


TCase * test_case_BmCriterion(void)
{
    /* BmFunction test case */
    TCase *tc= tcase_create("BmFunction");

    tcase_add_test(tc, test_BmCriterion_init);
    tcase_add_test(tc, test_BmCriterion_construction);
    tcase_add_test(tc, test_BmCriterion_print);
    tcase_add_test(tc, test_BmCriterion_switch);
    tcase_add_test(tc, test_BmCriterion_asBench);

    return tc;
}
