/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   BbMm Model - A library in KISS philosophy for *Bayesian-based Markov-models* creation and manipulation.
 *
 *   FEATURES:
 *       - BmDomain       : Definie e finit set of possible values (words, integer-interval, cluster-means, .... )
 *       - BmSpace        : a vector of variable in a collection of domains
 *       - BmModel        : <S, A, t, r>
 * 
 *   LICENSE: MIT License
 *
 *   Copyright © 2020-2021 Guillaume Lozenguez.
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

#ifndef WANDA_MODEL_H
#define WANDA_MODEL_H

#include "bbmm-structures.h"
#include "bbmm-probabilities.h"

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   W A N D A   D O M A I N                                               *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* 
  Represent a variation domain of finit discrete variables
*/

enum BmDomainType{
  BmDomainType_UNDEF= 0,
  BmDomainType_BASIC,
  BmDomainType_RANGE,
  BmDomainType_MEANS,
  BmDomainType_WORDS
}; //, BmDomainType_means};

typedef struct BmDomainStruct {
  char* name;
  uint type, size;
  void* dsc;
  char* (*str_fct)(struct BmDomainStruct *, uint);
  double (*value_fct)(struct BmDomainStruct *, uint);
} BmDomain;

/* Constructor Destructor */
BmDomain* newBmDomainBasic( char* name, uint size );   // Create a new instance of BmDomain of size `size`
BmDomain* newBmDomainRange( char* name, double from, double to, double by );   // Create a new instance of BmDomain of size `size`
BmDomain* newBmDomainWordsArray( char* name, uint size, char ** worldsArray );  // Create a new instance of BmDomain of size
BmDomain* newBmDomainWords( char* name, uint size, ... );
BmDomain* newBmDomainMeans( char* name, uint size, double* worlds );
BmDomain* newBmDomainAs( BmDomain* model );

//BmDomain* newBmDomainMeans(uint size, float * means); // Create a new instance of BmDomain of size
void deleteBmDomain(BmDomain* self); // Delete an instance of BmDomain

/* fill and empty a structure */
BmDomain* BmDomain_createBasic( BmDomain* self, char* name, uint size );
BmDomain* BmDomain_createRange( BmDomain* self, char* name, double from, double to, double by );
BmDomain* BmDomain_createWords( BmDomain* self, char* name, uint size, char ** worlds );
BmDomain* BmDomain_createMeans( BmDomain* self, char* name, uint size, double* means );
BmDomain* BmDomain_createAs( BmDomain* self, BmDomain* model );
BmDomain* BmDomain_destroy( BmDomain* self );

/* Accessor */
char* BmDomain_name(BmDomain* self);
uint BmDomain_size(BmDomain* self); // return the size of the wanda domain
double BmDomain_valueAt(BmDomain* self, uint index);
double BmDomainBasic_valueAt(struct BmDomainStruct* self, uint index);

double BmDomainRange_valueAt(struct BmDomainStruct* self, uint index);
double BmDomainMeans_valueAt(struct BmDomainStruct* elf, uint index);

/* Accessor - BmDomainRange */
double BmDomainRange_from(BmDomain* self); // return the size of the wanda domain
double BmDomainRange_to(BmDomain* self); // return the size of the wanda domain
double BmDomainRange_by(BmDomain* self); // return the size of the wanda domain

/* Accessor - BmDomainWords */

/* Engine */
uint BmDomain_searchStr( BmDomain* self, char* ref );
//uint BmDomain_searchValue( BmDomain* self, double ref );

/* Printing */
char* BmDomain_strAt(BmDomain* self, uint index); // return the str at a given `index`
char* BmDomain_strAt_fromValue(struct BmDomainStruct* self, uint index); // print `self` at the end of `output`
char* BmDomainWords_strAt(struct BmDomainStruct* self, uint index); // print `self` at the end of `output`

/* Test */
bool BmDomain_isType(BmDomain* self, enum BmDomainType type); // "return true if state is in the state space"

/* Search */
//uint BmDomain_classOf(BmDomain* self, char * item); // return the first index of an 'item' in the BmDomain (and size if not included)
//uint BmDomain_indexOf(BmDomain* self, char * item); // return the first index of an 'item' in the BmDomain (and size if not included)
//bool BmDomain_isIncluded(BmDomain* self, char * item); // Test if an item is in the domain

/* DomainStr */
//void BmDomain_at_copyItem(BmDomain* self, uint index, char * item); //copy a new `word` at a given `index`
//void BmDomain_copyAll(BmDomain* self, char ** domain ); //copy the overall string value

/* Printing */
//int BmDomain_strLen(BmDomain* self); // the minimal required buffer size to print the BmDomain `self`
char* BmDomain_wording(BmDomain* self); // print `self` on `output`
char* BmDomain_print(BmDomain* self, char* output); // print `self` at the end of `output`

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   W A N D A   S P A C E                                                 *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Represent a multi-dimentional variables' collection defined into finit and discret domains
  Space are dedicated to manages a collection of Domains
*/

typedef struct {
  uint dimention;
  char** varNames;
  BmDomain** varDomains;
} BmSpace;

/* Constructor Destructor */
BmSpace* newBmSpaceBasic(); // Create a new empty BmSpace
BmSpace* newBmSpace( uint dim, char ** variableNames, BmDomain ** variableDomains ); // Create a new instance of BmSpace of `dim` dimentions
BmSpace* newBmSpaceEmpty( uint dim ); // Create a new instance of BmSpace of `dim` dimentions
void deleteBmSpace(BmSpace* self); // Delete an instance of BmSpace
void deleteDeepBmSpace(BmSpace* self);

/* fill and empty a structure */
BmSpace* BmSpace_create( BmSpace* self, uint dim, char ** variableNames, BmDomain ** variableDomains ); // Create a BmSpace
BmSpace* BmSpace_createEmpty( BmSpace* self, uint dim ); // Create a BmSpace

BmSpace* BmSpace_destroy( BmSpace* self ); // Destroy a BmSpace
BmSpace* BmSpace_destroyDeep( BmSpace* self ); // Destroy a BmSpace and all its domains

/* Attach elements */
BmSpace* BmSpace_attachVariable( BmSpace* self, uint i, char* name, BmDomain* domain );

/* Test */
uint BmSpace_isValid(BmSpace* self);

/* Accessor */
uint BmSpace_dimention(BmSpace* self);             // "return the dimention of space (ie. the number of discrete variables composing the space)"
char* BmSpace_variable_name( BmSpace* self, int i );
BmDomain * BmSpace_variable_domain( BmSpace* self, int i ); // return the i-th domain (i in [1, BmSpace_dimention])
uint BmSpace_variable_domainSize(BmSpace* self, int i);    // "return the number of states (and stateId)"
ulong BmSpace_size(BmSpace* self);                 // "return the number of states (and stateId)"
BmCode* BmSpace_key_toNewCode(BmSpace* self, ulong key);

/* Search */
uint BmSpace_variableName_index(BmSpace* self, char* name);

/* Generators */
uint BmSpace_feedWithDomains( BmSpace* self, BmDomain** domainsBuffer, uint nboDomainsInBuffer ); // Generate the list of different domains; domainsBuffer should be of size BmSpace_dimention(self), the maximun number of different domains. return the number of different domains

/* Code Manipulation */
BmCode* BmSpace_asNewBmCode( BmSpace* self );// Generate the code form Space domains size

/* Code State */
char* BmSpace_printVariable_at( BmSpace* self, uint iVar, uint iVal, char* buffer );
char* BmSpace_code_wording( BmSpace* self, BmCode* code );
char* BmSpace_code_print( BmSpace* self, BmCode* code, char* output );

char* BmSpace_key_wording( BmSpace* self, ulong key );
char* BmSpace_key_print( BmSpace* self, ulong key, char* output );

/* Printing */
char* BmSpace_print(BmSpace* self, char* output);


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   W A N D A   P R O B A B I L I T Y                                     *                 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Probabilities are precise floating point variable (double) between 0.00 and 1.00 
  - Implementation in WdDistribution.c -
*/ 
char*  WdProbability_wording(double p); // Print probability on buffer and return buffer.

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   W A N D A   M O D E L                                                 *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Referring to Markov Decision process, a model is based on a state and action space, a transition and a reward function.
*/

typedef struct {
  BmSpace *state, *action, *shift;
  BmTransition *transition;
  //WdValueFct *reward;
  char** futureName;
} BmSystem;

/* Constructor Destructor */
BmSystem* newBmSystemEmpty( uint dimState, uint dimAction, uint dimShift );
BmSystem* newBmSystem( uint dimState, BmDomain ** stateDoms, uint dimAction, BmDomain ** actionDoms, uint dimShift, BmDomain ** shiftDomains );

void deleteBmSystem(BmSystem* self); // Delete an instance of BmSystem
void deleteDeepBmSystem(BmSystem* self);

/* fill and empty a structure */
BmSystem* BmSystem_createEmpty( BmSystem* self, uint dimState, uint dimAction, uint dimShift );
BmSystem* BmSystem_create( BmSystem* self, uint dimState, BmDomain ** stateDoms, uint dimAction, BmDomain ** actionDoms, uint dimShift, BmDomain ** shiftDomains );

BmSystem* BmSystem_deepDestroy( BmSystem* self );
BmSystem* BmSystem_destroy( BmSystem* self );

/* Initialize variable */
uint BmSystem_attachStateVariable( BmSystem* self, char * name, BmDomain* domain );
uint BmSystem_attachActionVariable( BmSystem* self, char * name, BmDomain* domain );
uint BmSystem_attachShiftVariable( BmSystem* self, char * name, BmDomain* domain );

BmCode* BmSystem_variable_dependOnArray( BmSystem* self, char * varName, uint parentSize, char ** parentNames );
BmCode* BmSystem_variable_dependOn( BmSystem* self, char * varName, uint parentSize, ... );

/* Initialize transition probability */
BmSystem* BmSystem_variable_initializeProbabilities( BmSystem* self, char* parent, uint numberOfOutputs, ... );
BmSystem* BmSystem_variable_addProbabilities( BmSystem* self, char * varName, uint numberOfParents, ... );

BmDistribution* BmSystem_nodeId_newBmDistribution(BmSystem* self, uint id, uint numberOfOutputs, char** outputs, double* probabilities );

/* Test */
uint BmSystem_isValid(BmSystem* self);

/* Update */

/* Accessor */
BmSpace* BmSystem_stateSpace(BmSystem* self);
BmSpace* BmSystem_actionSpace(BmSystem* self);
BmSpace* BmSystem_shiftSpace(BmSystem* self);
BmTransition* BmSystem_transition(BmSystem* self);
//WdValueFct* BmSystem_reward(BmSystem* self);

BmCondition* BmSystem_variable(BmSystem* self, char * varName);
uint BmSystem_variable_nodeId(BmSystem* self, char * varName);
char* BmSystem_nodeId_variableName(BmSystem* self, uint id);

uint BmSystem_nodeId_parentSize(BmSystem* self, uint id);
uint BmSystem_nodeId_parentId(BmSystem* self, uint id, char* parent);
BmDomain* BmSystem_nodeId_domain(BmSystem* self, uint id);

/* Generators */
uint BmSystem_feedWithDomains( BmSystem* self, BmDomain** domainsBuffer, uint nboDomainsInBuffer ); // Generate the list of different domains; domainsBuffer should be of size BmSpace_dimention(self), the maximun number of different domains. return the number of different domains

/* State */

/* Printing */
char* BmSystem_printNetwork( BmSystem* self, char* output );
char* BmSystem_printVariable( BmSystem* self, char* varName, char* output );

#endif // WANDA_MODEL_H