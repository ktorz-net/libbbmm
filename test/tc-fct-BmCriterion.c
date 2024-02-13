// BbMm
#include "bbmm.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmCriterion_init)
{
    BmCriterion* instance= newBmCriterionBasic(1, 1);
    char buffer[1024];

    strcpy(buffer, "");
    ck_assert_str_eq( BmCode_print( BmCriterion_inputRanges(instance), buffer ), "[2]");
    
    strcpy(buffer, "");
    ck_assert_str_eq( BmVector_print( BmCriterion_outputs(instance), buffer ), "[0.00]");

    deleteBmCriterion( instance );
}

START_TEST(test_BmCriterion_construction)
{
    BmCriterion* instance= newBmCriterionWith(
        newBmCode_list(2, 3, 4), 
        newBmVector_list(3, 0.01, 1.0, -3.2)
    );
    char buffer[1024];

    strcpy(buffer, "");
    ck_assert_str_eq( BmCode_print( BmCriterion_inputRanges(instance), buffer ), "[3, 4]");
    
    strcpy(buffer, "");
    ck_assert_str_eq( BmVector_print( BmCriterion_outputs(instance), buffer ), "[0.01, 1.00, -3.20]");

    deleteBmCriterion( instance );
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
    BmCriterion* instance= newBmCriterionWith(
        newBmCode_list(2, 3, 4), 
        newBmVector_list(3, 0.01, 1.0, -3.2)
    );
    char buffer[1024];
    
    BmBench* bench= BmCriterion_asNewBench( instance );

    strcpy(buffer, "");
    ck_assert_str_eq(
        BmBench_print( bench, buffer ),
        "{[1, 0, 1]:0.01, [2, 0, 1]:0.01, [3, 0, 1]:0.01}"
    );

    deleteBmBench( bench );
    deleteBmCriterion( instance );
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
