// BbMm
#include "bbmm-pbb.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmTransition_init)
{
    BmTransition* trans;
    {
        BmCode* state= newBmCode(2,  2, 4 );
        BmCode* action= newBmCode(1,  8 );

        trans= newBmTransition( state, action );

        deleteBmCode(state);
        deleteBmCode(action);
    }

    ck_assert_uint_eq( BmTransition_stateDimention(trans), 2 );
    ck_assert_uint_eq( BmTransition_actionDimention(trans), 1 );
    ck_assert_uint_eq( BmTransition_overallDimention(trans), 5 );

    ck_assert_uint_eq( BmTransition_indexOfStateVariableT0(trans, 1), 1 );
    ck_assert_uint_eq( BmTransition_indexOfStateVariableT0(trans, 2), 2 );
    ck_assert_uint_eq( BmTransition_indexOfActionVariable(trans, 1), 3 );
    ck_assert_uint_eq( BmTransition_indexOfStateVariableT1(trans, 1), 4 );
    ck_assert_uint_eq( BmTransition_indexOfStateVariableT1(trans, 2), 5 );
    
    ck_assert_uint_eq( BmTransition_actionDimention(trans), 1 );
    ck_assert_uint_eq( BmTransition_overallDimention(trans), 5 );

    ck_assert_str_eq( BmNet_wording( trans->network ), "1[], 2[], 3[], 4[], 5[]" );

    deleteBmTransition(trans);
}
END_TEST

BmTransition* test_newBmTransitionExemple()
{
    /* test bayesian network (with 8 nodes):
     * 
     *         3(2) 4(2) 5(2)
     *         |     \    \
     * 1(2) ---+--------------> 7
     *   \     |
     *     ----6(2)      
     *          \      
     * 2(2)--------------------> 8
     * 
     */
    
    BmCode* state= newBmCodeBasic( 2, 2 );
    BmCode* action= newBmCode(3, 2, 4, 2 );
    BmCode* shift= newBmCodeBasic( 1, 2 );

    BmTransition*  trans= newBmTransitionWithShift( state, action, shift );

    deleteBmCode(state);
    deleteBmCode(action);
    deleteBmCode(shift);

    return trans;
}

START_TEST(test_BmTransition_construction)
{
    BmTransition* trans= test_newBmTransitionExemple();

    ck_assert_str_eq( BmNet_wording( trans->network ), "1[], 2[], 3[], 4[], 5[], 6[], 7[], 8[]" );

    {
        uint dependancy[2]= {1, 3};
        BmTransition_node_dependArray(trans, 6, 2, dependancy);
    }

    BmTransition_node_depends(
        trans, 7, 3,
        1, 4, 5
    );
    BmTransition_node_depends(
        trans, 8, 2,
        2, 6
    );
    
    ck_assert_str_eq( BmNet_wording( trans->network ), "1[], 2[], 3[], 4[], 5[], 6[1, 3], 7[1, 4, 5], 8[2, 6]" );

    ck_assert_str_eq(
        BmCode_wording( ((BmCondition*)array_on( trans->nodes, 7))->parentRanges ),
        "[2, 4, 2]"
    );


    deleteBmTransition(trans);
}
END_TEST

void test_initializeNode6(BmTransition * trans)
{
    BmTransition_node_depends(trans, 6, 2, 1, 3);

    BmCondition * dep= BmTransition_nodeAt( trans, 6 );
    BmDistribution* distrib= newBmDistribution(1);

    BmDistribution_addOutput( distrib, 1, 0.5 );
    BmDistribution_addOutput( distrib, 2, 0.5 );

    {
        BmCode* code= newBmCode(1);
        BmCode_at_set(code, 1, 1);
        BmCondition_reinitialize( dep, code, distrib );
        deleteBmCode(code);
    }

    BmDistribution_initialize(distrib, 1);
    BmDistribution_addOutput( distrib, 1, 0.9 );
    BmDistribution_addOutput( distrib, 2, 0.1 );

    BmCode* inputCondition= newBmCode(2, 1, 1);
    BmCondition_at_set(dep, inputCondition, distrib);
    
    deleteBmCode(inputCondition);
    deleteBmDistribution(distrib);
}

void test_initializeNode7(BmTransition * trans)
{
    BmTransition_node_depends(trans, 7, 3, 1, 4, 5);

    BmCondition* dep= BmTransition_nodeAt( trans, 7 );
    BmDistribution* distrib= newBmDistribution(1);

    BmDistribution_addOutput( distrib, 2, 0.6 );
    BmDistribution_addOutput( distrib, 1, 0.4 );

    {
        BmCode* code= newBmCode(1);
        BmCode_at_set(code, 1, 1);
        BmCondition_reinitialize( dep, code, distrib );
        deleteBmCode(code);
    }

    BmDistribution_initialize(distrib, 1);
    BmDistribution_addOutput( distrib, 1, 0.8 );
    BmDistribution_addOutput( distrib, 2, 0.2 );

    BmCode* inputCondition= newBmCode(3, 1, 2, 2);
    BmCondition_at_set(dep, inputCondition, distrib);
    
    deleteBmCode(inputCondition);
    deleteBmDistribution(distrib);
}

void test_initializeNode8(BmTransition * trans)
{
    BmTransition_node_depends(trans, 8, 2, 2, 6);

    BmCondition* dep= BmTransition_nodeAt(trans, 8);
    BmDistribution* distrib= newBmDistribution(1);

    BmDistribution_addOutput( distrib, 1, 0.7 );
    BmDistribution_addOutput( distrib, 2, 0.3 );

    {
        BmCode* code= newBmCode(1);
        BmCode_at_set(code, 1, 1);
        BmCondition_reinitialize( dep, code, distrib );
        deleteBmCode(code);
    }
    
    deleteBmDistribution(distrib);
}

START_TEST(test_BmTransition_infering)
{
    BmTransition* trans= test_newBmTransitionExemple();
    test_initializeNode6(trans);
    test_initializeNode7(trans);
    test_initializeNode8(trans);

    ck_assert_str_eq( BmNet_wording( trans->network ), "1[], 2[], 3[], 4[], 5[], 6[1, 3], 7[1, 4, 5], 8[2, 6]" );

    BmCode* state= newBmCode(2, 1, 2);
    BmCode* action= newBmCode(3, 2, 1, 2);
    BmDistribution* overallDistrib;
    {
        // Set initial configuration :
        BmCode* startConf= newBmCodeBasic( BmCode_size(state) +  BmCode_size(action), 0 );

        for( uint i=1 ; i <= BmCode_size(state) ; ++i )
            BmCode_at_set( startConf, i, BmCode_at(state, i) );
        for( uint i=1 ; i <= BmCode_size(action) ; ++i )
            BmCode_at_set( startConf,  BmCode_size(state)+i, BmCode_at(action, i) );

        // Set a initial determinist distribution :
        BmDistribution * distrib= newBmDistribution( BmCode_size(startConf) );
        BmDistribution_addConfig(distrib, startConf, 1.0);

        // infer :
        overallDistrib= BmTransition_newDistributionByInfering(trans, distrib);
        
        // Clean :
        deleteBmDistribution(distrib);
        deleteBmCode( startConf );
    }
    
    ck_assert_str_eq(
    BmDistribution_wording( overallDistrib ),
        "{[1, 2, 2, 1, 2, 1, 2, 1]: 0.21 ; [1, 2, 2, 1, 2, 1, 2, 2]: 0.090 ; [1, 2, 2, 1, 2, 1, 1, 1]: 0.14 ; [1, 2, 2, 1, 2, 1, 1, 2]: 0.060 ; [1, 2, 2, 1, 2, 2, 2, 1]: 0.21 ; [1, 2, 2, 1, 2, 2, 2, 2]: 0.090 ; [1, 2, 2, 1, 2, 2, 1, 1]: 0.14 ; [1, 2, 2, 1, 2, 2, 1, 2]: 0.060}"
        );

    ck_assert_str_eq(
        BmDistribution_wording( BmTransition_distribution(trans) ),
        "{[1, 1]: 0.20 ; [1, 2]: 0.30 ; [2, 1]: 0.20 ; [2, 2]: 0.30}"
    );

    ck_assert_str_eq( BmCode_wording( state ), "[1, 2]" );
    ck_assert_str_eq( BmCode_wording( action ), "[2, 1, 2]" );

    ck_assert_str_eq(
        BmDistribution_wording( BmTransition_inferFromState_andAction(trans, state, action) ),
        "{[1, 1]: 0.20 ; [1, 2]: 0.30 ; [2, 1]: 0.20 ; [2, 2]: 0.30}"
    );
    
    deleteBmDistribution( overallDistrib );
    deleteBmTransition( trans );
}
END_TEST


START_TEST(test_BmTransition_print)
{
    BmTransition* trans= test_newBmTransitionExemple();

    char buffer[1024]= "";
    BmTransition_print(trans, buffer);
    ck_assert_str_eq( buffer,
        "[2, 2]x[2, 4, 2](2) | [] | [], [] |"
    );

    test_initializeNode6(trans);
    test_initializeNode7(trans);
    test_initializeNode8(trans);

    strcpy( buffer,  "" );
    BmTransition_print(trans, buffer);
    ck_assert_str_eq( buffer,
        "[2, 2]x[2, 4, 2](2) | [1, 3] | [1, 4, 5], [2, 6] |"
    );

    deleteBmTransition( trans );
}
END_TEST

/* Printing */

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmTransition(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmTransition_init);
    tcase_add_test(tc, test_BmTransition_construction);
    tcase_add_test(tc, test_BmTransition_infering);
    tcase_add_test(tc, test_BmTransition_print);
    
    return tc;
}
