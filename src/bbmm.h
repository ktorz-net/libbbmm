/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   libBbMm - a libraray dedicated to Bayesian-based Markov-models.
 * 
 *   STRUCTURE MODULE:
 *       - BmCode         : a fixed size array of digit (unsigned integers)
 *       - BmVector       : a fixed size array of values (doubles)
 *       - BmBench        : a dynamic-size collection of BmCode coupled to BmVector (i -> code and vector OR i -> digit and value)
 *       - BmTree         : a tree based BmCode (input code -> output digit )
 * 
 *   FUNCTION MODULE:
 *       - BmValueFct     : Determine a value from a code (in a given inputRanges)
 *       - BmFunction     : Determine a code+vector from a code
 *       <- BmDistbutor   : Determine a [code+vector distribution] from a code (used as bayesian node) >
 * 
 *   COMPONENT MODULE:
 *       - BmCondition    :
 *       - BmInferer      : Define a Bayesian Network as P(output | input) - potentially Dynamic P(state' | state, action)
 *       - BmEvaluator    : A value function over multiple criteria
 * 
 *   SOLVER MODULE:
 * 
 *   VERSION: 0.0.X
 * 
 *   LICENSE: MIT License
 *
 *   Copyright © 2022-2024 Guillaume Lozenguez.
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

#ifndef uint
#define uint unsigned int
#endif

#include <stdlib.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   B A S I S                                                   *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#ifndef digit
#define digit unsigned short
#endif

#ifndef hash
#define hash unsigned long long
#endif

#ifndef bool
typedef digit bool;
#define true 1
#define false 0
#endif

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
    digit *dsc;
} BmCode;

/* Constructor */
BmCode* newBmCode(digit dimention);
BmCode* newBmCode_numbers( digit dimention, digit* numbers );
BmCode* newBmCode_all(digit dimention, digit defaultValue);
BmCode* newBmCodeAs( BmCode* model );

BmCode* BmCode_create( BmCode* self, digit dimention );
BmCode* BmCode_create_numbers( BmCode* self, digit dimention, digit* numbers );
BmCode* BmCode_create_all( BmCode* self, digit dimention, digit defaultValue );
BmCode* BmCode_createAs( BmCode* self, BmCode* model );

BmCode* BmCode_createMerge( BmCode* self, digit numberOfCodes, BmCode ** codes );

/* Destructor */
void deleteBmCode( BmCode* instance );
BmCode* BmCode_destroy( BmCode* self );

/* Accessor */
digit BmCode_dimention( BmCode* self );
digit BmCode_digit( BmCode* self, digit index );
digit BmCode_count( BmCode* self, digit value );
hash BmCode_sum( BmCode* self );
hash BmCode_product( BmCode* self );

/* Re-Initializer */
BmCode* BmCode_reinit( BmCode* self, digit newDimention );

BmCode* BmCode_copy( BmCode* self, BmCode* model);
BmCode* BmCode_copyNumbers( BmCode* self, BmCode* model);

/* Construction */
BmCode* BmCode_redimention( BmCode* self, digit newDimention);
BmCode* BmCode_setAll( BmCode* self, digit value );
BmCode* BmCode_at_set( BmCode* self, digit index, digit value );
BmCode* BmCode_at_increment( BmCode* self, digit index, digit value );
BmCode* BmCode_at_decrement( BmCode* self, digit index, digit value );

BmCode* BmCode_setNumbers( BmCode* self, digit* numbers );

/* Operator */
void BmCode_sort( BmCode* self );
void BmCode_switch( BmCode* self, BmCode* anotherCode );
digit BmCode_search( BmCode* self, digit value );

/* Test */
bool BmCode_isEqualTo( BmCode* self, BmCode* another );
bool BmCode_isGreaterThan( BmCode* self, BmCode* another );
bool BmCode_isSmallerThan( BmCode* self, BmCode* another );

/* As Configuration (a BmCode configuration varring in a space defined by a ranges BmCode 'self' ) */
hash BmCode_keyOf( BmCode* self, BmCode* code);       // get the key value of the code regarding given ranges ( i.e. 0 <= self.numbers[i] < ranges[i] )

BmCode* BmCode_setCode_onKey( BmCode* self, BmCode* configuration, hash key ); // set the code as a key value in given ranges
BmCode* BmCode_setCodeFirst( BmCode* self, BmCode* configuration ); // set the code as a key value in given ranges
BmCode* BmCode_setCodeLast( BmCode* self, BmCode* configuration ); // set the code as a key value in given ranges

BmCode* BmCode_newBmCodeOnKey( BmCode* self, hash key ); // set the code as a key value in given ranges
BmCode* BmCode_newBmCodeFirst( BmCode* self ); // set the code as a key value in given ranges
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
  digit dimention;
  double * values;
} BmVector;

/* Constructor */
BmVector* newBmVector( digit dimention );
BmVector* newBmVector_values( digit dimention, double* values );
BmVector* newBmVector_all( digit dimention, double value );
BmVector* newBmVectorAs( BmVector* model );

BmVector* BmVector_create( BmVector* self, digit dimention );
BmVector* BmVector_create_values( BmVector* self, digit dimention, double* values );
BmVector* BmVector_create_all( BmVector* self, digit dimention, double value );
BmVector* BmVector_createAs( BmVector* self, BmVector* model );

/* Destructor */
BmVector* BmVector_destroy( BmVector* self );
void deleteBmVector( BmVector* self );

/* Re-Initialize */
BmVector* BmVector_reinit( BmVector* self, digit newDimention );
BmVector* BmVector_copy( BmVector* self, BmVector* model );

/* Accessor */
digit BmVector_dimention( BmVector* self );
double BmVector_value( BmVector* self, digit i );

/* Construction */
BmVector* BmVector_redimention(BmVector* self, digit newDimention);
double BmVector_at_set( BmVector* self, digit i, double value );
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
  uint codeDim, vectDim;
  BmCode   ** codes;
  BmVector ** vects;
} BmBench;

/* Constructor */
BmBench* newBmBench( uint capacity );
BmBench* newBmBench_codeDim_vectorDim( uint capacity, uint codeDim, uint vectorDim );
BmBench* newBmBench_startDigit_value( uint capacity, uint aDigit, double value );
BmBench* newBmBench_startWithCode_vector( uint capacity, BmCode* newCode, BmVector* newVector );
BmBench* newBmBenchAs( BmBench* model );

BmBench* BmBench_create( BmBench* self, uint capacity );
BmBench* BmBench_create_codeDim_vectorDim( BmBench* self, uint capacity, uint codeDim, uint vectorDim );
BmBench* BmBench_createAs( BmBench* self, BmBench* model );

/* Destructor */
BmBench* BmBench_destroy( BmBench* self);
void deleteBmBench( BmBench* self);

/* Re-Initializer */
BmBench* BmBench_reinit( BmBench* self, uint capacity );

/* Accessor */
uint BmBench_size( BmBench* self);

uint BmBench_codeDimention( BmBench* self);
uint BmBench_vectorDimention( BmBench* self);

BmCode* BmBench_codeAt( BmBench* self, uint i );
BmVector* BmBench_vectorAt( BmBench* self, uint i );

uint BmBench_digitAt( BmBench* self, uint i );
double BmBench_valueAt( BmBench* self, uint i );

uint BmBench_at_digit( BmBench* self, uint i, uint j );
double BmBench_at_value( BmBench* self, uint i, uint j );

/* Construction */
//void BmBench_resizeCapacity_start( BmBench* self, uint newCapacity, uint start );
void BmBench_resizeCapacity( BmBench* self, uint newCapacity);
//void BmBench_resizeCapacityFront( BmBench* self, uint newCapacity);
uint BmBench_attachCode_vector( BmBench* self, BmCode* newCode, BmVector* newVector );
uint BmBench_attachFrontCode_vector( BmBench* self, BmCode* newCode, BmVector* newVector );

BmCode* BmBench_detach( BmBench* self );
BmCode* BmBench_detachFront( BmBench* self );
//BmCode* BmBench_detach( BmBench* self, uint i );

BmBench* BmBench_increase( BmBench* self, uint number );
BmBench* BmBench_increaseFront( BmBench* self, uint number );

uint BmBench_attachCode( BmBench* self, BmCode* newItem );
uint BmBench_attachVector( BmBench* self, BmVector* newItem );

void BmBench_switch( BmBench* self, BmBench* doppleganger);

/* Construction Basic */
uint BmBench_addDigit_value( BmBench* self, uint d, double v );
BmBench* BmBench_at_setDigit( BmBench* self, uint i, uint aDigit );
BmBench* BmBench_at_setValue( BmBench* self, uint i, double value );

//void BmBench_add( BmBench *self, BmBench *another );
void BmBench_add_reducted( BmBench* self, BmBench* another, BmCode* mask );

/* Operators */
typedef bool (*fctptr_BmBench_compare)(BmBench*,uint,uint);
uint BmBench_sort( BmBench* self, fctptr_BmBench_compare compare );
uint BmBench_switchAt_at( BmBench* self, uint id1, uint id2 );

/* Comparison */

bool BmBench_is_codeGreater(BmBench* self, uint i1, uint i2);
bool BmBench_is_codeSmaller(BmBench* self, uint i1, uint i2);
bool BmBench_is_vectorGreater(BmBench* self, uint i1, uint i2);
bool BmBench_is_vectorSmaller(BmBench* self, uint i1, uint i2);

/* Test */

/* Printing */
char* BmBench_print( BmBench* self, char* output); // print `self` at the end of `output`
char* BmBench_printCodes(BmBench* self, char* output);
//char* BmBench_printVector(BmBench* self, char* output);
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
  BmCode* inputRanges;
  uint capacity, size;
  uint** branches;    // -> uint* branchvariable AND Reductor** // Funnel** branchSelector AND uint** branches ?
} BmTree;

/* Constructor */
BmTree* newBmTree( uint binarySpaceSize );
BmTree* newBmTreeWith( BmCode* newSpace );

BmTree* BmTree_createWhith( BmTree* self, BmCode* input );

/* Destructor */
BmTree* BmTree_destroy( BmTree* self);
void deleteBmTree( BmTree* self );

/* Re-Initializer */
BmTree* BmTree_reinitWith( BmTree* self, BmCode* newSpace );

BmTree* BmTree_clearWhith_on( BmTree* self, uint index, uint defaultOption );
BmTree* BmTree_clearOn( BmTree* self, uint defaultOption );

/* Accessor */
uint BmTree_dimention( BmTree* self );
uint BmTree_size( BmTree* self );
BmCode* BmTree_inputRanges( BmTree* self );
uint BmTree_at( BmTree* self, BmCode* code ); // Return the option number of a code/state.

/* output */
uint BmTreeChild( uint key );
uint BmTreeLeaf( uint key );

/* Construction */
void BmTree_reziseCapacity( BmTree* self, uint newCapacity );
void BmTree_reziseCompleteCapacity( BmTree* self );

uint BmTree_at_set( BmTree* self, BmCode* code, uint output ); // set the ouput value of a code or a partial code (with 0), return the number of potential dead branches
uint BmTree_at_readOrder_set( BmTree* self, BmCode* code, BmCode* codeOrder, uint output );

/* Branch Accessor */
uint BmTree_branchSize( BmTree* self, uint iBranch );
uint BmTree_branch_stateIndex( BmTree* self, uint iBranch, uint state );
uint BmTree_branch_state( BmTree* self, uint iBranch, uint state );
uint BmTree_branch_stateIsLeaf( BmTree* self, uint iBranch, uint state );
uint BmTree_branch_stateOption( BmTree* self, uint iBranch, uint state );
uint BmTree_branch_stateLeaf( BmTree* self, uint iBranch, uint state );
uint BmTree_branchVariable( BmTree* self, uint iBranch ); // Return the variable index represented by the branch.
uint BmTree_branchStart( BmTree* self, uint iBranch );
uint BmTree_branchBound( BmTree* self, uint iBranch );
uint BmTree_branchStep( BmTree* self, uint iBranch );
uint BmTree_branchNumberOfOutputs( BmTree* self, uint branch ); // Return the number of differents output
uint BmTree_deepOf( BmTree* self, BmCode* code); // Return the number of branch to cross before reaching the output.

/* Branch Construction */
uint BmTree_newBranch( BmTree* self, uint iVariable, uint start, uint bound, uint step );
uint BmTree_newBranch_full( BmTree* self, uint iVariable, uint defaultOption);
uint BmTree_newBranch_binary_options( BmTree* self, uint iVariable, uint  afterValue, uint option1, uint option2);
uint BmTree_newBranch_pivot_options( BmTree* self, uint iVariable, uint onValue, uint option1, uint optionOn, uint option2);
void BmTree_branch_state_connect( BmTree* self, uint branchA, uint stateA, uint branchB );
void BmTree_branch_state_setOption( BmTree* self, uint branchA, uint iState, uint outbut );

/* Cleanning */
uint BmTree_cleanDeadBranches( BmTree* self); // Detect and remove detached branches (or BmTree_update, BmTree_regenerate : rebuild the tree without dead branches)
uint BmTree_removeBranch( BmTree* self, uint iBranch); // Remove a branch: (must not change the order or the numerotation of the branch -> maintain a list of removed branches)

/* Generating */
BmBench* BmTree_asNewBench( BmTree* self );
void BmTree_fromBench( BmTree* self, BmBench* model );

/* Printing */
char* BmTree_branch_print( BmTree* self, uint iBranch, char* output );

char* BmTree_print( BmTree* self, char* output);
char* BmTree_print_sep( BmTree* self, char* output, char* separator );
char* BmTree_print( BmTree* self, char* output);

char* BmTree_printInside( BmTree* self, char* output); // print `self` at the end of `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   F U N C T I O N  :  V A L U E F C T                         *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * code -> value
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  BmTree* selector;
  BmVector* outputs;
} BmValueFct;

/* Constructor */
BmValueFct* newBmValueFctBasic( uint inputSize, uint outputSize );
BmValueFct* newBmValueFctWith( BmCode* newInputRanges, BmVector* newOutputs );

BmValueFct* BmValueFct_createWith( BmValueFct* self, BmCode* newInputRanges, BmVector* newOutputs );

/* Destructor */
BmValueFct* BmValueFct_destroy( BmValueFct* self );
void deleteBmValueFct( BmValueFct* instance );

/* re-initializer */
uint BmValueFct_reinitWith( BmValueFct* self, BmCode* newInputRanges, BmVector* newOutputs );

/* Accessor */
BmTree* BmValueFct_selector( BmValueFct* self );
uint BmValueFct_inputDimention( BmValueFct* self );
uint BmValueFct_outputSize( BmValueFct* self );
BmCode* BmValueFct_inputRanges( BmValueFct* self );
BmVector* BmValueFct_outputs( BmValueFct* self );

double BmValueFct_from( BmValueFct* self, BmCode* input );

/* Construction */
uint BmValueFct_addValue( BmValueFct* self, double ouputValue );
uint BmValueFct_ouputId_setValue( BmValueFct* self, uint ouputId, double ouputValue );
uint BmValueFct_from_set( BmValueFct* self, BmCode* input, uint ouputId );

/* Instance tools */
void BmValueFct_switch(BmValueFct* self, BmValueFct* doppelganger);

/* Generating */
BmBench* BmValueFct_asNewBench( BmValueFct* self );

/* Printing */
char* BmValueFct_print(BmValueFct* self, char* buffer);
char* BmValueFct_printSep(BmValueFct* self, char* buffer, char* separator);



/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   F U N C T I O N  :  F U N C T I O N                         *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * code -> code + vector
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  BmTree* selector;
  BmBench* outputs;
} BmFunction;

/* Constructor */
BmFunction* newBmFunctionBasic( uint inputSize );
BmFunction* newBmFunctionWith( BmCode* newInputRanges, BmBench* newOutputs );

BmFunction* BmFunction_createWith( BmFunction* self, BmCode* newInputRanges, BmBench* newOutputs );

/* Destructor */
BmFunction* BmFunction_destroy( BmFunction* self );
void deleteBmFunction( BmFunction* instance );

/* re-initializer */
BmFunction* BmFunction_reinitWith( BmFunction* self, BmCode* newInputRanges, BmBench* newOutputs );
BmFunction* BmFunction_reinitWithDefault( BmFunction* self, BmCode* newInputRanges, BmCode* newDefaultOutput, double defaultValue );

/* Accessor */
BmTree* BmFunction_selector( BmFunction* self );
uint BmFunction_inputDimention( BmFunction* self );
BmCode* BmFunction_inputRanges( BmFunction* self );
uint BmFunction_outputSize( BmFunction* self );
BmBench* BmFunction_outputs( BmFunction* self );

uint BmFunction_from( BmFunction* self, BmCode* input );
BmCode* BmFunction_codeFrom( BmFunction* self, BmCode* input );
double BmFunction_valueFrom( BmFunction* self, BmCode* input );

/* Construction */
uint BmFunction_attachOuput( BmFunction* self, BmCode* newOuputCode, double ouputValue );
uint BmFunction_from_set( BmFunction* self, BmCode* input, uint ouputId );
// uint BmFunction_from_attach( BmFunction* self, BmCode* input, BmCode* newOutput, double value );

/* Instance tools */
void BmFunction_switch(BmFunction* self, BmFunction* doppelganger);

/* Printing */
char* BmFunction_print(BmFunction* self, char* output);
char* BmFunction_printSep(BmFunction* self, char* output, char* separator);

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   F U N C T I O N  :  D I S T B U T O R                       *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * code -> bench
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   F U N C T I O N  :  C O N D I T I O N                       *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * code -> bench
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  BmTree* selector;
  uint range;
  uint distribSize, distribCapacity;
  BmBench **distributions;
} BmCondition;

/* Constructor */
BmCondition* newBmConditionBasic(uint domain);
BmCondition* newBmConditionWith(uint domain, BmCode* newInputRanges, BmBench* newDefaultDistrib);

BmCondition* BmCondition_createBasic(BmCondition* self, uint domain);
BmCondition* BmCondition_createWith(BmCondition* self, uint domain, BmCode* newInputRanges, BmBench* newDefaultDistrib);

/* Destructor */
BmCondition* BmCondition_destroy(BmCondition* self);
void deleteBmCondition(BmCondition* instance);

/* re-initializer */
uint BmCondition_reinitWith( BmCondition* self, uint domain, BmCode* newInputRanges, BmBench* newDistrib );
uint BmCondition_reinitDistributionsWith( BmCondition* self, BmBench* newDistrib );

/* Accessor */
uint BmCondition_range( BmCondition* self );
BmTree* BmCondition_selector( BmCondition* self );
BmCode* BmCondition_parents( BmCondition* self );
BmBench* BmCondition_from( BmCondition* self, BmCode* configuration );
BmBench* BmCondition_fromKey( BmCondition* self, uint configKey );
uint BmCondition_distributionSize( BmCondition* self );
BmBench* BmCondition_distributionAt( BmCondition* self, uint iDistrib );

/* Construction */
uint BmCondition_attach( BmCondition* self, BmBench* distribution );
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
BmCondition* BmInferer_reinitIndependantNode( BmInferer* self, uint index );
BmCondition* BmInferer_node_reinitWith( BmInferer* self, uint index, BmCode* newParents );
BmCondition* BmInferer_node_reinitWith_withDefault( BmInferer* self, uint index, BmCode* newDependenceList, BmBench* newDefaultDistrib );

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
  uint size;
  BmValueFct ** ccriteria;
  BmCode ** masks;
  BmVector* weights;
} BmEvaluator;

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
BmValueFct* BmEvaluator_criterion( BmEvaluator* self, uint iCritirion );
BmVector* BmEvaluator_weights( BmEvaluator* self );
double BmEvaluator_criterion_weight( BmEvaluator* self, uint iCritirion );
BmCode* BmEvaluator_criterion_mask( BmEvaluator* self, uint iCritirion );

/* Process */
double BmEvaluator_process( BmEvaluator* self, BmCode* input );
double BmEvaluator_criterion_process( BmEvaluator* self, uint iCriterion, BmCode* input );

double BmEvaluator_processState_action(BmEvaluator* self, BmCode* state, BmCode* action);
double BmEvaluator_processState_action_state(BmEvaluator* self, BmCode* state, BmCode* action, BmCode* statePrime);


/* Construction */
BmEvaluator* BmEvaluator_reinitCriterion( BmEvaluator* self, uint numberOfCriterion );
BmValueFct* BmEvaluator_criterion_reinitWith( BmEvaluator* self, uint iCrit, BmCode* newDependenceMask, BmVector* newValues  );
void BmEvaluator_criterion_from_set( BmEvaluator* self, uint index, BmCode* option, uint output );
void BmEvaluator_criterion_setWeight( BmEvaluator* self, uint iCritirion, double weight );

/* Infering */


/* Printing */


#endif // BBMM_H

