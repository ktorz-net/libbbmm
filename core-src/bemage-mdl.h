/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   WANDA Model - A library in KISS philosophy for decision model under-uncertainty
 *
 *   FEATURES:
 *       - WdDomain       : Definie e finit set of possible values (words, integer-interval, cluster-means, .... )
 *       - WdSpace        : a vector of variable in a collection of domains
 *       - WdModel        : <S, A, t, r>
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

#include "wanda-stt.h"
#include "wanda-pbb.h"

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   W A N D A   D O M A I N                                               *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* 
  Represent a variation domain of finit discrete variables
*/

enum WdDomainType{
  WdDomainType_UNDEF= 0,
  WdDomainType_BASIC,
  WdDomainType_RANGE,
  WdDomainType_MEANS,
  WdDomainType_WORDS
}; //, WdDomainType_means};

typedef struct WdDomainStruct {
  char* name;
  uint type, size;
  void* dsc;
  char* (*str_fct)(struct WdDomainStruct *, uint);
  double (*value_fct)(struct WdDomainStruct *, uint);
} WdDomain;

/* Constructor Destructor */
WdDomain* newWdDomainBasic( char* name, uint size );   // Create a new instance of WdDomain of size `size`
WdDomain* newWdDomainRange( char* name, double from, double to, double by );   // Create a new instance of WdDomain of size `size`
WdDomain* newWdDomainWordsArray( char* name, uint size, char ** worldsArray );  // Create a new instance of WdDomain of size
WdDomain* newWdDomainWords( char* name, uint size, ... );
WdDomain* newWdDomainMeans( char* name, uint size, double* worlds );
WdDomain* newWdDomainAs( WdDomain* model );

//WdDomain* newWdDomainMeans(uint size, float * means); // Create a new instance of WdDomain of size
void deleteWdDomain(WdDomain* self); // Delete an instance of WdDomain

/* fill and empty a structure */
WdDomain* WdDomain_createBasic( WdDomain* self, char* name, uint size );
WdDomain* WdDomain_createRange( WdDomain* self, char* name, double from, double to, double by );
WdDomain* WdDomain_createWords( WdDomain* self, char* name, uint size, char ** worlds );
WdDomain* WdDomain_createMeans( WdDomain* self, char* name, uint size, double* means );
WdDomain* WdDomain_createAs( WdDomain* self, WdDomain* model );
WdDomain* WdDomain_distroy( WdDomain* self );

/* Accessor */
char* WdDomain_name(WdDomain* self);
uint WdDomain_size(WdDomain* self); // return the size of the wanda domain
double WdDomain_valueAt(WdDomain* self, uint index);
double WdDomainBasic_valueAt(struct WdDomainStruct* self, uint index);

double WdDomainRange_valueAt(struct WdDomainStruct* self, uint index);
double WdDomainMeans_valueAt(struct WdDomainStruct* elf, uint index);

/* Accessor - WdDomainRange */
double WdDomainRange_from(WdDomain* self); // return the size of the wanda domain
double WdDomainRange_to(WdDomain* self); // return the size of the wanda domain
double WdDomainRange_by(WdDomain* self); // return the size of the wanda domain

/* Accessor - WdDomainWords */

/* Engine */
uint WdDomain_searchStr( WdDomain* self, char* ref );
//uint WdDomain_searchValue( WdDomain* self, double ref );

/* Printing */
char* WdDomain_strAt(WdDomain* self, uint index); // return the str at a given `index`
char* WdDomain_strAt_fromValue(struct WdDomainStruct* self, uint index); // print `self` at the end of `output`
char* WdDomainWords_strAt(struct WdDomainStruct* self, uint index); // print `self` at the end of `output`

/* Test */
bool WdDomain_isType(WdDomain* self, enum WdDomainType type); // "return true if state is in the state space"

/* Search */
//uint WdDomain_classOf(WdDomain* self, char * item); // return the first index of an 'item' in the WdDomain (and size if not included)
//uint WdDomain_indexOf(WdDomain* self, char * item); // return the first index of an 'item' in the WdDomain (and size if not included)
//bool WdDomain_isIncluded(WdDomain* self, char * item); // Test if an item is in the domain

/* DomainStr */
//void WdDomain_at_copyItem(WdDomain* self, uint index, char * item); //copy a new `word` at a given `index`
//void WdDomain_copyAll(WdDomain* self, char ** domain ); //copy the overall string value

/* Printing */
//int WdDomain_strLen(WdDomain* self); // the minimal required buffer size to print the WdDomain `self`
char* WdDomain_wording(WdDomain* self); // print `self` on `output`
char* WdDomain_print(WdDomain* self, char* output); // print `self` at the end of `output`

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
  WdDomain** varDomains;
} WdSpace;

/* Constructor Destructor */
WdSpace* newWdSpaceBasic(); // Create a new empty WdSpace
WdSpace* newWdSpace( uint dim, char ** variableNames, WdDomain ** variableDomains ); // Create a new instance of WdSpace of `dim` dimentions
WdSpace* newWdSpaceEmpty( uint dim ); // Create a new instance of WdSpace of `dim` dimentions
void deleteWdSpace(WdSpace* self); // Delete an instance of WdSpace
void deleteDeepWdSpace(WdSpace* self);

/* fill and empty a structure */
WdSpace* WdSpace_create( WdSpace* self, uint dim, char ** variableNames, WdDomain ** variableDomains ); // Create a WdSpace
WdSpace* WdSpace_createEmpty( WdSpace* self, uint dim ); // Create a WdSpace

WdSpace* WdSpace_distroy( WdSpace* self ); // Distroy a WdSpace
WdSpace* WdSpace_distroyDeep( WdSpace* self ); // Distroy a WdSpace and all its domains

/* Attach elements */
WdSpace* WdSpace_attachVariable( WdSpace* self, uint i, char* name, WdDomain* domain );

/* Test */
uint WdSpace_isValid(WdSpace* self);

/* Accessor */
uint WdSpace_dimention(WdSpace* self);             // "return the dimention of space (ie. the number of discrete variables composing the space)"
char* WdSpace_variable_name( WdSpace* self, int i );
WdDomain * WdSpace_variable_domain( WdSpace* self, int i ); // return the i-th domain (i in [1, WdSpace_dimention])
uint WdSpace_variable_domainSize(WdSpace* self, int i);    // "return the number of states (and stateId)"
ulong WdSpace_size(WdSpace* self);                 // "return the number of states (and stateId)"
WdCode* WdSpace_key_toNewCode(WdSpace* self, ulong key);

/* Search */
uint WdSpace_variableName_index(WdSpace* self, char* name);

/* Generators */
uint WdSpace_feedWithDomains( WdSpace* self, WdDomain** domainsBuffer, uint nboDomainsInBuffer ); // Generate the list of different domains; domainsBuffer should be of size WdSpace_dimention(self), the maximun number of different domains. return the number of different domains

/* Code Manipulation */
WdCode* WdSpace_asNewWdCode( WdSpace* self );// Generate the code form Space domains size

/* Code State */
char* WdSpace_printVariable_at( WdSpace* self, uint iVar, uint iVal, char* buffer );
char* WdSpace_code_wording( WdSpace* self, WdCode* code );
char* WdSpace_code_print( WdSpace* self, WdCode* code, char* output );

char* WdSpace_key_wording( WdSpace* self, ulong key );
char* WdSpace_key_print( WdSpace* self, ulong key, char* output );

/* Printing */
char* WdSpace_print(WdSpace* self, char* output);


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
  WdSpace *state, *action, *shift;
  WdTransition *transition;
  //WdValueFct *reward;
  char** futureName;
} WdModel;

/* Constructor Destructor */
WdModel* newWdModelEmpty( uint dimState, uint dimAction, uint dimShift );
WdModel* newWdModel( uint dimState, WdDomain ** stateDoms, uint dimAction, WdDomain ** actionDoms, uint dimShift, WdDomain ** shiftDomains );

void deleteWdModel(WdModel* self); // Delete an instance of WdModel
void deleteDeepWdModel(WdModel* self);

/* fill and empty a structure */
WdModel* WdModel_createEmpty( WdModel* self, uint dimState, uint dimAction, uint dimShift );
WdModel* WdModel_create( WdModel* self, uint dimState, WdDomain ** stateDoms, uint dimAction, WdDomain ** actionDoms, uint dimShift, WdDomain ** shiftDomains );

WdModel* WdModel_deepDistroy( WdModel* self );
WdModel* WdModel_distroy( WdModel* self );

/* Initialize variable */
uint WdModel_attachStateVariable( WdModel* self, char * name, WdDomain* domain );
uint WdModel_attachActionVariable( WdModel* self, char * name, WdDomain* domain );
uint WdModel_attachShiftVariable( WdModel* self, char * name, WdDomain* domain );

WdCode* WdModel_variable_dependOnArray( WdModel* self, char * varName, uint parentSize, char ** parentNames );
WdCode* WdModel_variable_dependOn( WdModel* self, char * varName, uint parentSize, ... );

/* Initialize transition probability */
WdModel* WdModel_variable_initializeProbabilities( WdModel* self, char * varName, char* parent, uint numberOfOutputs, ... );
WdModel* WdModel_variable_addProbabilities( WdModel* self, char * varName, uint numberOfParents, ... );

/* Test */
uint WdModel_isValid(WdModel* self);

/* Update */

/* Accessor */
WdSpace* WdModel_stateSpace(WdModel* self);
WdSpace* WdModel_actionSpace(WdModel* self);
WdSpace* WdModel_shiftSpace(WdModel* self);
WdTransition* WdModel_transition(WdModel* self);
//WdValueFct* WdModel_reward(WdModel* self);

uint WdModel_variable_nodeId(WdModel* self, char * varName);
char* WdModel_nodeId_variableName(WdModel* self, uint id);

uint WdModel_nodeId_parentSize(WdModel* self, uint id);
uint WdModel_nodeId_parentId(WdModel* self, uint id, char* parent);
WdDomain* WdModel_nodeId_domain(WdModel* self, uint id);

/* Generators */
uint WdModel_feedWithDomains( WdModel* self, WdDomain** domainsBuffer, uint nboDomainsInBuffer ); // Generate the list of different domains; domainsBuffer should be of size WdSpace_dimention(self), the maximun number of different domains. return the number of different domains

/* State */

/* Printing */
char* WdModel_printNetwork( WdModel* self, char* output );
char* WdModel_printVariable( WdModel* self, char* varName, char* output );

#endif // WANDA_MODEL_H