// BbMm
#include "bbmm-values.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmEval_init)
{
    BmEval* eval= newBmEvalBasic( 3, 1 );

    char buffer[1024]= "";

    BmCode_print( eval->variables, buffer );
    ck_assert_str_eq( buffer, "[2, 2, 2]" );

    ck_assert_uint_eq( BmEval_gaugeSize(eval), 1 );

    strcpy( buffer, "" );
    BmGauge_print( array_at(eval->gauges, 1), buffer );
    ck_assert_str_eq( buffer, "{}" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[1, 2, 3]" );

    ck_assert_double_eq( array_at(eval->weights, 1), 1.0 );

    deleteBmEval(eval);
}

START_TEST(test_BmEval_initSpace)
{
    BmEval* eval= newBmEvalWith( newBmCode_list(5, 2, 4, 6, 3, 5), 1 );

    char buffer[1024]= "";

    BmCode_print( eval->variables, buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5]" );

    ck_assert_uint_eq( BmEval_gaugeSize(eval), 1 );

    strcpy( buffer, "" );
    BmGauge_print( array_at(eval->gauges, 1), buffer );
    ck_assert_str_eq( buffer, "{}" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[1, 2, 3, 4, 5]" );

    ck_assert_double_eq( array_at(eval->weights, 1), 1.0 );

    deleteBmEval(eval);
}

START_TEST(test_BmEval_initStateAction)
{
    BmCode * stateSpace= newBmCode_list(3, 2, 4, 6);
    BmCode * actionSpace= newBmCode_list(2, 3, 5);
    BmEval* eval1= newBmEvalWith( 
        newBmCodeMergeList( 2, stateSpace, actionSpace ), 1 );

    char buffer[1024]= "";

    BmCode_print( eval1->variables, buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5]" );

    ck_assert_uint_eq( BmEval_gaugeSize(eval1), 1 );

    strcpy( buffer, "" );
    BmGauge_print( array_at(eval1->gauges, 1), buffer );
    ck_assert_str_eq( buffer, "{}" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval1->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[1, 2, 3, 4, 5]" );

    ck_assert_double_eq( array_at(eval1->weights, 1), 1.0 );

    BmEval* eval2= newBmEvalWith( 
        newBmCodeMergeList( 3, stateSpace, actionSpace, stateSpace ), 1 );

    strcpy( buffer, "" );
    BmCode_print( eval2->variables, buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5, 2, 4, 6]" );

    deleteBmCode(stateSpace);
    deleteBmCode(actionSpace);
    deleteBmEval(eval1);
    deleteBmEval(eval2);
}
END_TEST

START_TEST(test_BmEval_construction01)
{
/* Test a multi-variate value function:
 * 1.0*(1, 2) + 1.1*(3) + 1.2*(2, 4)
 */
    BmEval* eval= newBmEvalBasic( 4, 1 );
    BmCode_at_set( eval->variables, 1, 2 );
    BmCode_at_set( eval->variables, 2, 2 );
    BmCode_at_set( eval->variables, 3, 3 );
    BmCode_at_set( eval->variables, 4, 4 );

    ck_assert_uint_eq( BmEval_dimention( eval ), 4 );

    char buffer[1024];
    strcpy( buffer, "" );
    BmCode_print( eval->variables, buffer );
    ck_assert_str_eq( buffer, "[2, 2, 3, 4]" );

    ck_assert_uint_eq( BmEval_gaugeSize( eval ), 1 );

    // 3 Gauges
    BmEval_initializeGauges( eval, 3 );

    ck_assert_uint_eq( BmEval_gaugeSize( eval ), 3 );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[1, 2, 3, 4]" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 2), buffer );
    ck_assert_str_eq( buffer, "[1, 2, 3, 4]" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 3), buffer );
    ck_assert_str_eq( buffer, "[1, 2, 3, 4]" );

    BmEval_gaugeAt_initList( eval, 1, 2, 2, 1, 2 );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[1, 2]" );

    BmEval_gaugeAt_initList( eval, 2, 2, 1, 3 );
    BmEval_gaugeAt_initList( eval, 3, 2, 2, 2, 4 );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 2), buffer );
    ck_assert_str_eq( buffer, "[3]" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 3), buffer );
    ck_assert_str_eq( buffer, "[2, 4]" );

    ck_assert_uint_eq( BmEval_gaugeSize( eval ), 3 );

    // 3 weights: 

    ck_assert_double_eq_tol( BmEval_weightAt( eval, 1 ), 1.0, 0.00001 );
    ck_assert_double_eq_tol( BmEval_weightAt( eval, 2 ), 1.0, 0.00001 );
    ck_assert_double_eq_tol( BmEval_weightAt( eval, 3 ), 1.0, 0.00001 );

    BmEval_weightAt_set( eval, 1 , 1.0 );
    BmEval_weightAt_set( eval, 2 , 2.0 );
    BmEval_weightAt_set( eval, 3 , 3.0 );

    ck_assert_double_eq_tol( BmEval_weightAt( eval, 1 ), 1.0, 0.00001 );
    ck_assert_double_eq_tol( BmEval_weightAt( eval, 2 ), 2.0, 0.00001 );
    ck_assert_double_eq_tol( BmEval_weightAt( eval, 3 ), 3.0, 0.00001 );

    deleteBmEval( eval );
}
END_TEST

START_TEST(test_BmEval_construction02)
{
    BmEval* eval= newBmEvalWith( 
        newBmCode_list(4, 16, 16, 16, 16), 3 );

    BmCode * code= newBmCode_list( 2, 1, 2 );
    BmEval_gaugeAt_initList( eval, 1, 2, 2, 1, 2);
    BmEval_weightAt_set( eval, 1 , 1.1 );
    BmGauge_optionId_set( BmEval_gaugeAt(eval, 1), 2, 1.0 );
    
    BmGauge_at_setOptionId( BmEval_gaugeAt( eval, 1), code, 2 );

    BmEval_gaugeAt_initList( eval, 2, 2, 1, 3 );
    BmEval_weightAt_set( eval, 2 , 2.0 );
    BmGauge_optionId_set( BmEval_gaugeAt( eval, 2), 2, 1.0 );
    
    BmGauge_at_setOptionId( BmEval_gaugeAt( eval, 2), BmCode_initialize_list(code, 1, 3), 2 );
    
    BmEval_gaugeAt_initList( eval, 3, 2, 2, 2, 4 );
    BmEval_weightAt_set( eval, 3 , 3.0 );
    BmGauge_optionId_set( BmEval_gaugeAt( eval, 3), 2, 1.0 );

    BmGauge_at_setOptionId( BmEval_gaugeAt( eval, 3), BmCode_initialize_list(code, 2, 2, 4), 2 );

    code= BmCode_initialize_list( code, 4, 11, 12, 13, 14 );
    BmCode* part= BmCode_newBmCodeMask( code, array_at( eval->masks, 1) );

    char buffer[1024];
    strcpy( buffer, "" );
    BmCode_print( part, buffer );
    ck_assert_str_eq( buffer, "[11, 12]" );

    deleteBmCode( part );
    part= BmCode_newBmCodeMask( code, array_at( eval->masks, 2) );

    strcpy( buffer, "" );
    BmCode_print( part, buffer );
    ck_assert_str_eq( buffer, "[13]" );
    
    deleteBmCode( part );
    part= BmCode_newBmCodeMask( code, array_at( eval->masks, 3) );

    strcpy( buffer, "" );
    BmCode_print( part, buffer );
    ck_assert_str_eq( buffer, "[12, 14]" );
    
    ck_assert_double_eq_tol( 
        BmEval_valueOf( eval, code ), 
        0.0, 0.00001 );

    code= BmCode_initialize_list( code, 4, 1, 2, 13, 14 );
    ck_assert_double_eq_tol( 
        BmEval_valueOf( eval, code ), 
        1.1, 0.00001 );
    
    code= BmCode_initialize_list( code, 4, 11, 2, 3, 14 );
    ck_assert_double_eq_tol( 
        BmEval_valueOf( eval, code ), 
        2.0, 0.00001 );

    code= BmCode_initialize_list( code, 4, 11, 2, 3, 4 );
    ck_assert_double_eq_tol( 
        BmEval_valueOf( eval, code ), 
        2.0+3.0, 0.00001 );

    code= BmCode_initialize_list( code, 4, 1, 2, 3, 4 );
    ck_assert_double_eq_tol( 
        BmEval_valueOf( eval, code ), 
        1.1+2.0+3.0, 0.00001 );
    
    deleteBmCode( code );
    deleteBmCode( part );
    deleteBmEval( eval );
}

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
    tcase_add_test(tc, test_BmEval_construction01);
    tcase_add_test(tc, test_BmEval_construction02);
    tcase_add_test(tc, test_BmEval_print);
    
    return tc;
}