// BbMm
#include "bbmm.h"
#include "xtend-bbmm-variadic.h"
#include "bbmm-test.h"

#include <stdlib.h>
#include <stdio.h>

START_TEST(test_BmTree_init)
{
    BmTree* tree= newBmTreeWith( newBmCode_list(3, 2, 3, 2));

    ck_assert_uint_eq( BmCode_dimention( BmTree_inputRanges(tree) ), (uint)3 );
    ck_assert_uint_eq( tree->capacity, (uint)1 );
    ck_assert_uint_eq( tree->size, (uint)0 );
    
    {
        BmCode* config= newBmCode_list(3, 1, 1, 1);
        ck_assert_uint_eq( BmTree_at( tree, config), (uint)1 );

        BmCode_reinit_list( config, 3, 2, 3, 2);
        ck_assert_uint_eq( BmTree_at( tree, config), (uint)1 );
        BmCode_reinit_list( config, 3, 1, 2, 2);
        ck_assert_uint_eq( BmTree_at( tree, config), (uint)1 );

        deleteBmCode(config);
    }
    
    //for( uint i = 1 ; i <= 2 ; ++i )
    //    ck_assert_uint_eq( tree->branches[0][i], (uint)0 );

    BmCode* st= newBmCode(3);
    BmCode_setCodeFirst( BmTree_inputRanges(tree), st);
    while( BmCode_isIncluding( BmTree_inputRanges(tree), st) )
    {
        ck_assert_uint_eq( BmTree_at(tree, st), (uint)1 );
        BmCode_nextCode( BmTree_inputRanges(tree), st);
    }

    deleteBmCode( st );
    deleteBmTree( tree );
}
END_TEST

START_TEST(test_BmTree_branch_full)
{
    BmTree* tree= newBmTreeWith( newBmCode_list(3, 2, 7, 2));
    uint iBranch= BmTree_newBranch_full( tree, 2, 1 );

    ck_assert_uint_eq( iBranch, (uint)0 );

    ck_assert_uint_eq( BmTree_branchVariable( tree, iBranch ), (uint)2 );
    ck_assert_uint_eq( BmTree_branchStart( tree, iBranch ), (uint)1 );
    ck_assert_uint_eq( BmTree_branchBound( tree, iBranch ), (uint)7 );
    ck_assert_uint_eq( BmTree_branchStep( tree, iBranch ), (uint)1 );

    for( uint i = 1 ; i < 8 ; ++i )
    {
        ck_assert_uint_eq( BmTree_branch_stateIsLeaf( tree, iBranch, i ), (uint)(1) );
        ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, i ), (uint)(3+i) );
        ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, i ), (uint)1 );
    }

    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, iBranch ), (uint)1 );

    for( uint i = 1 ; i < 8 ; ++i )
    {
        BmTree_branch_state_setOption( tree, iBranch, i, i );
    }
    
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 1 ), (uint)1 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 2 ), (uint)2 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 3 ), (uint)3 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 4 ), (uint)4 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 5 ), (uint)5 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 6 ), (uint)6 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 7 ), (uint)7 );

    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, iBranch ), (uint)7 );

    //BmTree_branch_state_setOption( tree, b1, 2, 1 );
    
    deleteBmTree( tree );
}
END_TEST

START_TEST(test_BmTree_branch_binary)
{
    BmTree* tree= newBmTreeWith( newBmCode_list(3, 2, 7, 2));
    uint iBranch= BmTree_newBranch_binary_options( tree, 2, 4, 101, 102 );

    ck_assert_uint_eq( iBranch, (uint)0 );

    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 1 ), (uint)4 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 2 ), (uint)4 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 3 ), (uint)4 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 4 ), (uint)5 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 5 ), (uint)5 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 6 ), (uint)5 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 7 ), (uint)5 );

    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 1 ), (uint)101 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 2 ), (uint)101 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 3 ), (uint)101 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 4 ), (uint)102 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 5 ), (uint)102 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 6 ), (uint)102 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 7 ), (uint)102 );

    ck_assert_uint_eq( BmTree_branchVariable( tree, iBranch ), (uint)2 );
    ck_assert_uint_eq( BmTree_branchStart( tree, iBranch ), (uint)1 );
    ck_assert_uint_eq( BmTree_branchBound( tree, iBranch ), (uint)2 );
    ck_assert_uint_eq( BmTree_branchStep( tree, iBranch ), (uint)3 );

    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, iBranch ), (uint)2 );

    for( uint i = 1 ; i < 8 ; ++i )
    {
        BmTree_branch_state_setOption( tree, iBranch, i, i );
    }
    
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 1 ), (uint)3 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 2 ), (uint)3 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 3 ), (uint)3 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 4 ), (uint)7 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 5 ), (uint)7 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 6 ), (uint)7 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 7 ), (uint)7 );

    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, iBranch ), (uint)2 );

    //BmTree_branch_state_setOption( tree, b1, 2, 1 );
    
    deleteBmTree( tree );
}
END_TEST

START_TEST(test_BmTree_branch_pivot)
{
    BmTree* tree= newBmTreeWith( newBmCode_list(3, 2, 7, 2));
    uint iBranch= BmTree_newBranch_pivot_options( tree, 2, 3, 101, 102, 103 );

    ck_assert_uint_eq( iBranch, (uint)0 );

    ck_assert_uint_eq( BmTree_branchVariable( tree, iBranch ), (uint)2 );
    ck_assert_uint_eq( BmTree_branchStart( tree, iBranch ), (uint)2 );
    ck_assert_uint_eq( BmTree_branchBound( tree, iBranch ), (uint)3 );
    ck_assert_uint_eq( BmTree_branchStep( tree, iBranch ), (uint)1 );

    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 1 ), (uint)4 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 2 ), (uint)4 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 3 ), (uint)5 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 4 ), (uint)6 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 5 ), (uint)6 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 6 ), (uint)6 );
    ck_assert_uint_eq( BmTree_branch_stateIndex( tree, iBranch, 7 ), (uint)6 );

    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 1 ), (uint)101 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 2 ), (uint)101 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 3 ), (uint)102 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 4 ), (uint)103 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 5 ), (uint)103 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 6 ), (uint)103 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 7 ), (uint)103 );


    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, iBranch ), (uint)3 );

    for( uint i = 1 ; i < 8 ; ++i )
    {
        BmTree_branch_state_setOption( tree, iBranch, i, i );
    }
    
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 1 ), (uint)2 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 2 ), (uint)2 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 3 ), (uint)3 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 4 ), (uint)7 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 5 ), (uint)7 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 6 ), (uint)7 );
    ck_assert_uint_eq( BmTree_branch_stateOption( tree, iBranch, 7 ), (uint)7 );

    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, iBranch ), (uint)3 );

    //BmTree_branch_state_setOption( tree, b1, 2, 1 );
    
    deleteBmTree( tree );
}
END_TEST

START_TEST(test_BmTree_treeConstruction)
{
    BmTree* tree= newBmTreeWith( newBmCode_list( 3, 2, 3, 2) );
    char buffer[1024]= "";
    
    /* Tree:
     *  root   :           (2)
     *         :          / | \ 
     *         :         1  2  3
     *         :        /   |   \ 
     *  b1, b2 :      (1)  -1-   (3) 
     *         :     / |          | \ 
     *         :    1  2          1  2
     *         :   /   |          |   \ 
     *         : -3-  -1-        -2-  -4-
     */

    uint root= BmTree_newBranch_full( tree, 2, 1 );

    ck_assert_uint_eq( root, (uint)0 );
    ck_assert_uint_eq( BmTree_branchVariable(tree, root ), (uint)2 );
    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, root ), (uint)1 );

    strcpy(buffer, "");
    BmTree_printInside(tree, buffer);

    // printf( "---\n%s\n---\n", buffer );

    ck_assert_str_eq( buffer,
        "input: [2, 3, 2], size: 1\n\
0. input(2): r(1/1)-[leaf(1), leaf(1), leaf(1)]" );

    uint b1=  BmTree_newBranch_full( tree, 1, 3 );
    BmTree_branch_state_setOption( tree, b1, 2, 1 );
    
    ck_assert_uint_eq( b1, (uint)1 );
    ck_assert_uint_eq( BmTree_branchVariable(tree, b1 ), (uint)1 );
    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, b1 ), (uint)2 );

    BmTree_branch_state_connect( tree, root, 1, b1 );
    
    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, root ), (uint)2 );

    uint b2=  BmTree_newBranch_full( tree, 3, 2 );
    BmTree_branch_state_setOption( tree, b2, 2, 4 );
    
    ck_assert_uint_eq( b2, (uint)2 );
    ck_assert_uint_eq( BmTree_branchVariable(tree, b2 ), (uint)3 );
    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, b2 ), (uint)2 );

    BmTree_branch_state_connect( tree, root, 3, b2 );
    
    ck_assert_uint_eq( BmTree_branchNumberOfOutputs(tree, root ), (uint)3 );

    strcpy(buffer, "");
    BmTree_printInside(tree, buffer);

    // printf( "---\n%s\n---\n", buffer );

    ck_assert_str_eq( buffer,
        "input: [2, 3, 2], size: 3\n\
0. input(2): r(1/1)-[branch(1), leaf(1), branch(2)]\n\
1. input(1): r(1/1)-[leaf(3), leaf(1)]\n\
2. input(3): r(1/1)-[leaf(2), leaf(4)]" );

    deleteBmTree( tree );
}
END_TEST

START_TEST(test_BmTree_optionSelection)
{
    BmTree* tree= newBmTreeWith( newBmCode_list( 3, 2, 3, 2));

    uint root= BmTree_newBranch_full( tree, 2, 1 );
    uint b1=  BmTree_newBranch_full( tree, 1, 3 );
    BmTree_branch_state_connect( tree, root, 1, b1 );
    uint b2=  BmTree_newBranch_full( tree, 3, 2 );
    BmTree_branch_state_setOption( tree, b2, 2, 4 );
    BmTree_branch_state_connect( tree, root, 3, b2 );
    uint b3=  BmTree_newBranch_full( tree, 3, 1 );
    BmTree_branch_state_setOption( tree, b3, 2, 3 );
    BmTree_branch_state_connect( tree, b1, 2, b3 );
    

    /* Tree:
     *  root   :           (2)
     *         :          / | \ 
     *         :         1  2  3
     *         :        /   |   \ 
     *  b1, b2 :      (1)  -1-   (3) 
     *         :     / |          | \ 
     *         :    1  2          1  2
     *         :   /   |          |   \ 
     *  b3     : -3-  (3)        -2-  -4-
     *         :       | \
     *         :       1  2
     *         :       |   \
     *         :      -1-  -3-
     */
    char buffer[1024]= "";
    ck_assert_str_eq(
        BmTree_printInside(tree, buffer),
        "input: [2, 3, 2], size: 4\n\
0. input(2): r(1/1)-[branch(1), leaf(1), branch(2)]\n\
1. input(1): r(1/1)-[leaf(3), branch(3)]\n\
2. input(3): r(1/1)-[leaf(2), leaf(4)]\n\
3. input(3): r(1/1)-[leaf(1), leaf(3)]" );

    BmCode* st= newBmCode_list(3, 2, 1, 1);
    ck_assert_uint_eq( BmTree_at(tree, st), (uint)1 );
    
    BmCode_reinit_list(st, 3, 1, 1, 1);
    ck_assert_uint_eq( BmTree_at(tree, st), (uint)3 );
    
    BmCode_reinit_list(st, 3, 1, 2, 2);
    ck_assert_uint_eq( BmTree_at(tree, st), (uint)1 );
    
    BmCode_reinit_list(st, 3, 1, 3, 2);
    ck_assert_uint_eq( BmTree_at(tree, st), (uint)4 );

    deleteBmCode( st );
    deleteBmTree( tree );
}
END_TEST


START_TEST(test_BmTree_constructionFromExemple)
{
    BmTree* tree= newBmTreeWith( newBmCode_list( 3, 2, 3, 2));

    ck_assert_uint_eq( tree->size, (uint)0 );

    char buffer[1024]= "";
    ck_assert_str_eq(
        BmTree_printInside(tree, buffer),
        "input: [2, 3, 2], size: 0" );

    /* Tree:
     *                    (2)
     *                   / | \ 
     *                  1  2  3
     *                 /   |   \ 
     *               -1-  -1-   -1-
     */

    BmTree_clearWhith_on( tree, 2, 1 );
    
    strcpy( buffer, "");
    ck_assert_str_eq(
        BmTree_printInside(tree, buffer),
        "input: [2, 3, 2], size: 1\n\
0. input(2): r(1/1)-[leaf(1), leaf(1), leaf(1)]" );

    /* Tree: + [0, 1, 1] on 3
     *
     *               (2)
     *              / | \ 
     *             1  2  3
     *            /   |   \ 
     *          (3)  -1-   -1- 
     *         / |          
     *        1  2          
     *       /   |          
     *     -3-  -1-        
     */

    BmCode *code= newBmCode_list(3,  0, 1, 1 );
    BmTree_at_set( tree, code, 3 );

    strcpy(buffer, "");
    BmTree_printInside(tree, buffer);
    //tc_print(buffer);
    ck_assert_str_eq(
        buffer,
        "input: [2, 3, 2], size: 2\n\
0. input(2): r(1/1)-[branch(1), leaf(1), leaf(1)]\n\
1. input(3): r(1/1)-[leaf(3), leaf(1)]" );

    /* Tree: + [2, 0, 2] on 4
     *
     *               (2)       
     *              / | \     
     *             1  2  3    
     *            /   |   \    
     *          (3)  (1)   (1) 
     *         / |    | \    \  \
     *        1  2    1  2    1  2
     *       /   |    |  |    |   \
     *     -3-  (1)  -1- |   -1-  | 
     *         / |      (3)      (3)
     *       -1--4-      | \      | \
     *                  -1--4-   -1--4-
     * 
     */

    BmCode_reinit_list( code, 3, 2, 0, 2 );
    uint deadBranches= BmTree_at_set( tree, code, 4 );

    ck_assert_uint_eq( deadBranches, 0 );

    strcpy(buffer, "");
    ck_assert_str_eq(
        BmTree_printInside(tree, buffer),
        "input: [2, 3, 2], size: 7\n\
0. input(2): r(1/1)-[branch(1), branch(3), branch(5)]\n\
1. input(3): r(1/1)-[leaf(3), branch(2)]\n\
2. input(1): r(1/1)-[leaf(1), leaf(4)]\n\
3. input(1): r(1/1)-[leaf(1), branch(4)]\n\
4. input(3): r(1/1)-[leaf(1), leaf(4)]\n\
5. input(1): r(1/1)-[leaf(1), branch(6)]\n\
6. input(3): r(1/1)-[leaf(1), leaf(4)]" );

    deleteBmCode(code);
    deleteBmTree(tree);
}
END_TEST

START_TEST(test_BmTree_fromExempleFromScratch)
{
    BmTree* tree= newBmTreeWith( newBmCode_list( 3, 2, 3, 2));

    ck_assert_uint_eq( tree->size, (uint)0 );

    char buffer[1024]= "";
    ck_assert_str_eq(
        BmTree_printInside(tree, buffer),
        "input: [2, 3, 2], size: 0" );
    
    BmCode *code= newBmCode_list(3,  0, 1, 1 );
    BmTree_at_set( tree, code, 3 );

    strcpy(buffer, "");
    BmTree_printInside(tree, buffer);
    ck_assert_str_eq(
        buffer,
        "input: [2, 3, 2], size: 2\n\
0. input(2): r(1/1)-[branch(1), leaf(1), leaf(1)]\n\
1. input(3): r(1/1)-[leaf(3), leaf(1)]" );

    deleteBmCode(code);
    deleteBmTree( tree );
}
END_TEST

START_TEST(test_BmTree_BmBenchInterface)
{
    BmTree* tree= newBmTreeWith( newBmCode_list( 3, 2, 3, 2) );
    char buffer[1024]= "";
    
    BmCode *code= newBmCode_list(3,  0, 1, 1 );
    BmTree_at_set( tree, code, 3 );

    strcpy(buffer, "");
    BmTree_printInside(tree, buffer);
    ck_assert_str_eq(
        buffer,
        "input: [2, 3, 2], size: 2\n\
0. input(2): r(1/1)-[branch(1), leaf(1), leaf(1)]\n\
1. input(3): r(1/1)-[leaf(3), leaf(1)]" );

    BmBench* collection= BmTree_asNewBench( tree );

    strcpy(buffer, "");
    BmBench_print(collection, buffer);
    BmBench_sort( collection, (fctptr_BmBench_compare)BmBench_is_codeGreater );

    strcpy(buffer, "");
    BmBench_print(collection, buffer);
    ck_assert_str_eq( buffer,
        "{[0, 1, 1, 3]:0.00, [0, 1, 2, 1]:0.00, [0, 2, 0, 1]:0.00, [0, 3, 0, 1]:0.00}"
    );

    BmTree_clearWhith_on( tree, 2, 4 );

    strcpy(buffer, "");
    BmTree_printInside(tree, buffer);
    ck_assert_str_eq(
        buffer,
        "input: [2, 3, 2], size: 1\n0. input(2): r(1/1)-[leaf(4), leaf(4), leaf(4)]"
    );

/*    BmTree_fromBench( tree, collection );
 *
 *    strcpy(buffer, "");
 *    BmTree_printInside(tree, buffer);
 *    ck_assert_str_eq(
 *        buffer,
 *        "input: [2, 3, 2], size: 2\n\
 *0. input(2): r(1/1)-[branch(1), leaf(1), leaf(1)]\n\
 *1. input(3): r(1/1)-[leaf(3), leaf(1)]"
 *    ); */

    deleteBmBench( collection );
    deleteBmCode( code );
    deleteBmTree( tree );
}
END_TEST

START_TEST(test_BmTree_print)
{
    
    BmTree* tree= newBmTreeWith( newBmCode_list( 3, 2, 3, 2));
    
    /* Tree:
     *  root   :           (2)
     *         :          / | \ 
     *         :         1  2  3
     *         :        /   |   \ 
     *  b1, b2 :      (1)  -1-   (3) 
     *         :     / |          | \ 
     *         :    1  2          1  2
     *         :   /   |          |   \ 
     *         : -3-  -1-        -2-  -4-
     */

    uint root= BmTree_newBranch_full( tree, 2, 1 );

    uint b1=  BmTree_newBranch_full( tree, 1, 3 );
    BmTree_branch_state_setOption( tree, b1, 2, 1 );

    BmTree_branch_state_connect( tree, root, 1, b1 );

    uint b2=  BmTree_newBranch_full( tree, 3, 2 );
    BmTree_branch_state_setOption( tree, b2, 2, 4 );

    BmTree_branch_state_connect( tree, root, 3, b2 );

    char buffer[1024]= "";
    ck_assert_str_eq(
        BmTree_printInside(tree, buffer),
        "input: [2, 3, 2], size: 3\n\
0. input(2): r(1/1)-[branch(1), leaf(1), branch(2)]\n\
1. input(1): r(1/1)-[leaf(3), leaf(1)]\n\
2. input(3): r(1/1)-[leaf(2), leaf(4)]" );

    strcpy(buffer, "");
    BmBench* collection= BmTree_asNewBench( tree );

    BmBench_sort( collection, (fctptr_BmBench_compare)BmBench_is_codeGreater );

    strcpy(buffer, "");
    BmBench_print(collection, buffer);
    ck_assert_str_eq(
        buffer,
        "{[0, 2, 0, 1]:0.00, [0, 3, 1, 2]:0.00, [0, 3, 2, 4]:0.00, [1, 1, 0, 3]:0.00, [2, 1, 0, 1]:0.00}"
    );

    strcpy(buffer, "");
    BmTree_print(tree, buffer);
    ck_assert_str_eq(
        buffer,
        "{[0, 2, 0]:1, [1, 1, 0]:3, [2, 1, 0]:1, [0, 3, 1]:2, [0, 3, 2]:4}"
    );

    strcpy(buffer, "");
    BmCode* code= newBmCode_list(3, 0, 3, 1);
    ck_assert_uint_eq( BmTree_at( tree, code ), 2 ); 

    deleteBmTree( tree );
}
END_TEST

START_TEST(test_BmTree_ordered)
{
    BmTree* tree;
    tree= newBmTreeWith( BmCode_reinit_list( newBmCode(3), 3, 2, 3, 2 ));

    ck_assert_uint_eq( tree->size, (uint)0 );

    char buffer[1024]= "";
    ck_assert_str_eq(
        BmTree_printInside(tree, buffer),
        "input: [2, 3, 2], size: 0" );
    
    BmCode *code= newBmCode_list(3, 2, 1, 1 );
    BmCode *order= newBmCode_list(3, 3, 1, 2 ); // code variable importance 
    BmTree_at_readOrder_set( tree, code, order, 3 );

    strcpy(buffer, "");
    BmTree_printInside(tree, buffer);
    //tc_print(buffer);
    ck_assert_str_eq(
        buffer,
        "input: [2, 3, 2], size: 3\n\
0. input(3): r(1/1)-[branch(1), leaf(1)]\n\
1. input(1): r(1/1)-[leaf(1), branch(2)]\n\
2. input(2): r(1/1)-[leaf(3), leaf(1), leaf(1)]" ); // input should restect the order BmCode.

    deleteBmCode(code);
    deleteBmTree( tree );
}
END_TEST

START_TEST(test_BmTree_deadBranch)
{
}
END_TEST

START_TEST(test_BmTree_optimization)
{
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/
TCase * test_case_BmTree(void)
{
    TCase *tc= tcase_create("BmTree");

    tcase_add_test(tc, test_BmTree_init);
    tcase_add_test(tc, test_BmTree_branch_full);
    tcase_add_test(tc, test_BmTree_branch_binary);
    tcase_add_test(tc, test_BmTree_branch_pivot);
    tcase_add_test(tc, test_BmTree_treeConstruction);
    tcase_add_test(tc, test_BmTree_optionSelection);
    tcase_add_test(tc, test_BmTree_constructionFromExemple);
    tcase_add_test(tc, test_BmTree_fromExempleFromScratch);
    tcase_add_test(tc, test_BmTree_BmBenchInterface);
    tcase_add_test(tc, test_BmTree_print);
    tcase_add_test(tc, test_BmTree_ordered);
    tcase_add_test(tc, test_BmTree_deadBranch);
    tcase_add_test(tc, test_BmTree_optimization);

    return tc;
}
