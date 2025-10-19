/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   libBbMm - a library dedicated to Bayesian-based Markov-models.
 * 
 *   VERSION: 0.2.1
 *   
 * 
 *   STRUCTURE MODULE:
 *       - BmCode         : a fixed size array of digits (unsigned integers)
 *       - BmVector       : a fixed size array of values (doubles)
 *       - BmBench        : a dynamic-size collection of BmCode coupled to BmVector (i -> code and vector OR i -> digit and value)
 *       - BmTree         : a tree based BmCode (input code -> output digit )
 * 
 *   FUNCTION MODULE:
 *       - BmValueFct     : Determine a value from a code (in a given inputRanges)
 *       - BmFunction     : Determine a code+vector from a code
 *       <- BmDistbutor    : Determine a [code+vector distribution] from a code (used as Bayesian nodes) >
 * 
 *   COMPONENT MODULE:
 *       - BmCondition    : Represent conditional probabilities of a variable [input code -> digit distribution] (i.e. Bayesian Network's Node)
 *       - BmInferer      : Define a Bayesian Network as P(output | input) - potentially Dynamic P(state' | state) or P(state' | state, action)
 *       - BmEvaluator    : A value function over multiple criteria
 *
 *   MODEL MODUL:
 *       <- BmModel        : Define a Factored Markov Decision Process $<S, A, t, r>$ >
 * 
 *   SOLVER MODULE:
 *       <- BmPolicy       :                                                          >
 *       <- BmQValue       :                                                          >
 * 
 * 
 *   LICENSE: MIT License
 *
 *   Copyright © 2022-2025 Guillaume Lozenguez.
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
BmCode* newBmCode(digit dimension);
BmCode* newBmCode_numbers( digit dimension, digit* numbers );
BmCode* newBmCode_all(digit dimension, digit defaultValue);
BmCode* newBmCodeAs( BmCode* model );

BmCode* BmCode_create( BmCode* self, digit dimension );
BmCode* BmCode_create_numbers( BmCode* self, digit dimension, digit* numbers );
BmCode* BmCode_create_all( BmCode* self, digit dimension, digit defaultValue );
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
BmCode* BmCode_reinit( BmCode* self, digit newDimension );

BmCode* BmCode_copy( BmCode* self, BmCode* model);
BmCode* BmCode_copyNumbers( BmCode* self, BmCode* model);

/* Construction */
BmCode* BmCode_redimention( BmCode* self, digit newDimension);
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
BmVector* newBmVector( digit dimension );
BmVector* newBmVector_values( digit dimension, double* values );
BmVector* newBmVector_all( digit dimension, double value );
BmVector* newBmVectorAs( BmVector* model );

BmVector* BmVector_create( BmVector* self, digit dimension );
BmVector* BmVector_create_values( BmVector* self, digit dimension, double* values );
BmVector* BmVector_create_all( BmVector* self, digit dimension, double value );
BmVector* BmVector_createAs( BmVector* self, BmVector* model );

/* Destructor */
BmVector* BmVector_destroy( BmVector* self );
void deleteBmVector( BmVector* self );

/* Re-Initialize */
BmVector* BmVector_reinit( BmVector* self, digit newDimension );
BmVector* BmVector_copy( BmVector* self, BmVector* model );

/* Accessor */
digit BmVector_dimention( BmVector* self );
double BmVector_value( BmVector* self, digit i );

/* Construction */
BmVector* BmVector_redimention(BmVector* self, digit newDimension);
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
  digit capacity, start, size;
  digit codeDim, vectDim;
  BmCode   ** codes;
  BmVector ** vects;
} BmBench;

/* Constructor */
BmBench* newBmBench( digit capacity );
BmBench* newBmBench_codeDim_vectorDim( digit capacity, digit codeDim, digit vectorDim );
BmBench* newBmBench_startDigit_value( digit capacity, digit aDigit, double value );
BmBench* newBmBench_startWithCode_vector( digit capacity, BmCode* newCode, BmVector* newVector );
BmBench* newBmBenchAs( BmBench* model );

BmBench* BmBench_create( BmBench* self, digit capacity );
BmBench* BmBench_create_codeDim_vectorDim( BmBench* self, digit capacity, digit codeDim, digit vectorDim );
BmBench* BmBench_createAs( BmBench* self, BmBench* model );

/* Destructor */
BmBench* BmBench_destroy( BmBench* self);
void deleteBmBench( BmBench* self);

/* Re-Initializer */
BmBench* BmBench_reinit( BmBench* self, digit capacity );

/* Accessor */
digit BmBench_size( BmBench* self);

digit BmBench_codeDimention( BmBench* self);
digit BmBench_vectorDimention( BmBench* self);

BmCode* BmBench_codeAt( BmBench* self, digit i );
BmVector* BmBench_vectorAt( BmBench* self, digit i );

digit BmBench_digitAt( BmBench* self, digit i );
double BmBench_valueAt( BmBench* self, digit i );

digit BmBench_at_digit( BmBench* self, digit i, digit j );
double BmBench_at_value( BmBench* self, digit i, digit j );

/* Construction */
//void BmBench_resizeCapacity_start( BmBench* self, digit newCapacity, digit start );
void BmBench_resizeCapacity( BmBench* self, digit newCapacity);
//void BmBench_resizeCapacityFront( BmBench* self, digit newCapacity);
digit BmBench_attachCode_vector( BmBench* self, BmCode* newCode, BmVector* newVector );
digit BmBench_attachFrontCode_vector( BmBench* self, BmCode* newCode, BmVector* newVector );

BmCode* BmBench_detach( BmBench* self );
BmCode* BmBench_detachFront( BmBench* self );
//BmCode* BmBench_detach( BmBench* self, digit i );

BmBench* BmBench_increase( BmBench* self, digit number );
BmBench* BmBench_increaseFront( BmBench* self, digit number );

digit BmBench_attachCode( BmBench* self, BmCode* newItem );
digit BmBench_attachVector( BmBench* self, BmVector* newItem );

void BmBench_switch( BmBench* self, BmBench* doppleganger);

/* Construction Basic */
digit BmBench_addDigit_value( BmBench* self, digit d, double v );
BmBench* BmBench_at_setDigit( BmBench* self, digit i, digit aDigit );
BmBench* BmBench_at_setValue( BmBench* self, digit i, double value );

//void BmBench_add( BmBench *self, BmBench *another );
void BmBench_add_reducted( BmBench* self, BmBench* another, BmCode* mask );

/* Operators */
typedef bool (*fctptr_BmBench_compare)(BmBench*,digit,digit);
digit BmBench_sort( BmBench* self, fctptr_BmBench_compare compare );
digit BmBench_switchAt_at( BmBench* self, digit id1, digit id2 );

/* Comparison */

bool BmBench_is_codeGreater(BmBench* self, digit i1, digit i2);
bool BmBench_is_codeSmaller(BmBench* self, digit i1, digit i2);
bool BmBench_is_vectorGreater(BmBench* self, digit i1, digit i2);
bool BmBench_is_vectorSmaller(BmBench* self, digit i1, digit i2);

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
  digit capacity, size;
  digit** branches;    // -> digit* branchvariable AND Reductor** // Funnel** branchSelector AND digit** branches ?
} BmTree;

/* Constructor */
BmTree* newBmTree( digit binarySpaceSize );
BmTree* newBmTreeWith( BmCode* newSpace );

BmTree* BmTree_createWhith( BmTree* self, BmCode* input );

/* Destructor */
BmTree* BmTree_destroy( BmTree* self);
void deleteBmTree( BmTree* self );

/* Re-Initializer */
BmTree* BmTree_reinitWith( BmTree* self, BmCode* newSpace );

BmTree* BmTree_clearWhith_on( BmTree* self, digit index, digit defaultOption );
BmTree* BmTree_clearOn( BmTree* self, digit defaultOption );

/* Accessor */
digit BmTree_dimention( BmTree* self );
digit BmTree_size( BmTree* self );
BmCode* BmTree_inputRanges( BmTree* self );
digit BmTree_at( BmTree* self, BmCode* code ); // Return the option number of a code/state.

/* output */
digit BmTreeChild( digit key );
digit BmTreeLeaf( digit key );

/* Construction */
void BmTree_reziseCapacity( BmTree* self, digit newCapacity );
void BmTree_reziseCompleteCapacity( BmTree* self );

digit BmTree_at_set( BmTree* self, BmCode* code, digit output ); // set the ouput value of a code or a partial code (with 0), return the number of potential dead branches
digit BmTree_at_readOrder_set( BmTree* self, BmCode* code, BmCode* codeOrder, digit output );

/* Branch Accessor */
digit BmTree_branchSize( BmTree* self, digit iBranch );
digit BmTree_branch_stateIndex( BmTree* self, digit iBranch, digit state );
digit BmTree_branch_state( BmTree* self, digit iBranch, digit state );
digit BmTree_branch_stateIsLeaf( BmTree* self, digit iBranch, digit state );
digit BmTree_branch_stateOption( BmTree* self, digit iBranch, digit state );
digit BmTree_branch_stateLeaf( BmTree* self, digit iBranch, digit state );
digit BmTree_branchVariable( BmTree* self, digit iBranch ); // Return the variable index represented by the branch.
digit BmTree_branchStart( BmTree* self, digit iBranch );
digit BmTree_branchBound( BmTree* self, digit iBranch );
digit BmTree_branchStep( BmTree* self, digit iBranch );
digit BmTree_branchNumberOfOutputs( BmTree* self, digit branch ); // Return the number of differents output
digit BmTree_deepOf( BmTree* self, BmCode* code); // Return the number of branch to cross before reaching the output.

/* Branch Construction */
digit BmTree_newBranch( BmTree* self, digit iVariable, digit start, digit bound, digit step );
digit BmTree_newBranch_full( BmTree* self, digit iVariable, digit defaultOption);
digit BmTree_newBranch_binary_options( BmTree* self, digit iVariable, digit  afterValue, digit option1, digit option2);
digit BmTree_newBranch_pivot_options( BmTree* self, digit iVariable, digit onValue, digit optionBefore, digit optionOn, digit optionAfter);
void BmTree_branch_state_connect( BmTree* self, digit branchA, digit stateA, digit branchB );
void BmTree_branch_state_setOption( BmTree* self, digit branchA, digit iState, digit outbut );

/* Cleanning */
digit BmTree_cleanDeadBranches( BmTree* self); // Detect and remove detached branches (or BmTree_update, BmTree_regenerate : rebuild the tree without dead branches)
digit BmTree_removeBranch( BmTree* self, digit iBranch); // Remove a branch: (must not change the order or the numerotation of the branch -> maintain a list of removed branches)

/* Generating */
BmBench* BmTree_asNewBench( BmTree* self );
void BmTree_fromBench( BmTree* self, BmBench* model );

/* Printing */
char* BmTree_branch_print( BmTree* self, digit iBranch, char* output );

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
BmValueFct* newBmValueFctBasic( digit inputSize, digit outputSize );
BmValueFct* newBmValueFctWith( BmCode* newInputRanges, BmVector* newOutputs );

BmValueFct* BmValueFct_createWith( BmValueFct* self, BmCode* newInputRanges, BmVector* newOutputs );

/* Destructor */
BmValueFct* BmValueFct_destroy( BmValueFct* self );
void deleteBmValueFct( BmValueFct* instance );

/* re-initializer */
digit BmValueFct_reinitWith( BmValueFct* self, BmCode* newInputRanges, BmVector* newOutputs );

/* Accessor */
BmTree* BmValueFct_selector( BmValueFct* self );
digit BmValueFct_inputDimention( BmValueFct* self );
digit BmValueFct_outputSize( BmValueFct* self );
BmCode* BmValueFct_inputRanges( BmValueFct* self );
BmVector* BmValueFct_outputs( BmValueFct* self );

double BmValueFct_from( BmValueFct* self, BmCode* input );

/* Construction */
digit BmValueFct_addValue( BmValueFct* self, double ouputValue );
digit BmValueFct_ouputId_setValue( BmValueFct* self, digit ouputId, double ouputValue );
digit BmValueFct_from_set( BmValueFct* self, BmCode* input, digit ouputId );

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
BmFunction* newBmFunctionBasic( digit inputSize );
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
digit BmFunction_inputDimention( BmFunction* self );
BmCode* BmFunction_inputRanges( BmFunction* self );
digit BmFunction_outputSize( BmFunction* self );
BmBench* BmFunction_outputs( BmFunction* self );

digit BmFunction_from( BmFunction* self, BmCode* input );
BmCode* BmFunction_codeFrom( BmFunction* self, BmCode* input );
double BmFunction_valueFrom( BmFunction* self, BmCode* input );

/* Construction */
digit BmFunction_attachOuput( BmFunction* self, BmCode* newOuputCode, double ouputValue );
digit BmFunction_from_set( BmFunction* self, BmCode* input, digit ouputId );
// digit BmFunction_from_attach( BmFunction* self, BmCode* input, BmCode* newOutput, double value );

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
  digit range;
  digit distribSize, distribCapacity;
  BmBench **distributions;
} BmCondition;

/* Constructor */
BmCondition* newBmConditionBasic(digit domain);
BmCondition* newBmConditionWith(digit domain, BmCode* newInputRanges, BmBench* newDefaultDistrib);

BmCondition* BmCondition_createBasic(BmCondition* self, digit domain);
BmCondition* BmCondition_createWith(BmCondition* self, digit domain, BmCode* newInputRanges, BmBench* newDefaultDistrib);

/* Destructor */
BmCondition* BmCondition_destroy(BmCondition* self);
void deleteBmCondition(BmCondition* instance);

/* re-initializer */
digit BmCondition_reinitWith( BmCondition* self, digit domain, BmCode* newInputRanges, BmBench* newDistrib );
digit BmCondition_reinitDistributionsWith( BmCondition* self, BmBench* newDistrib );

/* Accessor */
digit BmCondition_range( BmCondition* self );
BmTree* BmCondition_selector( BmCondition* self );
BmCode* BmCondition_parents( BmCondition* self );
BmBench* BmCondition_from( BmCondition* self, BmCode* configuration );
BmBench* BmCondition_fromKey( BmCondition* self, digit configKey );
digit BmCondition_distributionSize( BmCondition* self );
BmBench* BmCondition_distributionAt( BmCondition* self, digit iDistrib );

/* Construction */
digit BmCondition_attach( BmCondition* self, BmBench* distribution );
digit BmCondition_from_attach( BmCondition* self, BmCode* configuration, BmBench* distribution );

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
  digit inputDimention, outputDimention, overallDimention;
  BmBench* network;
  BmCondition* nodes;
  BmBench* distribution;
} BmInferer;

/* Constructor*/
BmInferer* newBmInferer( BmCode* variableSpace, digit inputDimention, digit outputDimention );
BmInferer* newBmInfererStateAction( BmCode* stateSpace, BmCode* actionSpace );
BmInferer* newBmInfererStateActionShift( BmCode* stateSpace, BmCode* actionSpace, BmCode* shiftSpace );

BmInferer* BmInferer_create( BmInferer* self, BmCode* varDomains, digit inputDimention, digit outputDimention );

/* Destructor */
BmInferer* BmInferer_destroy(BmInferer* self);
void deleteBmInferer(BmInferer* self);

/* Accessor */
BmBench* BmInferer_distribution( BmInferer* self );

digit BmInferer_inputDimention( BmInferer* self );
digit BmInferer_outputDimention( BmInferer* self );
digit BmInferer_shiftDimention( BmInferer* self );
digit BmInferer_overallDimention( BmInferer* self );

BmCondition* BmInferer_node( BmInferer* self, digit iNode );
digit BmInferer_node_size( BmInferer* self, digit iVar );
BmCode* BmInferer_node_parents( BmInferer* self, digit iVar );

/* Construction */
BmCondition* BmInferer_reinitIndependantNode( BmInferer* self, digit index );
BmCondition* BmInferer_node_reinitWith( BmInferer* self, digit index, BmCode* newParents );
BmCondition* BmInferer_node_reinitWith_withDefault( BmInferer* self, digit index, BmCode* newDependenceList, BmBench* newDefaultDistrib );

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
  digit size;
  BmValueFct ** ccriteria;
  BmCode ** masks;
  BmVector* weights;
} BmEvaluator;

/* Constructor*/
BmEvaluator* newBmEvaluatorBasic( digit spaceDimention, digit numberOfCriteria );
BmEvaluator* newBmEvaluatorWith( BmCode* newSpace, digit numberOfCriteria );

BmEvaluator* BmEvaluator_createWith( BmEvaluator* self, BmCode* newSpace, digit numberOfCriteria );

/* Destructor */
void deleteBmEvaluator( BmEvaluator* self );
BmEvaluator* BmEvaluator_destroy( BmEvaluator* self);

/* Accessor */
BmCode* BmEvaluator_space( BmEvaluator* self );
digit BmEvaluator_numberOfCriteria( BmEvaluator* self );
BmValueFct* BmEvaluator_criterion( BmEvaluator* self, digit iCritirion );
BmVector* BmEvaluator_weights( BmEvaluator* self );
double BmEvaluator_criterion_weight( BmEvaluator* self, digit iCritirion );
BmCode* BmEvaluator_criterion_mask( BmEvaluator* self, digit iCritirion );

/* Process */
double BmEvaluator_process( BmEvaluator* self, BmCode* input );
double BmEvaluator_criterion_process( BmEvaluator* self, digit iCriterion, BmCode* input );

double BmEvaluator_processState_action(BmEvaluator* self, BmCode* state, BmCode* action);
double BmEvaluator_processState_action_state(BmEvaluator* self, BmCode* state, BmCode* action, BmCode* statePrime);

/* Construction */
BmEvaluator* BmEvaluator_reinitCriterion( BmEvaluator* self, digit numberOfCriterion );
BmValueFct* BmEvaluator_criterion_reinitWith( BmEvaluator* self, digit iCrit, BmCode* newDependenceMask, BmVector* newValues  );
void BmEvaluator_criterion_from_set( BmEvaluator* self, digit index, BmCode* option, digit output );
void BmEvaluator_criterion_setWeight( BmEvaluator* self, digit iCritirion, double weight );

/* Infering */

/* Printing */


#endif // BBMM_H

