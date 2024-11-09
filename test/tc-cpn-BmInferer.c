// BbMm
#include "bbmm.h"
#include "xtend-bbmm-variadic.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmInferer_init)
{
    BmInferer* trans;
    {
        BmCode* state= newBmCode_list(2,  2, 4 );
        BmCode* action= newBmCode_list(1,  8 );

        trans= newBmInfererStateAction( state, action );

        deleteBmCode(state);
        deleteBmCode(action);
    }

    ck_assert_uint_eq( BmInferer_inputDimention(trans), 3 );
    ck_assert_uint_eq( BmInferer_outputDimention(trans), 2 );
    ck_assert_uint_eq( BmInferer_overallDimention(trans), 5 );
    
    char buffer[1024]= "";
    ck_assert_str_eq( BmBench_printCodes( trans->network, buffer), "{[], [], [], [], []}" );

    deleteBmInferer(trans);
}
END_TEST

BmInferer* test_newBmInfererExemple()
{
    /* Test bayesian network (with 8 nodes):
     * 
     *         3(2) 4(4) 5(2)
     *         |     \    \
     * 1(2) ---+--------------> 7
     *   \     |
     *     ----6(2)
     *          \
     * 2(2)--------------------> 8
     * 
     */
    
    BmCode* state= newBmCode_all( 2, 2 );
    BmCode* action= newBmCode_list(3, 2, 4, 2 );
    BmCode* shift= newBmCode_all( 1, 2 );

    BmInferer*  trans= newBmInfererStateActionShift( state, action, shift );

    deleteBmCode(state);
    deleteBmCode(action);
    deleteBmCode(shift);

    return trans;
}

START_TEST(test_BmInferer_construction)
{
    BmInferer* trans= test_newBmInfererExemple();

    char buffer[1024]= "";
    ck_assert_str_eq( BmBench_printNetwork( trans->network, buffer ), "1[], 2[], 3[], 4[], 5[], 6[], 7[], 8[]" );

    {
        BmBench* distrib= newBmBench_codeDim_vectorDim( 1, 1, 1 );
        BmBench_attachCode_vector( distrib, newBmCode_list(1, 1), newBmVector_list( 1, 1.0 ) );
        BmInferer_node_reinitWith_withDefault(trans, 6,
            newBmCode_list(2, 1, 3),
            distrib
        );
    }
    {
        BmBench* distrib= newBmBench_codeDim_vectorDim( 1, 1, 1 );
        BmBench_attachCode_vector( distrib, newBmCode_list(1, 1), newBmVector_list( 1, 1.0 ) );
        BmInferer_node_reinitWith_withDefault(trans, 7,
            newBmCode_list(3, 1, 4, 5),
            distrib
        );
    }
    {
        BmBench* distrib= newBmBench_codeDim_vectorDim( 1, 1, 1 );
        BmBench_attachCode_vector( distrib, newBmCode_list(1, 1), newBmVector_list( 1, 1.0 ) );
        BmInferer_node_reinitWith_withDefault(trans, 8,
            newBmCode_list(2, 2, 6),
            distrib
        );
    }
    
    strcpy(buffer, "");
    ck_assert_str_eq( BmBench_printNetwork( trans->network, buffer ), "1[], 2[], 3[], 4[], 5[], 6[1, 3], 7[1, 4, 5], 8[2, 6]" );

    strcpy(buffer, "");
    ck_assert_str_eq(
        BmCode_print( BmCondition_parents( (BmCondition*)array_on( trans->nodes, 7) ), buffer ),
        "[2, 4, 2]"
    );

    deleteBmInferer(trans);
}
END_TEST

void test_initializeNode6(BmInferer * trans)
{
/*
    (1) -- (6) | default -> [ 1: 0.5, 2: 0.5 ]
         /     | 1, 1    -> [ 1: 0.9, 2: 0.1 ]
      (3)      |
*/

    BmBench* distrib= newBmBench(2);
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 1), newBmVector_list(1, 0.5) );
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 2), newBmVector_list(1, 0.5) );

    BmCondition * dep= BmInferer_node_reinitWith_withDefault(
        trans, 6,
        newBmCode_list(2, 1, 3),
        distrib
    );

    distrib= newBmBench(2);
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 1), newBmVector_list(1, 0.9) );
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 2), newBmVector_list(1, 0.1) );

    BmCode* inputCondition= newBmCode_list(2, 1, 1);
    BmCondition_from_attach(dep, inputCondition, distrib);

    deleteBmCode(inputCondition);
/*
    BmInferer_node_depends(trans, 6, 2, 1, 3);

    BmCondition * dep= BmInferer_nodeAt( trans, 6 );
    BmDistribution* distrib= newBmDistribution(1);

    BmDistribution_addOutput( distrib, 1, 0.5 );
    BmDistribution_addOutput( distrib, 2, 0.5 );
    BmCondition_reinitializeDefaultDistrib( dep, distrib );

    BmDistribution_initialize(distrib, 1);
    BmDistribution_addOutput( distrib, 1, 0.9 );
    BmDistribution_addOutput( distrib, 2, 0.1 );

    BmCode* inputCondition= newBmCode_list(2, 1, 1);
    BmCondition_from_set(dep, inputCondition, distrib);
    
    deleteBmCode(inputCondition);
    deleteBmDistribution(distrib);
*/
}

void test_initializeNode7(BmInferer * trans)
{
    /*
       (1) 
          \
    (4) -- (7) | default -> [ 2: 0.6, 1: 0.4 ]
          /    | 1, 2, 2 -> [ 1: 0.8, 2: 0.2 ]
       (5)     |
    */

    BmBench* distrib= newBmBench(2);
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 2), newBmVector_list(1, 0.6) );
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 1), newBmVector_list(1, 0.4) );

    BmCondition * dep= BmInferer_node_reinitWith_withDefault(
        trans, 7,
        newBmCode_list(3, 1, 4, 5),
        distrib
    );

    distrib= newBmBench(2);
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 1), newBmVector_list(1, 0.8) );
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 2), newBmVector_list(1, 0.2) );

    BmCode* inputCondition= newBmCode_list(3, 1, 2, 2);
    BmCondition_from_attach(dep, inputCondition, distrib);
    
    deleteBmCode(inputCondition);

/*
    BmInferer_node_depends(trans, 7, 3, 1, 4, 5);

    BmCondition* dep= BmInferer_nodeAt( trans, 7 );
    BmDistribution* distrib= newBmDistribution(1);

    BmDistribution_addOutput( distrib, 2, 0.6 );
    BmDistribution_addOutput( distrib, 1, 0.4 );
    BmCondition_reinitializeDefaultDistrib( dep, distrib );

    BmDistribution_initialize(distrib, 1);
    BmDistribution_addOutput( distrib, 1, 0.8 );
    BmDistribution_addOutput( distrib, 2, 0.2 );

    BmCode* inputCondition= newBmCode_list(3, 1, 2, 2);
    BmCondition_from_set(dep, inputCondition, distrib);
    
    deleteBmCode(inputCondition);
    deleteBmDistribution(distrib);
*/
}

void test_initializeNode8(BmInferer * trans)
{
/*
    (2) -- (8) | default -> [ 1: 0.7, 2: 0.3 ]
         /     |
      (6)      |
*/
    BmBench* distrib= newBmBench(2);
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 1), newBmVector_list(1, 0.7) );
    BmBench_attachCode_vector( distrib, newBmCode_list(1, 2), newBmVector_list(1, 0.3) );

    BmInferer_node_reinitWith_withDefault(
        trans, 8,
        newBmCode_list(2, 2, 6),
        distrib
    );
/*
    BmInferer_node_depends(trans, 8, 2, 2, 6);

    BmCondition* dep= BmInferer_nodeAt(trans, 8);
    BmDistribution* distrib= newBmDistribution(1);

    BmDistribution_addOutput( distrib, 1, 0.7 );
    BmDistribution_addOutput( distrib, 2, 0.3 );
    BmCondition_reinitializeDefaultDistrib( dep, distrib );
    
    deleteBmDistribution(distrib);
*/
}

START_TEST(test_BmInferer_infering)
{
    BmInferer* trans= test_newBmInfererExemple();
    test_initializeNode6(trans);
    test_initializeNode7(trans);
    test_initializeNode8(trans);

    char buffer[1024] = "";
    ck_assert_str_eq( BmBench_printNetwork( trans->network, buffer ), "1[], 2[], 3[], 4[], 5[], 6[1, 3], 7[1, 4, 5], 8[2, 6]" );

    BmCode* state= newBmCode_list(2, 1, 2);
    BmCode* action= newBmCode_list(3, 2, 1, 2);
    BmBench* overallDistrib;

    ck_assert(1);

    // Set initial configuration :
    BmCode* startConf= newBmCode( BmCode_dimention(state) +  BmCode_dimention(action) );

    for( uint i=1 ; i <= BmCode_dimention(state) ; ++i )
        BmCode_at_set( startConf, i, BmCode_digit(state, i) );
    for( uint i=1 ; i <= BmCode_dimention(action) ; ++i )
        BmCode_at_set( startConf,  BmCode_dimention(state)+i, BmCode_digit(action, i) );

    ck_assert(1);
    // Set a initial determinist distribution :
    BmBench * distrib= newBmBench( BmCode_dimention(startConf) );
    BmBench_attachCode_vector(distrib, newBmCodeAs( startConf ), newBmVector_list(1, 1.0) );

    ck_assert(1);
    // infer :
    overallDistrib= BmInferer_process_newOverallDistribution(trans, distrib);
    
    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmBench_print( overallDistrib, buffer ),
        "{[1, 2, 2, 1, 2, 1, 2, 1]:0.21, [1, 2, 2, 1, 2, 1, 2, 2]:0.09, [1, 2, 2, 1, 2, 1, 1, 1]:0.14, [1, 2, 2, 1, 2, 1, 1, 2]:0.06, [1, 2, 2, 1, 2, 2, 2, 1]:0.21, [1, 2, 2, 1, 2, 2, 2, 2]:0.09, [1, 2, 2, 1, 2, 2, 1, 1]:0.14, [1, 2, 2, 1, 2, 2, 1, 2]:0.06}"
    );
    
    BmBench* transDistrib= BmInferer_distribution(trans);
    ck_assert_uint_eq( BmBench_size(transDistrib), 4 );

    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmBench_print( transDistrib, buffer ),
        "{[1, 1]:0.28, [1, 2]:0.12, [2, 1]:0.42, [2, 2]:0.18}"
    );
    
    strcpy( buffer, "" );
    ck_assert_str_eq( BmCode_print( state, buffer ), "[1, 2]" );
    strcpy( buffer, "" );
    ck_assert_str_eq( BmCode_print( action, buffer ), "[2, 1, 2]" );

    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmBench_print( BmInferer_processState_Action(trans, state, action), buffer ),
        "{[1, 1]:0.28, [1, 2]:0.12, [2, 1]:0.42, [2, 2]:0.18}"
    );
    
    // Clean Up :
    deleteBmBench( overallDistrib );
    deleteBmBench(distrib);
    deleteBmCode( startConf );
    deleteBmInferer( trans );
}
END_TEST


START_TEST(test_BmInferer_print)
{
    BmInferer* trans= test_newBmInfererExemple();

    char buffer[1024]= "";
    BmInferer_print(trans, buffer);
    ck_assert_str_eq( buffer,
        "[2, 2]x[2, 4, 2](2) | [] | [], [] |"
    );

    test_initializeNode6(trans);
    test_initializeNode7(trans);
    test_initializeNode8(trans);

    strcpy( buffer,  "" );
    BmInferer_print(trans, buffer);
    ck_assert_str_eq( buffer,
        "[2, 2]x[2, 4, 2](2) | [1, 3] | [1, 4, 5], [2, 6] |"
    );

    deleteBmInferer( trans );
}
END_TEST

/* Printing */

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmInferer(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmInferer_init);
    tcase_add_test(tc, test_BmInferer_construction);
    tcase_add_test(tc, test_BmInferer_infering);
    tcase_add_test(tc, test_BmInferer_print);
    
    return tc;
}
