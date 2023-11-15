/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   BbMm Structures - A library in KISS philosophy for *Bayesian-based Markov-models* data structures.
 * 
 *   FEATURES:
 *       - BmCode         : a fixed size list of digit (unsigned integers)
 *       - BmBench        : a dynamic-size collection of BmCode with tag and value (i -> code + tag and value )
 *       - BmTree         : a tree based BmCode (BmCode -> tag and value )
 *       - BmVector       : a fixed size list of values (doubles)
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

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   S O M E   G L O B A L E   D E F I N I T I O N                         *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#ifndef BEMAGE_STT_H
#define BEMAGE_STT_H

#include <stdarg.h>

#define uint unsigned int
#define ulong unsigned long
#define INT_BUFFER_SIZE 16
#define WORD_BUFFER_SIZE 64
#define LINE_BUFFER_SIZE 1024  // ie. 16 words
#define TEXT_BUFFER_SIZE 16384 // ie. 16 lines

typedef uint bool;

#define true 1
#define false 0
#define SIGNIFICANT_DIGITS 2

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
 *   B b M m   S T R U C T U R E :  B A S I C   T O O L S              *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* buffers */
char* bm_wordBuffer(); //[WORD_BUFFER_SIZE];
char* bm_lineBuffer(); //[WORD_BUFFER_SIZE];
char* bm_textBuffer(); //[WORD_BUFFER_SIZE];

/* double  manipulation */
char* bm_printDouble(double p, char* output);

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  C O D E                              *
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
uint BmCode_size( BmCode* self);
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
BmCode* BmCode_resize( BmCode* self, uint newSize);
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
bool BmCode_isLighterThan( BmCode* self, BmCode* another );

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
char* BmCode_wording( BmCode* self); // print `self` on `output`
char* BmCode_print( BmCode* self, char* buffer);   // print `self` at the end of `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  B E N C H                              *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint capacity, start, size;
  BmCode ** items;
  uint * tags;
  double * values;
} BmBench;

/* Constructor */
BmBench* newBmBench(uint capacity);

BmBench* BmBench_create( BmBench* self, uint capacity);

/* Destructor */
BmBench* BmBench_destroy( BmBench* self);
void deleteBmBench( BmBench* self);

/* Re-Initializer */

/* Accessor */
uint BmBench_size( BmBench* self);
BmCode* BmBench_at( BmBench* self, uint i );
uint BmBench_tagAt( BmBench* self, uint i );
double BmBench_valueAt( BmBench* self, uint i );

/* Construction */
void BmBench_resizeCapacity( BmBench* self, uint newCapacity );

uint BmBench_attachLast( BmBench* self, BmCode* newItem );
BmCode* BmBench_detachLast( BmBench* self );

uint BmBench_attachFirst( BmBench* self, BmCode* newItem );
BmCode* BmBench_detachFirst( BmBench* self );

BmCode* BmBench_at_tag( BmBench* self, uint i, uint tagValue );
BmCode* BmBench_at_value( BmBench* self, uint i, double value );

/* Operator */
void BmBench_sortOnItem( BmBench* self);
void BmBench_sortOnTag( BmBench* self);

/* Test */

/* Printing */
char* BmBench_print( BmBench* self, char* output); // print `self` at the end of `output`
char* BmBench_printCodes(BmBench* self, char* output);


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  T R E E                                *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Apply a tree structure to a Space for clustering states in a finit number of options.
*/

typedef struct {
  BmCode* input;
  uint optionBound;
  uint capacity, size;
  uint** branches;
} BmTree;

/* Constructor */
BmTree* newBmTreeBasic( uint inputSize, uint optionSize );
BmTree* newBmTreeWith( BmCode* newInput, uint optionSize );
BmTree* newBmTree( BmCode* input, uint optionSize );

BmTree* BmTree_createWhith( BmTree* self, BmCode* input, uint optionSize );

/* Destructor */
BmTree* BmTree_destroy( BmTree* self);
void deleteBmTree( BmTree* self );

/* Re-Initializer */
void BmTree_reinitWhith_on( BmTree* self, uint index, int defaultOption);
void BmTree_reinitOn( BmTree* self, int defaultOption);

/* Accessor */
uint BmTree_at( BmTree* self, BmCode* code); // Return the ouput value of a code/state.
uint BmTree_branch_option( BmTree* self, uint iBranch, uint option );
uint BmTree_branchVariable( BmTree* self, uint iBranch ); // Return the variable index represented by the branch.
uint BmTree_branchSize( BmTree* self, uint branch ); // Return the number of possible output.
uint BmTree_deepOf( BmTree* self, BmCode* code); // Return the number of branch to cross before reaching the output.

/* Construction */
void BmTree_reziseCapacity( BmTree* self, uint newCapacity);
void BmTree_reziseCompleteCapacity( BmTree* self);
uint BmTree_newBranch( BmTree* self, uint iVariable, int defaultOption);
void BmTree_branch_option_connect( BmTree* self, uint branchA, uint i, uint branchB );
void BmTree_branch_option_output( BmTree* self, uint branchA, uint i, uint outbut );

uint BmTree_at_set( BmTree* self, BmCode* code, uint output ); // set the ouput value of a code or a partial code (with 0), return the number of potential dead branches
uint BmTree_at_readOrder_set( BmTree* self, BmCode* code, BmCode* codeOrder, uint output );

/* Cleanning */
uint BmTree_cleanDeadBranches( BmTree* self); // Detect and remove detached branches (or BmTree_update, BmTree_regenerate : rebuild the tree without dead branches)
uint BmTree_removeBranch( BmTree* self, uint iBranch); // Remove a branch: (must not change the order or the numerotation of the branch -> maintain a list of removed branches)

/* Generating */
BmBench* BmTree_asNewBench( BmTree* self );

/* Printing */
char* BmTree_printBranch( BmTree* self, uint iBranch, char* output );
char* BmTree_wording( BmTree* self); // print `self` on `output`

char* BmTree_print( BmTree* self, char* output);
char* BmTree_print_sep( BmTree* self, char* output, char* separator );
char* BmTree_print_sep_options( BmTree* self, char* output, char* separator, char** optionStrs );
char* BmTree_print( BmTree* self, char* output);

char* BmTree_printInside( BmTree* self, char* output); // print `self` at the end of `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  V E C T O R                            *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint size;
  double * values;
} BmVector;

/* Constructor */
BmVector* newBmVectorBasic( uint size );
BmVector* newBmVector_values( uint size, double* values );
BmVector* newBmVector_list( uint size, double val1, ... );
BmVector* newBmVector_all( uint size, double* value );

BmVector* BmVector_createBasic( BmVector* self, uint size );
BmVector* BmVector_create_values( BmVector* self, uint size, double* values );
BmVector* BmVector_create_all( BmVector* self, uint size, double value );

/* Destructor */
BmVector* BmVector_destroy( BmVector* self );
void deleteBmVector( BmVector* self );

/* Re-Initialize */
//BmVector* BmVector_resize(BmVector* self, uint size);

/* Accessor */
uint BmVector_size( BmVector* self );
double BmVector_at( BmVector* self, uint i );

/* Construction */
double BmVector_at_set( BmVector* self, uint i, double value );

/* Operation */
double BmVector_sum( BmVector* self );
double BmVector_product( BmVector* self );

/* Printing */
char* BmVector_print( BmVector* self, char* output );


#endif
