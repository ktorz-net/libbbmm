// BbMm
#include "bbmm-values.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmMatrice_init)
{
}
END_TEST

START_TEST(test_BmMatrice_construction)
{
}
END_TEST

START_TEST(test_BmMatrice_print)
{
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmMatrice(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmMatrice_init);
    tcase_add_test(tc, test_BmMatrice_construction);
    tcase_add_test(tc, test_BmMatrice_print);
    
    return tc;
}