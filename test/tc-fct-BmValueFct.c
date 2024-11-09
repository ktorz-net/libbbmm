// BbMm
#include "bbmm.h"
#include "xtend-bbmm-variadic.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmValueFct_init)
{
    BmValueFct* instance= newBmValueFctBasic(1, 1);
    char buffer[1024];

    ck_assert_uint_eq( BmValueFct_inputDimention(instance), 1 );
    ck_assert_uint_eq( BmValueFct_outputSize(instance), 1 );

    strcpy(buffer, "");
    ck_assert_str_eq( BmCode_print( BmValueFct_inputRanges(instance), buffer ), "[2]");
    
    strcpy(buffer, "");
    ck_assert_str_eq( BmVector_print( BmValueFct_outputs(instance), buffer ), "[0.00]");

    deleteBmValueFct( instance );
}

START_TEST(test_BmValueFct_construction)
{
    BmValueFct* instance= newBmValueFctWith(
        newBmCode_list(2, 3, 4), 
        newBmVector_list(3, 0.01, 1.0, -3.2)
    );
    char buffer[1024];

    ck_assert_uint_eq( BmValueFct_inputDimention(instance), 2 );
    ck_assert_uint_eq( BmValueFct_outputSize(instance), 3 );

    strcpy(buffer, "");
    ck_assert_str_eq( BmCode_print( BmValueFct_inputRanges(instance), buffer ), "[3, 4]");
    
    strcpy(buffer, "");
    ck_assert_str_eq( BmVector_print( BmValueFct_outputs(instance), buffer ), "[0.01, 1.00, -3.20]");

    deleteBmValueFct( instance );
}

START_TEST(test_BmValueFct_print)
{
    // TO DO...
}

START_TEST(test_BmValueFct_switch)
{
    // TO DO...
}

START_TEST(test_BmValueFct_asBench)
{
    BmValueFct* instance= newBmValueFctWith(
        newBmCode_list(2, 4, 3), 
        newBmVector_list(3, 0.01, 1.0, -3.2)
    );
    char buffer[1024];
    
    BmBench* bench= BmValueFct_asNewBench( instance );

    strcpy(buffer, "");
    ck_assert_str_eq(
        BmBench_print( bench, buffer ),
        "{[1, 0, 1]:0.01, [2, 0, 1]:0.01, [3, 0, 1]:0.01, [4, 0, 1]:0.01}"
    );

    deleteBmBench( bench );
    deleteBmValueFct( instance );
}


TCase * test_case_BmValueFct(void)
{
    /* BmFunction test case */
    TCase *tc= tcase_create("BmFunction");

    tcase_add_test(tc, test_BmValueFct_init);
    tcase_add_test(tc, test_BmValueFct_construction);
    tcase_add_test(tc, test_BmValueFct_print);
    tcase_add_test(tc, test_BmValueFct_switch);
    tcase_add_test(tc, test_BmValueFct_asBench);

    return tc;
}
