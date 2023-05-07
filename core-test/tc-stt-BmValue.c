// BbMm
#include "bbmm-probabilities.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmValue_init)
{
    BmCode* input= newBmCode_list(2, 3, 4 );
    BmValue* fct= newBmValueBasic(input, 3);

    ck_assert_uint_eq( BmValue_dimention(fct), 2 );
    ck_assert_uint_eq( BmValue_optionSize(fct), 3 );

    BmValue_optionId_set( fct, 1, 11.0 );
    BmValue_optionId_set( fct, 2, 22.0 );
    BmValue_optionId_set( fct, 3, 33.0 );

    ck_assert_double_eq( BmValue_optionId( fct, 1 ), 11.0 );
    ck_assert_double_eq( BmValue_optionId( fct, 2 ), 22.0 );
    ck_assert_double_eq( BmValue_optionId( fct, 3 ), 33.0 );

    deleteBmCode( input );
    deleteBmValue( fct );
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