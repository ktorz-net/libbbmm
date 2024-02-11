// BbMm
#include "bbmm.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmDecision_init)
{
    BmDecision* instance= newBmDecisionBasic(2);
    BmTree* selector= BmDecision_selector(instance);
    BmBench* outputs= BmDecision_outputs(instance);
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

    deleteBmDecision( instance );
}

START_TEST(test_BmDecision_construction)
{
    BmDecision* instance= newBmDecisionWith(
        newBmCode_list( 3, 2, 2, 3 ),
        newBmBenchWith(2, newBmCode_list( 2, 1, 1 ), 1.1 )
    );
    BmTree* selector= BmDecision_selector(instance);
    BmBench* outputs= BmDecision_outputs(instance);

    ck_assert_uint_eq(
        BmDecision_attachOuput( instance, newBmCode_list( 2, 1, 2 ), 2.1 ),
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
    BmDecision_from_set( instance, input, 2 );

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

    ck_assert_double_eq(
        BmDecision_from( instance, input),
        2
    );

    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmCode_print( BmDecision_codeFrom( instance, input ), buffer ),
        "[1, 2]"
    );

    ck_assert_double_eq(
        BmDecision_valueFrom( instance, input ),
        2.1
    );

    BmCode_at_set( input, 1, 2 );

    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmCode_print( BmDecision_codeFrom( instance, input ), buffer ),
        "[1, 1]"
    );

    ck_assert_double_eq(
        BmDecision_valueFrom( instance, input ),
        1.1
    );

    deleteBmCode(input);
    deleteBmDecision( instance );
}

START_TEST(test_BmDecision_print)
{
    BmDecision* instance= newBmDecisionWith(
        newBmCode_list( 3, 2, 2, 3 ),
        newBmBenchWith(2, newBmCode_list( 2, 1, 1 ), 1.1 )
    );
    BmCode* input= newBmCode_list( 3, 1, 1, 1 );
    BmDecision_from_set(
        instance, input, 
        BmDecision_attachOuput(
            instance, newBmCode_list( 2, 1, 2 ), 2.1 )
    );

    char buffer[1024];
    strcpy( buffer, "" );
    BmDecision_print( instance, buffer );

    //printf( "<--\n%s\n-->\n", buffer );

    ck_assert_str_eq(
        buffer,
        "Selector:\n\
{[2, 0, 0]:1,\n\
  [1, 2, 0]:1,\n\
  [0, 1, 1]:2,\n\
  [0, 1, 2]:1,\n\
  [0, 1, 3]:1}\n\
Outputs:\n\
{[1, 1]:1.10, [1, 2]:2.10}"
    );

    deleteBmCode(input);
    deleteBmDecision( instance );
}

START_TEST(test_BmDecision_switch)
{
    BmDecision* instance= newBmDecisionWith(
        newBmCode_list( 3, 2, 2, 3 ),
        newBmBenchWith(2, newBmCode_list( 2, 1, 1 ), 1.1 )
    );
    BmCode* input= newBmCode_list( 3, 1, 1, 1 );
    BmDecision_from_set(
        instance, input, 
        BmDecision_attachOuput(
            instance, newBmCode_list( 2, 1, 2 ), 2.1 )
    );

    BmDecision* doble= newBmDecisionBasic(2);

    BmDecision_switch( instance, doble );

    char buffer[1024];
    strcpy( buffer, "" );
    BmDecision_print( instance, buffer );

    //printf( "<--\n%s\n-->\n", buffer );
    ck_assert_str_eq(
        buffer,
        "Selector:\n\
{[1, 0]:1,\n\
  [2, 0]:1}\n\
Outputs:\n\
{}"
    );

    strcpy( buffer, "" );
    BmDecision_print( doble, buffer );
    
    //printf( "<--\n%s\n-->\n", buffer );
    ck_assert_str_eq(
        buffer,
        "Selector:\n\
{[2, 0, 0]:1,\n\
  [1, 2, 0]:1,\n\
  [0, 1, 1]:2,\n\
  [0, 1, 2]:1,\n\
  [0, 1, 3]:1}\n\
Outputs:\n\
{[1, 1]:1.10, [1, 2]:2.10}"
    );

    deleteBmCode(input);
    deleteBmDecision( instance );
    deleteBmDecision( doble );
}

START_TEST(test_BmDecision_asBench)
{
    // TO DO...
}


/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmDecision(void)
{
    /* BmDecision test case */
    TCase *tc= tcase_create("BmDecision");

    tcase_add_test(tc, test_BmDecision_init);
    tcase_add_test(tc, test_BmDecision_construction);
    tcase_add_test(tc, test_BmDecision_print);
    tcase_add_test(tc, test_BmDecision_switch);
    tcase_add_test(tc, test_BmDecision_asBench);

    return tc;
}
