// BbMm
#include "bbmm.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmEvaluator_init)
{
    BmEvaluator* eval= newBmEvaluatorBasic( 3, 1 );

    char buffer[1024]= "";

    BmCode_print( BmEvaluator_space(eval), buffer );
    ck_assert_str_eq( buffer, "[2, 2, 2]" );

    ck_assert_uint_eq( BmEvaluator_numberOfCriteria(eval), 1 );

    strcpy( buffer, "" );
    BmTree_print( array_at(eval->criteria, 1), buffer );
    ck_assert_str_eq( buffer, "{}" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[]" );

    strcpy( buffer, "" );
    BmVector_print( BmEvaluator_weights(eval), buffer );
    ck_assert_str_eq( buffer, "[1.00]" );

    deleteBmEvaluator(eval);
}

START_TEST(test_BmEvaluator_initSpace)
{
    BmEvaluator* eval= newBmEvaluatorWith(
        newBmCode_list(5, 2, 4, 6, 3, 5), 1
    );

    char buffer[1024]= "";

    BmCode_print( BmEvaluator_space(eval), buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5]" );

    BmCode* code= newBmCode_list( 5, 1, 2, 1, 3, 4 );

    ck_assert_double_eq_tol( BmEvaluator_process( eval, code ), 0.0, 0.00001 );

    deleteBmEvaluator(eval);
}

START_TEST(test_BmEvaluator_initStateAction)
{
    BmCode * stateSpace= newBmCode_list(3, 2, 4, 6);
    BmCode * actionSpace= newBmCode_list(2, 3, 5);
    BmEvaluator* eval1= newBmEvaluatorWith( 
        newBmCodeMergeList( 2, stateSpace, actionSpace ), 1
    );

    char buffer[1024]= "";

    BmCode_print( BmEvaluator_space(eval1), buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5]" );

    strcpy( buffer, "" );
    BmTree_print( array_at(eval1->criteria, 1), buffer );
    ck_assert_str_eq( buffer, "{}" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval1->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[]" );

    ck_assert_double_eq( BmVector_at( BmEvaluator_weights(eval1), 1), 1.0 );

    ck_assert_double_eq_tol( BmEvaluator_processState_action( eval1, stateSpace, actionSpace ), 0.0, 0.00001 );

    BmEvaluator* eval2= newBmEvaluatorWith( 
        newBmCodeMergeList( 3, stateSpace, actionSpace, stateSpace ), 1 );

    strcpy( buffer, "" );
    BmCode_print( eval2->space, buffer );
    ck_assert_str_eq( buffer, "[2, 4, 6, 3, 5, 2, 4, 6]" );

    ck_assert_double_eq_tol( BmEvaluator_processState_action_state( eval2, stateSpace, actionSpace, stateSpace ), 0.0, 0.00001 );

    deleteBmCode(stateSpace);
    deleteBmCode(actionSpace);
    deleteBmEvaluator(eval1);
    deleteBmEvaluator(eval2);
}
END_TEST

START_TEST(test_BmEvaluator_construction01)
{
/* Test a multi-variate value function:
 * 1.0*(1, 2) + 1.1*(3) + 1.2*(2, 4)
 */
    BmEvaluator* eval= newBmEvaluatorBasic( 4, 1 );
    BmCode_at_set( eval->space, 1, 10 );
    BmCode_at_set( eval->space, 2, 2 );
    BmCode_at_set( eval->space, 3, 3 );
    BmCode_at_set( eval->space, 4, 4 );

    ck_assert_uint_eq( BmCode_dimention( eval->space ), 4 );

    char buffer[1024];
    strcpy( buffer, "" );
    BmCode_print( eval->space, buffer );
    ck_assert_str_eq( buffer, "[10, 2, 3, 4]" );

    ck_assert_uint_eq( BmEvaluator_numberOfCriteria( eval ), 1 );

    // 3 Gauges
    BmEvaluator_reinitCriterion( eval, 3 );

    ck_assert_uint_eq( BmEvaluator_numberOfCriteria( eval ), 3 );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[]" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 2), buffer );
    ck_assert_str_eq( buffer, "[]" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 3), buffer );
    ck_assert_str_eq( buffer, "[]" );

    BmEvaluator_crit_reinitWith(
        eval, 1, 
        newBmCode_list(2, 1, 2),
        2, 0.0
    );
    
    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 1), buffer );
    ck_assert_str_eq( buffer, "[1, 2]" );

    strcpy( buffer, "" );
    BmCode_print( BmEvaluator_crit(eval, 1)->inputSpace, buffer );
    ck_assert_str_eq( buffer, "[10, 2]" );

    BmEvaluator_crit_reinitWith(
        eval, 2, 
        newBmCode_list(1, 3),
        2, 0.0
    );

    BmEvaluator_crit_reinitWith(
        eval, 3, 
        newBmCode_list(2, 2, 4),
        2, 0.0
    );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 2), buffer );
    ck_assert_str_eq( buffer, "[3]" );

    strcpy( buffer, "" );
    BmCode_print( array_at( eval->masks, 3), buffer );
    ck_assert_str_eq( buffer, "[2, 4]" );

    ck_assert_uint_eq( BmEvaluator_numberOfCriteria( eval ), 3 );

    // 3 weights: 

    ck_assert_double_eq_tol( BmEvaluator_crit_weight( eval, 1 ), 1.0, 0.00001 );
    ck_assert_double_eq_tol( BmEvaluator_crit_weight( eval, 2 ), 1.0, 0.00001 );
    ck_assert_double_eq_tol( BmEvaluator_crit_weight( eval, 3 ), 1.0, 0.00001 );

    BmEvaluator_crit_setWeight( eval, 1 , 1.0 );
    BmEvaluator_crit_setWeight( eval, 2 , 2.0 );
    BmEvaluator_crit_setWeight( eval, 3 , 3.0 );

    ck_assert_double_eq_tol( BmEvaluator_crit_weight( eval, 1 ), 1.0, 0.00001 );
    ck_assert_double_eq_tol( BmEvaluator_crit_weight( eval, 2 ), 2.0, 0.00001 );
    ck_assert_double_eq_tol( BmEvaluator_crit_weight( eval, 3 ), 3.0, 0.00001 );

    deleteBmEvaluator( eval );
}
END_TEST

START_TEST(test_BmEvaluator_construction02)
{
    BmEvaluator* eval= newBmEvaluatorWith( 
        newBmCode_list(4, 16, 16, 16, 16), 3 );

    BmCode * code= newBmCode_list( 2, 1, 2 );
    
    // Initialize Criterion 1:
    BmTree* crit= BmEvaluator_crit_reinitWith(
        eval, 1,
        newBmCode_list(2, 1, 2),
        2, 0.0
    );
    BmTree_option_setValue( crit, 2, 1.1 );
    BmTree_at_set( crit, code, 2 );

    // Initialize Criterion 2:
    crit= BmEvaluator_crit_reinitWith(
        eval, 2,
        newBmCode_list(1, 3),
        2, 0.0
    );
    BmTree_option_setValue( crit, 2, 1.0 );
    BmTree_at_set( crit, BmCode_reinit_list(code, 1, 3), 2 );
    BmEvaluator_crit_setWeight( eval, 2, 2.0 );

    // Initialize Criterion 3:
    crit= BmEvaluator_crit_reinitWith(
        eval, 3,
        newBmCode_list(2, 2, 4),
        2, 0.0
    );
    BmTree_option_setValue( crit, 2, 1.0 );
    BmTree_at_set( crit, BmCode_reinit_list(code, 2, 2, 4), 2 );
    BmEvaluator_crit_setWeight( eval, 3, 3.0 );
    
    // Tests:
    code= BmCode_reinit_list( code, 4, 11, 12, 13, 14 );
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
        BmEvaluator_process( eval, code ), 
        0.0, 0.00001 );

    code= BmCode_reinit_list( code, 4, 1, 2, 13, 14 );
    ck_assert_double_eq_tol( 
        BmEvaluator_process( eval, code ), 
        1.1, 0.00001 );
    
    code= BmCode_reinit_list( code, 4, 11, 2, 3, 14 );
    ck_assert_double_eq_tol( 
        BmEvaluator_process( eval, code ), 
        2.0, 0.00001 );

    code= BmCode_reinit_list( code, 4, 11, 2, 3, 4 );
    ck_assert_double_eq_tol( 
        BmEvaluator_process( eval, code ), 
        2.0+3.0, 0.00001 );

    code= BmCode_reinit_list( code, 4, 1, 2, 3, 4 );
    ck_assert_double_eq_tol( 
        BmEvaluator_process( eval, code ), 
        1.1+2.0+3.0, 0.00001 );
    
    deleteBmCode( code );
    deleteBmCode( part );
    deleteBmEvaluator( eval );
}

START_TEST(test_BmEvaluator_print)
{
/*
*/
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmEvaluator(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmEvaluator_init);
    tcase_add_test(tc, test_BmEvaluator_initSpace);
    tcase_add_test(tc, test_BmEvaluator_initStateAction);
    tcase_add_test(tc, test_BmEvaluator_construction01);
    tcase_add_test(tc, test_BmEvaluator_construction02);
    tcase_add_test(tc, test_BmEvaluator_print);
    
    return tc;
}