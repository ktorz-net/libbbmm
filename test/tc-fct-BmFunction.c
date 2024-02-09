// BbMm
#include "bbmm.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmFunction_init)
{
    BmFunction* instance= newBmFunctionBasic(2);
    BmTree* selector= BmFunction_selector(instance);
    BmBench* outputs= BmFunction_outputs(instance);
    char buffer[2048]= "";

    BmTree_printInside(selector, buffer);
    strcat( buffer, "\n---\n" );
    BmBench_print(outputs, buffer);

    //printf( "<--\n%s\n-->\n", buffer );

    ck_assert_str_eq(
        buffer,
        "input: [2, 2], size: 1\n\
0. input(1): [leaf(1), leaf(1)]\n\
---\n\
{}"
    );

    deleteBmFunction( instance );
}

START_TEST(test_BmFunction_construction)
{
    BmFunction* instance= newBmFunctionWith(
        newBmCode_list( 3, 2, 2, 3 ),
        newBmBenchWith(2, newBmCode_list( 2, 1, 1 ), 1.1 )
    );
    BmTree* selector= BmFunction_selector(instance);
    BmBench* outputs= BmFunction_outputs(instance);

    ck_assert_uint_eq(
        BmBench_attachLast( outputs, newBmCode_list( 2, 1, 2 ), 2.1 ),
        2 );

    char buffer[2048]= "";

    BmTree_printInside(selector, buffer);
    strcat( buffer, "\n---\n" );
    BmBench_print(outputs, buffer);

    //printf( "<--\n%s\n-->\n", buffer );

    ck_assert_str_eq(
        buffer,
        "input: [2, 2, 3], size: 1\n\
0. input(1): [leaf(1), leaf(1)]\n\
---\n\
{[1, 1]:1.10, [1, 2]:2.10}"
    );

    BmCode* input= newBmCode_list( 3, 1, 1, 1 );
    BmTree_at_set( selector, input, 2 );

    strcpy( buffer, "" );
    BmTree_printInside(selector, buffer);
    strcat( buffer, "\n---\n" );
    BmBench_print(outputs, buffer);

    //printf( "<--\n%s\n-->\n", buffer );

    ck_assert_str_eq(
        buffer,
        "input: [2, 2, 3], size: 3\n\
0. input(1): [branch(1), leaf(1)]\n\
1. input(2): [branch(2), leaf(1)]\n\
2. input(3): [leaf(2), leaf(1), leaf(1)]\n\
---\n\
{[1, 1]:1.10, [1, 2]:2.10}"
    );

    deleteBmCode(input);
    deleteBmFunction( instance );
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
