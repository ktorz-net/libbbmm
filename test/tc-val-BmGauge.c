// BbMm
#include "bbmm-values.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmGauge_init)
{
    BmCode* input= newBmCode_list(2, 3, 4 );
    BmGauge* crit= newBmGaugeBasic(input, 3);

    ck_assert_uint_eq( BmGauge_dimention(crit), 2 );
    ck_assert_uint_eq( BmGauge_optionSize(crit), 3 );

    BmGauge_optionId_set( crit, 1, 11.0 );
    BmGauge_optionId_set( crit, 2, 22.0 );
    BmGauge_optionId_set( crit, 3, 33.0 );

    ck_assert_double_eq( BmGauge_optionId( crit, 1 ), 11.0 );
    ck_assert_double_eq( BmGauge_optionId( crit, 2 ), 22.0 );
    ck_assert_double_eq( BmGauge_optionId( crit, 3 ), 33.0 );

    BmCode* iCode= BmCode_newBmCodeFirst( input );
    while( BmCode_isIncluding( input, iCode ) )
    {
        ck_assert_double_eq( BmGauge_at( crit, iCode ), 11.0 );
        BmCode_nextCode( input, iCode );
    }
    
    deleteBmCode( iCode );
    deleteBmCode( input );
    deleteBmGauge( crit );
}
END_TEST

START_TEST(test_BmGauge_construction)
{
    BmCode* input= newBmCode_list(4, 2, 4, 6, 8 );
    double options[3]= { 1.1, 2.2, 3.3 };
    BmGauge* crit= newBmGauge_options( input, 3, options);
    
    ck_assert_double_eq( BmGauge_optionId( crit, 1 ), 1.1 );
    ck_assert_double_eq( BmGauge_optionId( crit, 2 ), 2.2 );
    ck_assert_double_eq( BmGauge_optionId( crit, 3 ), 3.3 );

    BmCode* code= newBmCode_list(4, 0, 1, 0, 4 );
    BmGauge_at_setOptionId( crit, code, 2 );
    BmCode_reinit_list( code, 4, 1, 1, 2, 0 );    
    BmGauge_at_setOptionId( crit, code, 3 );

    BmCode_reinit_list( code, 4, 1, 1, 1, 1 );    
    ck_assert_double_eq( BmGauge_at( crit, code ), 1.1 );

    BmCode_reinit_list( code, 4, 1, 1, 1, 4 );    
    ck_assert_double_eq( BmGauge_at( crit, code ), 2.2 );

    BmCode_reinit_list( code, 4, 2, 1, 3, 4 );    
    ck_assert_double_eq( BmGauge_at( crit, code ), 2.2 );

    BmCode_reinit_list( code, 4, 1, 1, 2, 8 );    
    ck_assert_double_eq( BmGauge_at( crit, code ), 3.3 );

    BmCode_reinit_list( code, 4, 1, 1, 2, 4 );    
    ck_assert_double_eq( BmGauge_at( crit, code ), 3.3 );

    BmCode_reinit_list( code, 4, 1, 2, 2, 7 );    
    ck_assert_double_eq( BmGauge_at( crit, code ), 1.1 );
    
    deleteBmCode( code );
    deleteBmCode( input );
    deleteBmGauge( crit );
}
END_TEST

START_TEST(test_BmGauge_print)
{
    BmCode* input= newBmCode_list(4, 2, 4, 6, 8 );
    double options[3]= { 1.1, 2.2, 3.3 };
    BmGauge* crit= newBmGauge_options( input, 3, options);
    char buffer[2048]= "";

    BmGauge_setList( crit, 2,
        0, 1, 0, 4, 2,
        1, 1, 2, 0, 3
    );

    BmGauge_print( crit, buffer );
    ck_assert_str_eq( buffer, "{[0, 2, 0, 0]:1.100000,\n\
[0, 3, 0, 0]:1.100000,\n\
[0, 4, 0, 0]:1.100000,\n\
[2, 0, 0, 1]:1.100000,\n\
[1, 0, 1, 0]:1.100000,\n\
[1, 0, 2, 0]:3.300000,\n\
[1, 0, 3, 0]:1.100000,\n\
[1, 0, 4, 0]:1.100000,\n\
[1, 0, 5, 0]:1.100000,\n\
[1, 0, 6, 0]:1.100000,\n\
[2, 0, 0, 2]:1.100000,\n\
[1, 0, 1, 0]:1.100000,\n\
[1, 0, 2, 0]:3.300000,\n\
[1, 0, 3, 0]:1.100000,\n\
[1, 0, 4, 0]:1.100000,\n\
[1, 0, 5, 0]:1.100000,\n\
[1, 0, 6, 0]:1.100000,\n\
[2, 0, 0, 3]:1.100000,\n\
[1, 0, 1, 0]:1.100000,\n\
[1, 0, 2, 0]:3.300000,\n\
[1, 0, 3, 0]:1.100000,\n\
[1, 0, 4, 0]:1.100000,\n\
[1, 0, 5, 0]:1.100000,\n\
[1, 0, 6, 0]:1.100000,\n\
[2, 0, 0, 4]:2.200000,\n\
[1, 0, 1, 0]:2.200000,\n\
[1, 0, 2, 0]:3.300000,\n\
[1, 0, 3, 0]:2.200000,\n\
[1, 0, 4, 0]:2.200000,\n\
[1, 0, 5, 0]:2.200000,\n\
[1, 0, 6, 0]:2.200000,\n\
[2, 0, 0, 5]:1.100000,\n\
[1, 0, 1, 0]:1.100000,\n\
[1, 0, 2, 0]:3.300000,\n\
[1, 0, 3, 0]:1.100000,\n\
[1, 0, 4, 0]:1.100000,\n\
[1, 0, 5, 0]:1.100000,\n\
[1, 0, 6, 0]:1.100000,\n\
[2, 0, 0, 6]:1.100000,\n\
[1, 0, 1, 0]:1.100000,\n\
[1, 0, 2, 0]:3.300000,\n\
[1, 0, 3, 0]:1.100000,\n\
[1, 0, 4, 0]:1.100000,\n\
[1, 0, 5, 0]:1.100000,\n\
[1, 0, 6, 0]:1.100000,\n\
[2, 0, 0, 7]:1.100000,\n\
[1, 0, 1, 0]:1.100000,\n\
[1, 0, 2, 0]:3.300000,\n\
[1, 0, 3, 0]:1.100000,\n\
[1, 0, 4, 0]:1.100000,\n\
[1, 0, 5, 0]:1.100000,\n\
[1, 0, 6, 0]:1.100000,\n\
[2, 0, 0, 8]:1.100000,\n\
[1, 0, 1, 0]:1.100000,\n\
[1, 0, 2, 0]:3.300000,\n\
[1, 0, 3, 0]:1.100000,\n\
[1, 0, 4, 0]:1.100000,\n\
[1, 0, 5, 0]:1.100000,\n\
[1, 0, 6, 0]:1.100000}"
    );

    deleteBmCode( input );
    deleteBmGauge( crit );
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmGauge(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmGauge_init);
    tcase_add_test(tc, test_BmGauge_construction);
    tcase_add_test(tc, test_BmGauge_print);
    
    return tc;
}