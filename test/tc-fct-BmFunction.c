// BbMm
#include "bbmm.h"
#include "xtend-bbmm-variadic.h"
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
0. input(1): r(1/1)-[leaf(1), leaf(1)]\n\
---\n\
{}"
    );

    deleteBmFunction( instance );
}

START_TEST(test_BmFunction_construction)
{
    BmFunction* instance= newBmFunctionWith(
        newBmCode_list( 3, 2, 2, 3 ),
        newBmBench_startWithCode_vector(2, newBmCode_list( 2, 1, 1 ), newBmVector_list(1, 1.1) )
    );
    BmTree* selector= BmFunction_selector(instance);
    BmBench* outputs= BmFunction_outputs(instance);

    ck_assert_uint_eq(
        BmFunction_attachOuput( instance, newBmCode_list( 2, 1, 2 ), 2.1 ),
        2 );

    char buffer[2048]= "";

    BmTree_printInside(selector, buffer);
    strcat( buffer, "\n---\n" );
    BmBench_print(outputs, buffer);

    //printf( "<--\n%s\n-->\n", buffer );

    ck_assert_str_eq(
        buffer,
        "input: [2, 2, 3], size: 1\n\
0. input(1): r(1/1)-[leaf(1), leaf(1)]\n\
---\n\
{[1, 1]:1.10, [1, 2]:2.10}"
    );

    BmCode* input= newBmCode_list( 3, 1, 1, 1 );
    BmFunction_from_set( instance, input, 2 );

    strcpy( buffer, "" );
    BmTree_printInside(selector, buffer);
    strcat( buffer, "\n---\n" );
    BmBench_print(outputs, buffer);

    //printf( "<--\n%s\n-->\n", buffer );

    ck_assert_str_eq(
        buffer,
        "input: [2, 2, 3], size: 3\n\
0. input(1): r(1/1)-[branch(1), leaf(1)]\n\
1. input(2): r(1/1)-[branch(2), leaf(1)]\n\
2. input(3): r(1/1)-[leaf(2), leaf(1), leaf(1)]\n\
---\n\
{[1, 1]:1.10, [1, 2]:2.10}"
    );

    ck_assert_double_eq(
        BmFunction_from( instance, input),
        2
    );

    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmCode_print( BmFunction_codeFrom( instance, input ), buffer ),
        "[1, 2]"
    );

    ck_assert_double_eq(
        BmFunction_valueFrom( instance, input ),
        2.1
    );

    BmCode_at_set( input, 1, 2 );

    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmCode_print( BmFunction_codeFrom( instance, input ), buffer ),
        "[1, 1]"
    );

    ck_assert_double_eq(
        BmFunction_valueFrom( instance, input ),
        1.1
    );

    deleteBmCode(input);
    deleteBmFunction( instance );
}

START_TEST(test_BmFunction_print)
{
    BmFunction* instance= newBmFunctionWith(
        newBmCode_list( 3, 2, 2, 3 ),
        newBmBench_startWithCode_vector(2, newBmCode_list( 2, 1, 1 ), newBmVector_list(1, 1.1) )
    );
    BmCode* input= newBmCode_list( 3, 1, 1, 1 );
    BmFunction_from_set(
        instance, input, 
        BmFunction_attachOuput(
            instance, newBmCode_list( 2, 1, 2 ), 2.1 )
    );

    char buffer[1024];
    strcpy( buffer, "" );
    BmFunction_print( instance, buffer );

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
    deleteBmFunction( instance );
}

START_TEST(test_BmFunction_switch)
{
    BmFunction* instance= newBmFunctionWith(
        newBmCode_list( 3, 2, 2, 3 ),
        newBmBench_startWithCode_vector(2, newBmCode_list( 2, 1, 1 ), newBmVector_list(1, 1.1) )
    );
    BmCode* input= newBmCode_list( 3, 1, 1, 1 );
    BmFunction_from_set(
        instance, input, 
        BmFunction_attachOuput(
            instance, newBmCode_list( 2, 1, 2 ), 2.1 )
    );

    BmFunction* doble= newBmFunctionBasic(2);

    BmFunction_switch( instance, doble );

    char buffer[1024];
    strcpy( buffer, "" );
    BmFunction_print( instance, buffer );

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
    BmFunction_print( doble, buffer );
    
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
    deleteBmFunction( instance );
    deleteBmFunction( doble );
}

START_TEST(test_BmFunction_asBench)
{
    // TO DO...
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
    tcase_add_test(tc, test_BmFunction_print);
    tcase_add_test(tc, test_BmFunction_switch);
    tcase_add_test(tc, test_BmFunction_asBench);

    return tc;
}
