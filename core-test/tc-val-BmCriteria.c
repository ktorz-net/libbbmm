// BbMm
#include "bbmm-values.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmCriteria_init)
{
    BmCode* input= newBmCode_list(2, 3, 4 );
    BmCriteria* fct= newBmCriteriaBasic(input, 3);

    ck_assert_uint_eq( BmCriteria_dimention(fct), 2 );
    ck_assert_uint_eq( BmCriteria_optionSize(fct), 3 );

    BmCriteria_optionId_set( fct, 1, 11.0 );
    BmCriteria_optionId_set( fct, 2, 22.0 );
    BmCriteria_optionId_set( fct, 3, 33.0 );

    ck_assert_double_eq( BmCriteria_optionId( fct, 1 ), 11.0 );
    ck_assert_double_eq( BmCriteria_optionId( fct, 2 ), 22.0 );
    ck_assert_double_eq( BmCriteria_optionId( fct, 3 ), 33.0 );

    BmCode* iCode= BmCode_newBmCodeFirst( input );
    while( BmCode_isIncluding( input, iCode ) )
    {
        ck_assert_double_eq( BmCriteria_at( fct, iCode ), 11.0 );
        BmCode_nextCode( input, iCode );
    }
    
    deleteBmCode( iCode );
    deleteBmCode( input );
    deleteBmCriteria( fct );
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmCriteria(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmCriteria_init);
    
    return tc;
}