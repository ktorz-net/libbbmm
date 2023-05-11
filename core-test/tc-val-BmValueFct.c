// BbMm
#include "bbmm-values.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmValueFct_init)
{
    BmCode * stateSpace= newBmCode_list(3, 2, 4, 6);
    BmCode * actionSpace= newBmCode_list(2, 3, 5);
    BmValueFct* vals= newBmValueFctBasic( stateSpace, actionSpace );

    char buffer[1024]= "";

    BmCode_print( vals->variable, buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5]" );

    deleteBmCode(stateSpace);
    deleteBmCode(actionSpace);
    deleteBmValueFct(vals);
}
END_TEST

START_TEST(test_BmValueFct_construction)
{
}
END_TEST

START_TEST(test_BmValueFct_print)
{
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmValueFct(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmValueFct_init);
    tcase_add_test(tc, test_BmValueFct_construction);
    tcase_add_test(tc, test_BmValueFct_print);
    
    return tc;
}