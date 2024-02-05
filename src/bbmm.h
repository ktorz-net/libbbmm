/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   libBbMm - a libraray dedicated to Bayesian-based Markov-models.
 * 
 *   STRUCTURE MODULE:
 *       - BmCode         : a fixed size array of digit (unsigned integers)
 *       - BmBench        : a dynamic-size collection of BmCode with and value (i -> code and value )
 *       - BmTree         : a tree based BmCode (code -> output and value )
 *       - BmVector       : a fixed size array of values (doubles)
 * 
 *   FUNCTION MODULE:
 *       - BmCondition    : Define a Bayesian Node (conditional probabilities over variable affectations)
 *       - BmInferer      : Define a Bayesian Network as P(output | input) - potentially Dynamic P(state' | state, action)
 *       - BmEvaluator    : A value function over multiple criteria
 * 
 *   SOLVER MODULE:
 * 
 *   LICENSE: MIT License
 *
 *   Copyright © 2022-2023 Guillaume Lozenguez.
 * 
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom the
 *   Software is furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *   IN THE SOFTWARE.
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#ifndef BBMM_H
#define BBMM_H

#include <stdarg.h>

#define uint unsigned int
#define ulong unsigned long

typedef uint bool;
#define true 1
#define false 0

/* Clasical constructor/dextructor */
#define newEmpty(Type) malloc(sizeof(Type))
#define delEmpty(instance) free(instance)

/* Array manipulation */
#define newEmptyArray(Type, size) malloc( sizeof(Type)*(size+1) )
#define deleteEmptyArray(instance) free(instance)
#define array_on(instance, index) instance+(index-1)
#define array_at(instance, index) instance[index-1]
#define array_at_set(instance, index, value) instance[index-1]= value

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  C O D E                                *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
    uint *dsc;
} BmCode;

/* Constructor */
BmCode* newBmCode(uint size);
BmCode* newBmCode_numbers( uint size, uint* numbers );
BmCode* newBmCode_all(uint size, uint defaultValue);
BmCode* newBmCodeAs( BmCode* model );

BmCode* newBmCode_list( uint size, uint number1, ... );
BmCode* newBmCodeMergeList( uint numberOfCodes, BmCode* code1, ... );

BmCode* BmCode_create( BmCode* self, uint size );
BmCode* BmCode_create_numbers( BmCode* self, uint size, uint* numbers );
BmCode* BmCode_create_all( BmCode* self, uint size, uint defaultValue );
BmCode* BmCode_createAs( BmCode* self, BmCode* model );

BmCode* BmCode_createMerge( BmCode* self, uint numberOfCodes, BmCode ** codes );

/* Destructor */
void deleteBmCode( BmCode* instance);
BmCode* BmCode_destroy( BmCode* self);

/* Accessor */
uint BmCode_dimention( BmCode* self);
uint* BmCode_numbers( BmCode* self);
uint BmCode_at( BmCode* self, uint index);
uint BmCode_count( BmCode* self, uint value);
ulong BmCode_sum( BmCode* self);
ulong BmCode_product( BmCode* self);

/* Re-Initializer */
BmCode* BmCode_reinit( BmCode* self, uint newSize);
BmCode* BmCode_reinit_list( BmCode* self, uint newSize, uint number1, ... );

BmCode* BmCode_copy( BmCode* self, BmCode* model);
BmCode* BmCode_copyNumbers( BmCode* self, BmCode* model);

/* Construction */
BmCode* BmCode_redimention( BmCode* self, uint newSize);
BmCode* BmCode_setAll( BmCode* self, uint value );
BmCode* BmCode_at_set( BmCode* self, uint index, uint value );
BmCode* BmCode_at_increment( BmCode* self, uint index, uint value );
BmCode* BmCode_at_decrement( BmCode* self, uint index, uint value );

BmCode* BmCode_setNumbers( BmCode* self, uint* numbers );

/* Operator */
void BmCode_sort( BmCode* self );
void BmCode_switch( BmCode* self, BmCode* anotherCode );
uint BmCode_search( BmCode* self, uint value );

/* Test */
bool BmCode_isEqualTo( BmCode* self, BmCode* another );
bool BmCode_isGreaterThan( BmCode* self, BmCode* another );
bool BmCode_isSmallerThan( BmCode* self, BmCode* another );

/* As Configuration (a BmCode configuration varring in a space defined by a ranges BmCode 'self' ) */
ulong BmCode_keyOf( BmCode* self, BmCode* code);       // get the key value of the code regarding given ranges ( i.e. 0 <= self.numbers[i] < ranges[i] )

BmCode* BmCode_setCode_onKey( BmCode* self, BmCode* configuration, ulong key ); // set the code as a key value in given ranges
BmCode* BmCode_setCodeFirst( BmCode* self, BmCode* configuration ); // set the code as a key value in given ranges
BmCode* BmCode_setCodeLast( BmCode* self, BmCode* configuration ); // set the code as a key value in given ranges

BmCode* BmCode_newBmCodeOnKey( BmCode* self, ulong key ); // set the code as a key value in given ranges
BmCode* BmCode_newBmCodeFirst( BmCode* self); // set the code as a key value in given ranges
BmCode* BmCode_newBmCodeLast( BmCode* self ); // set the code as a key value in given ranges

BmCode* BmCode_nextCode( BmCode* self, BmCode* configuration ); // set the code as a key value in given ranges
BmCode* BmCode_previousCode( BmCode* self, BmCode* configuration ); // set the code as a key value in given ranges
bool BmCode_isIncluding( BmCode* self, BmCode* configuration ); // set the code as a key value in given ranges

/* Mask */
BmCode* BmCode_newBmCodeMask(BmCode* self, BmCode* mask);

/* Printing */
char* BmCode_print( BmCode* self, char* buffer);   // print `self` at the end of `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  V E C T O R                            *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint size;
  double * values;
} BmVector;

/* Constructor */
BmVector* newBmVector( uint size );
BmVector* newBmVector_values( uint size, double* values );
BmVector* newBmVector_list( uint size, double val1, ... );
BmVector* newBmVector_all( uint size, double value );

BmVector* BmVector_create( BmVector* self, uint size );
BmVector* BmVector_create_values( BmVector* self, uint size, double* values );
BmVector* BmVector_create_all( BmVector* self, uint size, double value );

/* Destructor */
BmVector* BmVector_destroy( BmVector* self );
void deleteBmVector( BmVector* self );

/* Re-Initialize */
BmVector* BmVector_reinit( BmVector* self, uint newSize );
BmVector* BmVector_copy( BmVector* self, BmVector* model );

/* Accessor */
uint BmVector_dimention( BmVector* self );
double BmVector_at( BmVector* self, uint i );
double* BmVector_values( BmVector* self );

/* Construction */
BmVector* BmVector_redimention(BmVector* self, uint size);
double BmVector_at_set( BmVector* self, uint i, double value );
BmVector* BmVector_setValues( BmVector* self, double* values );

/* Operation */
double BmVector_sum( BmVector* self );
double BmVector_product( BmVector* self );

/* Test */
bool BmVector_isEqualTo( BmVector* self, BmVector* another );
bool BmVector_isGreaterThan( BmVector* self, BmVector* another );
bool BmVector_isSmallerThan( BmVector* self, BmVector* another );

/* Printing */
char* BmVector_print( BmVector* self, char* output );
char* BmVector_format_print( BmVector* self, char* format, char* buffer);


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  B E N C H                              *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint capacity, start, size;
  BmCode ** items;
  double * values;
} BmBench;

/* Constructor */
BmBench* newBmBench( uint capacity );
BmBench* newBmBenchWith( uint capacity, BmCode* newFirstItems, double value );
BmBench* newBmBenchAs( BmBench* model );

BmBench* BmBench_create( BmBench* self, uint capacity );
BmBench* BmBench_createWith( BmBench* self, uint capacity, BmCode* newFirstItems, double value );
BmBench* BmBench_createAs( BmBench* self, BmBench* model );

/* Destructor */
BmBench* BmBench_destroy( BmBench* self);
void deleteBmBench( BmBench* self);

/* Re-Initializer */
BmBench* BmBench_reinit( BmBench* self, uint capacity );

/* Accessor */
uint BmBench_size( BmBench* self);
uint BmBench_capacity( BmBench* self);
BmCode* BmBench_at( BmBench* self, uint i );
double BmBench_valueAt( BmBench* self, uint i );
uint BmBench_dimention( BmBench* self);

/* Construction */
void BmBench_resizeCapacity( BmBench* self, uint newCapacity );
uint BmBench_attach( BmBench* self, BmCode* newItem );
//BmCode* BmBench_detach( BmBench* self, uint i );

uint BmBench_attachLast( BmBench* self, BmCode* newItem, double value );
BmCode* BmBench_detachLast( BmBench* self );

uint BmBench_attachFirst( BmBench* self, BmCode* newItem, double value );
BmCode* BmBench_detachFirst( BmBench* self );

BmCode* BmBench_at_setValue( BmBench* self, uint i, double value );

void BmBench_switch( BmBench* self, BmBench* doppleganger);

//void BmBench_add( BmBench *self, BmBench *another );
void BmBench_add_reducted( BmBench* self, BmBench* another, BmCode* mask );

/* Operators */
typedef bool (*fctptr_BmBench_compare)(BmBench*,uint,uint);
uint BmBench_sort( BmBench* self, fctptr_BmBench_compare compare );
uint BmBench_switchCodes( BmBench* self, uint id1, uint id2 );

/* Comparison */
bool BmBench_isGreater(BmBench* self, uint i1, uint i2);
bool BmBench_isSmaller(BmBench* self, uint i1, uint i2);
bool BmBench_isGreaterValue(BmBench* self, uint i1, uint i2);
bool BmBench_isSmallerValue(BmBench* self, uint i1, uint i2);

/* Test */

/* Printing */
char* BmBench_print( BmBench* self, char* output); // print `self` at the end of `output`
char* BmBench_printCodes(BmBench* self, char* output);
char* BmBench_printNetwork(BmBench* self, char* output);


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  T R E E                                *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Apply a tree structure to a Space for clustering states
 * in a finit number of options.
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  BmCode* inputSpace;
  BmVector* outputValues;
  uint bound, capacity, size;
  uint** branches;
} BmTree;

/* Constructor */
BmTree* newBmTree( uint binarySpaceSize, uint optionSize );
BmTree* newBmTreeWith( BmCode* newSpace, uint optionSize );

BmTree* BmTree_createWhith( BmTree* self, BmCode* input, uint optionSize );

/* Destructor */
BmTree* BmTree_destroy( BmTree* self);
void deleteBmTree( BmTree* self );

/* Re-Initializer */
BmTree* BmTree_reinitWith( BmTree* self, BmCode* newSpace, uint optionSize );

BmTree* BmTree_clearWhith_on( BmTree* self, uint index, uint defaultOption );
BmTree* BmTree_clearOn( BmTree* self, uint defaultOption );

/* Accessor */
BmCode* BmTree_inputSpace( BmTree* self );
uint BmTree_outputSize( BmTree* self );
uint BmTree_at( BmTree* self, BmCode* code); // Return the option number of a code/state.
double BmTree_at_value( BmTree* self, BmCode* code); // Return the value of a code/state.

/* Construction */
void BmTree_reziseCapacity( BmTree* self, uint newCapacity );
void BmTree_reziseCompleteCapacity( BmTree* self );

void BmTree_option_setValue( BmTree* self, uint iOption, double value ); // attach a tag and a value to a given option.

uint BmTree_at_set( BmTree* self, BmCode* code, uint output ); // set the ouput value of a code or a partial code (with 0), return the number of potential dead branches
uint BmTree_at_readOrder_set( BmTree* self, BmCode* code, BmCode* codeOrder, uint output );

/* Branch Accessor */
uint BmTree_branchSize( BmTree* self, uint branch ); // Return the number of
uint BmTree_branch_state( BmTree* self, uint iBranch, uint state );
uint BmTree_branchVariable( BmTree* self, uint iBranch ); // Return the variable index represented by the branch.
uint BmTree_deepOf( BmTree* self, BmCode* code); // Return the number of branch to cross before reaching the output.

/* Branch Construction */
uint BmTree_newBranch( BmTree* self, uint iVariable, uint defaultOption);
void BmTree_branch_state_connect( BmTree* self, uint branchA, uint stateA, uint branchB );
void BmTree_branch_state_set( BmTree* self, uint branchA, uint iState, uint outbut );

/* Cleanning */
uint BmTree_cleanDeadBranches( BmTree* self); // Detect and remove detached branches (or BmTree_update, BmTree_regenerate : rebuild the tree without dead branches)
uint BmTree_removeBranch( BmTree* self, uint iBranch); // Remove a branch: (must not change the order or the numerotation of the branch -> maintain a list of removed branches)

/* Generating */
BmBench* BmTree_asNewBench( BmTree* self );
void BmTree_fromBench( BmTree* self, BmBench* model );

/* Printing */
char* BmTree_printBranch( BmTree* self, uint iBranch, char* output );

char* BmTree_print( BmTree* self, char* output);
char* BmTree_print_sep( BmTree* self, char* output, char* separator );
char* BmTree_print_sep_options( BmTree* self, char* output, char* separator, char** optionStrs );
char* BmTree_print( BmTree* self, char* output);

char* BmTree_printInside( BmTree* self, char* output); // print `self` at the end of `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   F U N C T I O N  :  C O N D I T I O N                       *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a Bayesian Node (conditional probabilities over variable affectations)
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint domain;
  BmCode* parentRanges;
  BmTree* selector;
  uint distribSize, distribCapacity;
  BmBench **distributions;
} BmCondition;

/* Constructor */
BmCondition* newBmConditionBasic(uint domain);
BmCondition* newBmConditionWith(uint domain, BmCode* newParentRanges, BmBench* newDefaultDistrib);

BmCondition* BmCondition_createBasic(BmCondition* self, uint domain);
BmCondition* BmCondition_createWith(BmCondition* self, uint domain, BmCode* newParentRanges, BmBench* newDefaultDistrib);

/* Destructor */
BmCondition* BmCondition_destroy(BmCondition* self);
void deleteBmCondition(BmCondition* instance);

/* re-initializer */
uint BmCondition_reinitWith( BmCondition* self, uint domain, BmCode* newParents, BmBench* newDistrib );
uint BmCondition_reinitDistributionsWith( BmCondition* self, BmBench* newDistrib );

/* Accessor */
uint BmCondition_domain( BmCondition* self );
BmCode* BmCondition_parents( BmCondition* self );
BmBench* BmCondition_from( BmCondition* self, BmCode* configuration );
BmBench* BmCondition_fromKey( BmCondition* self, uint configKey );
uint BmCondition_distributionSize( BmCondition* self );
BmBench* BmCondition_distributionAt( BmCondition* self, uint iDistrib );

/* Construction */
uint BmCondition_from_attach( BmCondition* self, BmCode* configuration, BmBench* distribution );

/* Instance tools */
void BmCondition_switch(BmCondition* self, BmCondition* doppelganger);

/* Inferring */
BmBench* BmCondition_infer( BmCondition* self, BmBench* distribOverConfigurations );
BmBench* BmCondition_newDistributionByInfering( BmCondition* self, BmBench* distribOverConfigurations );
BmBench* BmCondition_newDistributionByInfering_mask( BmCondition* self, BmBench* longDistrib, BmCode* parentMask );

/* Printing */
char* BmCondition_print(BmCondition* self, char* output);
char* BmCondition_printSep(BmCondition* self, char* output, char* separator);

char* BmCondition_printExtend(BmCondition* self, char* output); // print `self` at the end of `output`
char* BmCondition_printExtendSep(BmCondition* self, char* output, char* separator);

char* BmCondition_printIdentity( BmCondition* self, char* output ); // print `self` at the end of on `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   F U N C T I O N  :  I N F E R E R                           *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a Bayesian Network composed of state, action and tramsitional nodes
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint inputDimention, outputDimention, overallDimention;
  BmBench* network;
  BmCondition* nodes;
  BmBench* distribution;
} BmInferer;

/* Constructor*/
BmInferer* newBmInferer( BmCode* variableSpace, uint inputDimention, uint outputDimention );
BmInferer* newBmInfererStateAction( BmCode* stateSpace, BmCode* actionSpace );
BmInferer* newBmInfererStateActionShift( BmCode* stateSpace, BmCode* actionSpace, BmCode* shiftSpace );

BmInferer* BmInferer_create( BmInferer* self, BmCode* varDomains, uint inputDimention, uint outputDimention );

/* Destructor */
BmInferer* BmInferer_destroy(BmInferer* self);
void deleteBmInferer(BmInferer* self);

/* Accessor */
BmBench* BmInferer_distribution( BmInferer* self );

uint BmInferer_inputDimention( BmInferer* self );
uint BmInferer_outputDimention( BmInferer* self );
uint BmInferer_shiftDimention( BmInferer* self );
uint BmInferer_overallDimention( BmInferer* self );

BmCondition* BmInferer_node( BmInferer* self, uint iNode );
uint BmInferer_node_size( BmInferer* self, uint iVar );
BmCode* BmInferer_node_parents( BmInferer* self, uint iVar );

/* Construction */
BmCondition* BmInferer_node_reinitIndependant( BmInferer* self, uint index );
BmCondition* BmInferer_node_reinitWith( BmInferer* self, uint index, BmCode* newDependenceList, BmBench* newDefaultDistrib );

/* Process */
BmBench* BmInferer_process( BmInferer* self, BmBench* inputDistribution );        // Return distribution over output varibales
BmBench* BmInferer_process_newOverallDistribution( BmInferer* self, BmBench* inputDistribution ); // Return distribution over all variables
BmBench* BmInferer_processState_Action( BmInferer* self, BmCode* state, BmCode* action ); // Return distribution over statePrime (output)

/* Printing */
char* BmInferer_print(BmInferer* self, char* output); // print `self` at the end of `output`
char* BmInferer_printStateActionSignature(BmInferer* self, char* output); // print `self` at the end of `output`
char* BmInferer_printDependency(BmInferer* self, char* output); // print `self` at the end of `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   F U N C T I O N  :  E V A L U A T O R                       *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a multi-critera Value function
 * (input -> value vector \times weight -> value)
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  BmCode* space;
  uint criteriaSize;
  BmTree ** criteria;
  BmCode ** masks;
  BmVector* weights;
} BmEvaluator ;

/* Constructor*/
BmEvaluator* newBmEvaluatorBasic( uint spaceDimention, uint numberOfCriteria );
BmEvaluator* newBmEvaluatorWith( BmCode* newSpace, uint numberOfCriteria );

BmEvaluator* BmEvaluator_createWith( BmEvaluator* self, BmCode* newSpace, uint numberOfCriteria );

/* Destructor */
void deleteBmEvaluator( BmEvaluator* self );
BmEvaluator* BmEvaluator_destroy( BmEvaluator* self);

/* Accessor */
BmCode* BmEvaluator_space( BmEvaluator* self );
uint BmEvaluator_numberOfCriteria( BmEvaluator* self );
BmTree* BmEvaluator_crit( BmEvaluator* self, uint iCritirion );
BmVector* BmEvaluator_weights( BmEvaluator* self );
double BmEvaluator_crit_weight( BmEvaluator* self, uint iCritirion );

/* Process */
double BmEvaluator_process( BmEvaluator* self, BmCode* input );
double BmEvaluator_processState_action(BmEvaluator* self, BmCode* state, BmCode* action);
double BmEvaluator_processState_action_state(BmEvaluator* self, BmCode* state, BmCode* action, BmCode* statePrime);

double BmEvaluator_crit_process( BmEvaluator* self, uint iCriterion, BmCode* input );

/* Construction */
BmEvaluator* BmEvaluator_reinitCriterion( BmEvaluator* self, uint numberOfCriterion );
BmTree* BmEvaluator_crit_reinitWith( BmEvaluator* self, uint index, BmCode* newDependenceMask, uint numberOfOptions, double defaultValue );
void BmEvaluator_crit_setWeight( BmEvaluator* self, uint iCritirion, double weight );

/* Infering */


/* Printing */


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S O L V E R :  . . .                                        *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */


#endif // BBMM_H
