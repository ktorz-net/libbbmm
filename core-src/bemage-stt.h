
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   BeMAge Structures - A library in KISS philosophy of
 *                      data structures useful for BeMAge project
 *
 *   FEATURES:
 *       - BmCode        : a fixed size list of unsigned integers
 *       - BmBench       : a dynamic-size collection of BmCode
 *       - BmNet         : a network based on unsigned integers
 *       - BmTree        : a decision tree based BmCode (BmCode -> uint)
 *       - BmData        : A decision tree based BmCode (BmCode -> double)
 *       - BmFunction    : A decision tree based BmCode (BmCode -> double)
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
#define delEmptyArray(instance) free(instance)
#define array_on(instance, index) instance+(index-1)
#define array_at(instance, index) instance[index-1]
#define array_at_set(instance, index, value) instance[index-1]= value

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B e M A g e   S T R U C T U R E :  B A S I C   T O O L S              *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* buffers */
char* bm_wordBuffer(); //[WORD_BUFFER_SIZE];
char* bm_lineBuffer(); //[WORD_BUFFER_SIZE];
char* bm_textBuffer(); //[WORD_BUFFER_SIZE];

/* double  manipulation */
char* bm_printDouble(double p, char* output);

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B e M A g e   S T R U C T U R E :  C O D E                              *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
    uint *dsc;
} BmCode;

/* Constructor */
//BmCode* newBasicBmCode();
//BmCode* newBmCode(uint size);
BmCode* newBmCodeBasic(uint size, uint defaultValue);
BmCode* newBmCodeArray(uint size, uint* numbers);
BmCode* newBmCodeAs(BmCode* model);
BmCode* newBmCode(uint size, ...);

BmCode* BmCode_createBasic( BmCode* self, uint size, uint defaultValue );
BmCode* BmCode_createArray( BmCode* self, uint size, uint* numbers );
BmCode* BmCode_createAs( BmCode* self, BmCode* model );

/* Destructor */
void deleteBmCode(BmCode* instance);
BmCode* BmCode_distroy(BmCode* self);

/* Arrayed */
BmCode* newArrayOfBmCode(uint arraySize, uint codeSize);
void deleteArrayOfBmCode(BmCode* array, uint arraySize);

/* Initializer */
BmCode* BmCode_initializeArray(BmCode* self, uint newSize, uint* numbers );
BmCode* WdCore_initializeBasic(BmCode* self, uint newSize);
BmCode* WdCore_initializeHomogene(BmCode* self, uint newSize, uint defaultRange);

BmCode* BmCode_initialize(BmCode* self, uint newSize, ...);

BmCode* BmCode_copy(BmCode* self, BmCode* model);
BmCode* BmCode_copyNumbers(BmCode* self, BmCode* model);

/* Accessor */
uint BmCode_size(BmCode* self);
uint* BmCode_numbers(BmCode* self);
uint BmCode_at(BmCode* self, uint index);
uint BmCode_count(BmCode* self, uint value);
ulong BmCode_sum(BmCode* self);
ulong BmCode_product(BmCode* self);

/* Construction */
BmCode* BmCode_resize(BmCode* self, uint newSize);
BmCode* BmCode_setAll(BmCode* self, uint value );
BmCode* BmCode_at_set(BmCode* self, uint index, uint value );
BmCode* BmCode_at_increment(BmCode* self, uint index, uint value );
BmCode* BmCode_at_decrement(BmCode* self, uint index, uint value );

BmCode* BmCode_setNumbers(BmCode* self, uint* numbers );

/* sort */
void BmCode_sort( BmCode* self );
void BmCode_switch( BmCode* self, BmCode* anotherCode );
uint BmCode_search( BmCode* self, uint value );

/* Test */
bool BmCode_isEqualTo(BmCode* self, BmCode* another);
bool BmCode_isGreaterThan(BmCode* self, BmCode* another);
bool BmCode_isLighterThan(BmCode* self, BmCode* another);

/* configuration managment (a BmCode configuration varring in a space defined by a ranges BmCode 'self' ) */
ulong BmCode_keyOf(BmCode* self, BmCode* code);       // get the key value of the code regarding given ranges ( i.e. 0 <= self.numbers[i] < ranges[i] )

BmCode* BmCode_setCode_onKey(BmCode* self, BmCode* configuration, ulong key ); // set the code as a key value in given ranges
BmCode* BmCode_setCodeFirst(BmCode* self, BmCode* configuration); // set the code as a key value in given ranges
BmCode* BmCode_setCodeLast(BmCode* self, BmCode* configuration); // set the code as a key value in given ranges

BmCode* BmCode_newBmCodeOnKey( BmCode* self, ulong key ); // set the code as a key value in given ranges
BmCode* BmCode_newBmCodeFirst( BmCode* self); // set the code as a key value in given ranges
BmCode* BmCode_newBmCodeLast( BmCode* self ); // set the code as a key value in given ranges

BmCode* BmCode_nextCode(BmCode* self, BmCode* configuration); // set the code as a key value in given ranges
BmCode* BmCode_previousCode(BmCode* self, BmCode* configuration); // set the code as a key value in given ranges
bool BmCode_isIncluding(BmCode* self, BmCode* configuration); // set the code as a key value in given ranges

/* Printing */
char* BmCode_wording(BmCode* self); // print `self` on `output`
char* BmCode_print(BmCode* self, char* buffer);   // print `self` at the end of `output`

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B e M A g e   S T R U C T U R E :  N E T W O R K                        *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a Simple Network 
*/

typedef struct {
  uint size;
  BmCode* edges;
} WdNet;

/* Constructor Destructor */
WdNet* newWdNet(uint size);
void deleteWdNet(WdNet* self);

WdNet* WdNet_create(WdNet* self, uint size); // generate
WdNet* WdNet_distroy(WdNet* self);

/* Accessor */
BmCode* WdNet_at(WdNet* self, uint i);

/* Test */

/* Construction */
void WdNet_clearAt(WdNet* self, uint node);
void WdNet_at_connect(WdNet* self, uint from, BmCode* targets);

/* Printing */
char* WdNet_wording(WdNet* self); // print `self` on `output`
char* WdNet_print(WdNet* self, char* output); // print `self` at the end of `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B e M A g e   S T R U C T U R E :  C O L L E C T I O N                    *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint capacity, size;
  BmCode ** items;
  uint attributsSize, valuesSize;
  uint ** attributs;
  double ** values;
//  char ** wordings;
} WdCollection;

/* Constructor Destructor */
WdCollection* newWdCollection(uint capacity);
WdCollection* newWdCollectionCharacterized(uint capacity, uint attributsSize, uint valuesSize);
void deleteWdCollection(WdCollection* self);
void deleteDeepWdCollection(WdCollection* self);

WdCollection* WdCollection_create(WdCollection* self, uint capacity);
WdCollection* WdCollection_createCharacterized(WdCollection* self, uint capacity, uint attributsSize, uint valuesSize);
WdCollection* WdCollection_distroy(WdCollection* self);
WdCollection* WdCollection_distroyDeep(WdCollection* self);

/* Initializer */

/* Modification */
void WdCollection_resizeCapacity( WdCollection* self, uint newCapacity );

/* Accessor */
uint WdCollection_size(WdCollection* self);
BmCode* WdCollection_at( WdCollection* self, uint i );

uint WdCollection_attributsSize( WdCollection* self );
uint WdCollection_valuesSize( WdCollection* self );

uint* WdCollection_at_attributs( WdCollection* self, uint iItem );
double* WdCollection_at_values( WdCollection* self, uint iItem );

uint WdCollection_at_attribut( WdCollection* self, uint iItem, uint iAtt );
double WdCollection_at_value( WdCollection* self, uint iItem, uint iVal );

/* Test */

/* Construction Item */
uint WdCollection_attachNewItem( WdCollection* self, BmCode* item );

uint WdCollection_at_attribut_set( WdCollection* self, uint iItem, uint iAtt, uint value );
uint WdCollection_at_value_set( WdCollection* self, uint iItem, uint iVal, double value );

void WdCollection_sortOnItem(WdCollection* self);

/* Printing */
char* WdCollection_print(WdCollection* self, char* output); // print `self` at the end of `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B e M A g e   T R E E                                                   *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Apply a tree structure to a Space for clustering states in a finit number of options.
*/

typedef struct {
  BmCode* input;
  uint optionBound;
  uint capacity, size;
  uint** branches;
} WdTree;

/* Constructor Destructor */
WdTree* newWdTree( BmCode* input, uint optionSize );
WdTree* newWdTreeInputSizeAndBound( uint inputSize, uint* inputBounds, uint optionSize );
void deleteWdTree( WdTree* self );

WdTree* WdTree_create( WdTree* self, uint inputSize, uint* inputBounds, uint optionSize );
WdTree* WdTree_distroy(WdTree* self);

/* initialize */
void WdTree_initializeWhith_on(WdTree* self, uint index, int defaultOption);
void WdTree_initializeOn(WdTree* self, int defaultOption);

/* Construction */
void WdTree_reziseCapacity( WdTree* self, uint newCapacity);
void WdTree_reziseCompleteCapacity( WdTree* self);
uint WdTree_newBranch(WdTree* self, uint iVariable, int defaultOption);
void WdTree_branch_option_connect( WdTree* self, uint branchA, uint i, uint branchB );
void WdTree_branch_option_output( WdTree* self, uint branchA, uint i, uint outbut );
uint WdTree_at_set( WdTree* self, BmCode* code, uint output ); // set the ouput value of a code or a partial code (with 0), return the number of potential dead branches
uint WdTree_at_set_fromBranch( WdTree* self, BmCode* code, uint output, uint iBranch ); // cf. WdTree_at_set, but starting from a given branch.

/* Cleanning */
uint WdTree_cleanDeadBranches( WdTree* self); // Detect and remove detached branches (or WdTree_update, WdTree_regenerate : rebuild the tree without dead branches)
uint WdTree_removeBranch(WdTree* self, uint iBranch); // Remove a branch: (must not change the order or the numerotation of the branch -> maintain a list of removed branches)

/* Accessor */
uint WdTree_at( WdTree* self, BmCode* code); // Return the ouput value of a code/state.
uint WdTree_branch_option( WdTree* self, uint iBranch, uint option );
uint WdTree_branchVariable( WdTree* self, uint iBranch ); // Return the variable index represented by the branch.
uint WdTree_branchSize( WdTree* self, uint branch ); // Return the number of possible output.
uint WdTree_deepOf( WdTree* self, BmCode* code); // Return the number of branch to cross before reaching the output.

/* Generating */
WdCollection* WdTree_asNewCollection( WdTree* self );

/* Printing */
char* WdTree_printBranch( WdTree* self, uint iBranch, char* output );
char* WdTree_wording(WdTree* self); // print `self` on `output`
char* WdTree_print(WdTree* self, char* output); // print `self` at the end of `output`

char* WdTree_printConditions(WdTree* self, char* output);

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B e M A g e   S T R U C T U R E :  V A L U E  F U N C T I O N           *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Set an output value to each configuration in given ranges (BmCode -> value).
*/

#endif
