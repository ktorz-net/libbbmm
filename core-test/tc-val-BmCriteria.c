// BbMm
#include "bbmm-values.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmCriteria_init)
{
    BmCode* input= newBmCode_list(2, 3, 4 );
    BmCriteria* crit= newBmCriteriaBasic(input, 3);

    ck_assert_uint_eq( BmCriteria_dimention(crit), 2 );
    ck_assert_uint_eq( BmCriteria_optionSize(crit), 3 );

    BmCriteria_optionId_set( crit, 1, 11.0 );
    BmCriteria_optionId_set( crit, 2, 22.0 );
    BmCriteria_optionId_set( crit, 3, 33.0 );

    ck_assert_double_eq( BmCriteria_optionId( crit, 1 ), 11.0 );
    ck_assert_double_eq( BmCriteria_optionId( crit, 2 ), 22.0 );
    ck_assert_double_eq( BmCriteria_optionId( crit, 3 ), 33.0 );

    BmCode* iCode= BmCode_newBmCodeFirst( input );
    while( BmCode_isIncluding( input, iCode ) )
    {
        ck_assert_double_eq( BmCriteria_at( crit, iCode ), 11.0 );
        BmCode_nextCode( input, iCode );
    }
    
    deleteBmCode( iCode );
    deleteBmCode( input );
    deleteBmCriteria( crit );
}
END_TEST


START_TEST(test_BmCriteria_construction)
{
    BmCode* input= newBmCode_list(4, 2, 4, 6, 8 );
    double options[3]= { 1.1, 2.2, 3.3 };
    BmCriteria* crit= newBmCriteria_options( input, 3, options);
    
    ck_assert_double_eq( BmCriteria_optionId( crit, 1 ), 1.1 );
    ck_assert_double_eq( BmCriteria_optionId( crit, 2 ), 2.2 );
    ck_assert_double_eq( BmCriteria_optionId( crit, 3 ), 3.3 );

    BmCode* code= newBmCode_list(4, 0, 1, 0, 4 );
    BmCriteria_at_setOptionId( crit, code, 2 );
    BmCode_initialize_list( code, 4, 1, 1, 2, 0 );    
    BmCriteria_at_setOptionId( crit, code, 3 );

    BmCode_initialize_list( code, 4, 1, 1, 1, 1 );    
    ck_assert_double_eq( BmCriteria_at( crit, code ), 1.1 );

    BmCode_initialize_list( code, 4, 1, 1, 1, 4 );    
    ck_assert_double_eq( BmCriteria_at( crit, code ), 2.2 );

    BmCode_initialize_list( code, 4, 2, 1, 3, 4 );    
    ck_assert_double_eq( BmCriteria_at( crit, code ), 2.2 );

    BmCode_initialize_list( code, 4, 1, 1, 2, 8 );    
    ck_assert_double_eq( BmCriteria_at( crit, code ), 3.3 );

    BmCode_initialize_list( code, 4, 1, 1, 2, 4 );    
    ck_assert_double_eq( BmCriteria_at( crit, code ), 3.3 );

    BmCode_initialize_list( code, 4, 1, 2, 2, 7 );    
    ck_assert_double_eq( BmCriteria_at( crit, code ), 1.1 );
    
    deleteBmCode( code );
    deleteBmCode( input );
    deleteBmCriteria( crit );
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
    tcase_add_test(tc, test_BmCriteria_construction);
    
    return tc;
}