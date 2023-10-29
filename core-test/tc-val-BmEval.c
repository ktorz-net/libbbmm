// BbMm
#include "bbmm-values.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmEval_init)
{
    BmEval* eval= newBmEvalBasic( 3, 2 );

    char buffer[1024]= "";

    BmCode_print( eval->variable, buffer );
    ck_assert_str_eq( buffer, "[2, 2, 2]" );

    ck_assert_uint_eq( eval->critNumber, 1 );

    strcpy( buffer, "" );
    BmCriteria_print( array_at(eval->criteria, 1), buffer );
    ck_assert_str_eq( buffer, "{}" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[1, 2, 3]" );

    ck_assert_double_eq( array_at(eval->weights, 1), 1.0 );

    deleteBmEval(eval);
}

START_TEST(test_BmEval_initSpace)
{
    BmCode * space= newBmCode_list(5, 2, 4, 6, 3, 5);
    BmEval* eval= newBmEvalSpace( space );

    char buffer[1024]= "";

    BmCode_print( eval->variable, buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5]" );

    ck_assert_uint_eq( eval->critNumber, 1 );

    strcpy( buffer, "" );
    BmCriteria_print( array_at(eval->criteria, 1), buffer );
    ck_assert_str_eq( buffer, "{}" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[1, 2, 3, 4, 5]" );

    ck_assert_double_eq( array_at(eval->weights, 1), 1.0 );

    deleteBmCode(space);
    deleteBmEval(eval);
}

START_TEST(test_BmEval_initStateAction)
{
    BmCode * stateSpace= newBmCode_list(3, 2, 4, 6);
    BmCode * actionSpace= newBmCode_list(2, 3, 5);
    BmEval* eval1= newBmEvalStateAction( stateSpace, actionSpace );

    char buffer[1024]= "";

    BmCode_print( eval1->variable, buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5]" );

    ck_assert_uint_eq( eval1->critNumber, 1 );

    strcpy( buffer, "" );
    BmCriteria_print( array_at(eval1->criteria, 1), buffer );
    ck_assert_str_eq( buffer, "{}" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval1->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[1, 2, 3, 4, 5]" );

    ck_assert_double_eq( array_at(eval1->weights, 1), 1.0 );

    BmEval* eval2= newBmEvalStateActionState( stateSpace, actionSpace );

    strcpy( buffer, "" );
    BmCode_print( eval2->variable, buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5, 2, 4, 6]" );

    deleteBmCode(stateSpace);
    deleteBmCode(actionSpace);
    deleteBmEval(eval1);
    deleteBmEval(eval2);
}
END_TEST

START_TEST(test_BmEval_construction)
{
}
END_TEST

START_TEST(test_BmEval_print)
{
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmEval(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmEval_init);
    tcase_add_test(tc, test_BmEval_initSpace);
    tcase_add_test(tc, test_BmEval_initStateAction);
    tcase_add_test(tc, test_BmEval_construction);
    tcase_add_test(tc, test_BmEval_print);
    
    return tc;
}