// Wanda
#include "bbmm-mdl.h"
#include "bbmm-test.h"

#include <stdio.h>

/* Simple Example:
 * 
 *         A
 *          \
 *     D -----> D' (Future D)
 *  
 * domD= [1, 2, 3, 4, 5, 6]
 * domA ["Roll", "Score"]
 * 
 * state: D in domD
 * action: A in domA
 * 
 * Transition:
 * D' fct A, D :
 *  "Roll", x : 1-6: 1/6
 *  "Score", 1 : 1: 1
 *  "Score", 2 : 1: 1/10, 2: 9/10
 *  "Score", 3 : 1: 1/10, 3: 9/10
 *  "Score", 4 : 1: 1/10, 4: 9/10
 *  "Score", 5 : 1: 1/10, 5: 9/10
 *  "Score", 6 : 1: 1/10, 6: 9/10
 * 
 * Reward:
 * fct A, D:
 *  "Roll":  -1 
 *  "Score", 1 : 10
 *  "Score", 2 : 20
 *  "Score", 3 : 30
 *  "Score", 4 : 40
 *  "Score", 5 : 50
 *  "Score", 6 : 60
 *  
 */

START_TEST(test_BmSystem_initSpace)
{
    uint go= 1;
    ck_assert(go);

    BmDomain * domD= newBmDomainRange("D", 1, 6, 1);
    BmDomain * domA= newBmDomainWords("A", 2, "Roll", "Score" );

    BmDomain* stateDoms[1]= {domD};
    BmDomain* actionDoms[1]= {domA};

    ck_assert(go);

    BmSystem *model= newBmSystem( 1, stateDoms, 1, actionDoms, 0, NULL );

    ck_assert_uint_eq( BmSpace_dimention( model->state ), 1 );
    ck_assert_uint_eq( BmSpace_dimention( model->action ), 1 );
    ck_assert_uint_eq( BmSpace_dimention( model->shift ), 0 );

    ck_assert(go);

    ck_assert( BmSystem_isValid(model) );

    ck_assert(go);

    char buffer[256];

    /* Look at State Space */
    BmSpace* space= BmSystem_stateSpace(model);

    ck_assert(go);
    
    strcpy(buffer, "");
    BmSpace_print( space, buffer );
    ck_assert_str_eq(
        buffer,
        "D[1, 2, 3, 4, 5, 6]."
    );

    ck_assert(go);

    /* Look at Action Space */
    space=  BmSystem_actionSpace(model);

    strcpy(buffer, "");
    BmSpace_print( space, buffer );
    ck_assert_str_eq(
        buffer,
        "A[Roll, Score]."
    );

    /* Look at Shift Space */

    ck_assert(go);

    BmDomain * domainsBuffer[16];
    uint nboDomains= BmSystem_feedWithDomains( model, domainsBuffer, 0 );
    ck_assert_uint_eq( nboDomains, 2);

    ck_assert_str_eq( BmDomain_wording( array_at(domainsBuffer, 1) ), "D[1, 2, 3, 4, 5, 6]" );
    ck_assert_str_eq( BmDomain_wording( array_at(domainsBuffer, 2) ), "A[Roll, Score]" );
    
    deleteDeepBmSystem(model);
}
END_TEST

START_TEST(test_BmSystem_initSpace2)
{
    uint go= 1;
    ck_assert(go);

    BmSystem *model= newBmSystemEmpty( 1, 1, 0 );

    ck_assert( !BmSystem_isValid(model) );

    ck_assert_uint_eq( BmSystem_attachStateVariable( model, "D", newBmDomainRange("D", 1, 6, 1) ) , 1);
    ck_assert_uint_eq( BmSystem_attachActionVariable( model, "A", newBmDomainWords("A", 2, "Roll", "Score" ) ), 1 );

    ck_assert( BmSystem_isValid(model) );

    ck_assert_uint_eq( BmSpace_dimention( model->state ), 1 );
    ck_assert_uint_eq( BmSpace_dimention( model->action ), 1 );
    ck_assert_uint_eq( BmSpace_dimention( model->shift ), 0 );

    ck_assert_str_eq( BmSpace_variable_name( model->state, 1 ), "D" );
    ck_assert_str_eq( BmSpace_variable_name( model->action, 1 ), "A" );

    ck_assert_uint_eq( BmSystem_variable_nodeId( model, "D" ), 1 );
    ck_assert_uint_eq( BmSystem_variable_nodeId( model, "A" ), 2 );
    ck_assert_uint_eq( BmSystem_variable_nodeId( model, "D'" ), 3 );

    ck_assert_str_eq( BmSystem_nodeId_variableName( model, 1), "D" );
    ck_assert_str_eq( BmSystem_nodeId_variableName( model, 2), "A" );
    ck_assert_str_eq( BmSystem_nodeId_variableName( model, 3), "D'" );

    char buffer[256];

    BmSpace* space= BmSystem_stateSpace(model);
    
    strcpy(buffer, "");
    BmSpace_print( space, buffer );
    ck_assert_str_eq(
        buffer,
        "D[1, 2, 3, 4, 5, 6]."
    );

    space=  BmSystem_actionSpace(model);

    strcpy(buffer, "");
    BmSpace_print( space, buffer );
    ck_assert_str_eq(
        buffer,
        "A[Roll, Score]."
    );

    space= BmSystem_shiftSpace(model);
    
    strcpy(buffer, "");
    BmSpace_print( space, buffer );
    ck_assert_str_eq(
        buffer,
        "."
    );

    strcpy(buffer, "");
    BmTransition_print( model->transition, buffer );
    ck_assert_str_eq(
        buffer,
        "[6]x[2]() | | [] |"
    );

    deleteDeepBmSystem(model);
}
END_TEST

START_TEST(test_BmSystem_initTransition)
{
    uint go= 1;
    ck_assert(go);
    char buffer[1024]= "";

    BmSystem *model= newBmSystemEmpty( 1, 1, 0 );
    ck_assert_uint_eq( BmSystem_attachStateVariable( model, "D", newBmDomainRange("D", 1, 6, 1) ) , 1);
    ck_assert_uint_eq( BmSystem_attachActionVariable( model, "A", newBmDomainWords("A", 2, "Roll", "Score" ) ), 1 );

    strcpy(buffer, "");
    BmTransition_print( model->transition, buffer );
    ck_assert_str_eq(
        buffer,
        "[6]x[2]() | | [] |"
    );

    strcpy(buffer, "");
    BmSystem_printNetwork( model, buffer );
    ck_assert_str_eq(
        buffer,
        "D.6(), A.2(), D'.6()"
    );

    BmCode* code= BmSystem_variable_dependOn( model, "D'", 2, "D", "A" );

    strcpy(buffer, "");
    ck_assert_str_eq( BmCode_print(code, buffer), "[1, 2]" );

    strcpy(buffer, "");
    BmTransition_print( model->transition, buffer );
    ck_assert_str_eq(
        buffer,
        "[6]x[2]() | | [1, 2] |"
    );

    strcpy(buffer, "");
    BmSystem_printNetwork( model, buffer );
    ck_assert_str_eq(
        buffer,
        "D.6(), A.2(), D'.6(D.6, A.2)"
    );

    strcpy(buffer, "");
    BmCondition_print( BmTransition_nodeAt( model->transition, BmSystem_variable_nodeId(model,  "D'") ), buffer );
    tc_print( buffer );
    ck_assert_str_eq(
        buffer,
        "[6, 2]->[6]: {[1, 0]: {[1]: 1.00},\n\
  [2, 0]: {[1]: 1.00},\n\
  [3, 0]: {[1]: 1.00},\n\
  [4, 0]: {[1]: 1.00},\n\
  [5, 0]: {[1]: 1.00},\n\
  [6, 0]: {[1]: 1.00}}"
    );

    strcpy(buffer, "");
    BmSystem_printVariable( model, "D'", buffer );
    ck_assert_str_eq(
        buffer,
        "D'.6(D.6, A.2): {[1, -]: {[1]: 1.00},\n\
  [2, -]: {[1]: 1.00},\n\
  [3, -]: {[1]: 1.00},\n\
  [4, -]: {[1]: 1.00},\n\
  [5, -]: {[1]: 1.00},\n\
  [6, -]: {[1]: 1.00}}"
    );

    /* Transition:
    * D' fct A, D :
    *  "Roll", x : 1-6: 1/6  (Default)
    *  "Score", 1 : 1: 1
    *  "Score", 2 : 1: 1/10, 2: 9/10
    *  "Score", 3 : 1: 1/10, 3: 9/10
    *  "Score", 4 : 1: 1/10, 4: 9/10
    *  "Score", 5 : 1: 1/10, 5: 9/10
    *  "Score", 6 : 1: 1/10, 6: 9/10
    */

    ck_assert(go);

    BmSystem_variable_initializeProbabilities( model, "D'", "A", 6,
        "1", 0.333, "2", 0.333, "3", 0.333, "4", 0.333, "5", 0.333, "6", 0.334 );
    
    strcpy(buffer, "");
    BmSystem_printVariable( model, "D'", buffer );
    //wt_print( buffer );
    ck_assert_str_eq(
        buffer,
        "D'.6(D.6, A.2): {[-, Roll]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33},\n\
  [-, Score]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33}}"
    );

    ck_assert(go);
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "1", 1, "1", 1.0 );

    strcpy(buffer, "");
    BmSystem_printVariable( model, "D'", buffer );
    //wt_print( buffer );
    ck_assert_str_eq(
        buffer, "D'.6(D.6, A.2): {[-, Roll]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33},\n\
  [1, Score]: {[1]: 1.00},\n\
  [2, Score]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33},\n\
  [3, Score]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33},\n\
  [4, Score]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33},\n\
  [5, Score]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33},\n\
  [6, Score]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33}}"
    );

    ck_assert(go);
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "2", 2, "1", 0.1, "2", 0.9 ); 
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "3", 2, "1", 0.1, "3", 0.9 );
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "4", 2, "1", 0.1, "4", 0.9 );
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "5", 2, "1", 0.1, "5", 0.9 );
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "6", 2, "1", 0.1, "6", 0.9 );

    strcpy(buffer, "");
    BmSystem_printVariable( model, "D'", buffer );
    //wt_print( buffer );
    ck_assert_str_eq(
        buffer,
        "D'.6(D.6, A.2): {[-, Roll]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33},\n\
  [1, Score]: {[1]: 1.00},\n\
  [2, Score]: {[1]: 0.10 ; [2]: 0.90},\n\
  [3, Score]: {[1]: 0.10 ; [3]: 0.90},\n\
  [4, Score]: {[1]: 0.10 ; [4]: 0.90},\n\
  [5, Score]: {[1]: 0.10 ; [5]: 0.90},\n\
  [6, Score]: {[1]: 0.10 ; [6]: 0.90}}"
    );

    strcpy(buffer, "");
    BmTransition_print( model->transition, buffer );
    ck_assert_str_eq( buffer, "[6]x[2]() | | [1, 2] |" );

    strcpy(buffer, "");
    BmCondition_print( BmTransition_nodeAt(model->transition, 1), buffer );
    ck_assert_str_eq( buffer, "[1]->[6]: {[1]: {[1]: 1.00}}" );

    strcpy(buffer, "");
    BmCondition_print( BmTransition_nodeAt(model->transition, 2), buffer );
    ck_assert_str_eq( buffer, "[1]->[2]: {[1]: {[1]: 1.00}}" );

    strcpy(buffer, "");
    BmCondition_print( BmTransition_nodeAt(model->transition, 3), buffer );
    ck_assert_str_eq( buffer, "[6, 2]->[6]: {[0, 1]: {[1]: 0.33 ; [2]: 0.33 ; [3]: 0.33 ; [4]: 0.33 ; [5]: 0.33 ; [6]: 0.33},\n\
  [1, 2]: {[1]: 1.00},\n\
  [2, 2]: {[1]: 0.10 ; [2]: 0.90},\n\
  [3, 2]: {[1]: 0.10 ; [3]: 0.90},\n\
  [4, 2]: {[1]: 0.10 ; [4]: 0.90},\n\
  [5, 2]: {[1]: 0.10 ; [5]: 0.90},\n\
  [6, 2]: {[1]: 0.10 ; [6]: 0.90}}" );

    deleteDeepBmSystem(model);
}
END_TEST

START_TEST(test_BmSystem_playTransition)
{
    uint go= 1;
    ck_assert(go);
    //char buffer[1024]= "";

    // Create structure:
    BmSystem *model= newBmSystemEmpty( 1, 1, 0 );
    BmSystem_attachStateVariable( model, "D", newBmDomainRange("Dice", 1, 6, 1) );
    BmSystem_attachActionVariable( model, "A", newBmDomainWords("Action", 2, "Roll", "Score" ) );

    // Set dependancy:
    BmSystem_variable_dependOn( model, "D'", 2, "D", "A" );

    BmSystem_variable_initializeProbabilities( model, "D'", "A", 6, "1", 0.333, "2", 0.333, "3", 0.333, "4", 0.333, "5", 0.333, "6", 0.334 );
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "1", 1, "1", 1.0 );
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "2", 2, "1", 0.1, "2", 0.9 );
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "3", 2, "1", 0.1, "3", 0.9 );
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "4", 2, "1", 0.1, "4", 0.9 );
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "5", 2, "1", 0.1, "5", 0.9 );
    BmSystem_variable_addProbabilities( model, "D'", 2, "A", "Score", "D", "6", 2, "1", 0.1, "6", 0.9 );

    //BmSystem_newDistribution_from_byDoing();

    deleteDeepBmSystem(model);
}
END_TEST

START_TEST(test_BmSystem_initReward)
{
/*
    BmSystem *model= newTestModel();

    deleteBmSystem(model);
*/}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmSystem(void)
{
    /* BmDomain test case */
    TCase *tc= tcase_create("BmSystem");

    tcase_add_test(tc, test_BmSystem_initSpace);
    tcase_add_test(tc, test_BmSystem_initSpace2);
    tcase_add_test(tc, test_BmSystem_initTransition);
    tcase_add_test(tc, test_BmSystem_playTransition);
    tcase_add_test(tc, test_BmSystem_initReward);
    
    return tc;
}
