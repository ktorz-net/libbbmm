/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   WANDA Probability - A library in KISS philosophy of
 *                       WANDA probility and condition models
 *
 *   FEATURES:
 *       - WdDistribution      : Define a distribution of probabilities over configuration (codes)
 *       - WdCondition         : Define a Bayesian Node (conditional probabilities over variable affectations)
 *       - WdTransition        : Define a Dynamic Bayesian Network composed of state, action and tramsitional nodes 
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

#ifndef WANDA_PROBABILITY_H
#define WANDA_PROBABILITY_H

#include "wanda-stt.h"


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   W A N D A   D I S T R I B U T I O N                                   *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a distribution of probabilities over configuration (codes)
*/

typedef struct {
  uint capacity, size;
  uint dimention;
  WdCode** configurations;
  double* probabilities;
} WdDistribution;

/* Constructor Destructor */
WdDistribution* newWdDistribution(uint dimention);
WdDistribution* newWdDistributionAs( WdDistribution* model );

void deleteWdDistribution(WdDistribution* instance);

/* fill and empty a structure */
WdDistribution* WdDistribution_create(WdDistribution* self, uint dimention);
WdDistribution* WdDistribution_createAs(WdDistribution* self, WdDistribution* model);
WdDistribution* WdDistribution_distroy(WdDistribution* self);

/* Switching double beffering */
void WdDistribution_switch(WdDistribution* self, WdDistribution* another);

/* Accessor */
uint WdDistribution_dimention(WdDistribution* self);
uint WdDistribution_size(WdDistribution* self);
WdCode* WdDistribution_configAt(WdDistribution* self, uint i );
uint WdDistribution_outputAt(WdDistribution* self, uint i );
double WdDistribution_probabilityAt(WdDistribution* self, uint i );
// double WdDistribution_probabilityOfState(WdDistribution* self, WdCode* configuration );

/* Test */

/* Construction */
void WdDistribution_initialize(WdDistribution* self, uint dimention);
void WdDistribution_initializeAs(WdDistribution* self, WdDistribution* model);
void WdDistribution_resizeCapacity(WdDistribution* self, uint newCapacity);
void WdDistribution_clear(WdDistribution* self);

/* Construction - configuration (Multi-dimentional element) */
uint WdDistribution_addConfig(WdDistribution* self, WdCode* configuration, double probability);
uint WdDistribution_setOnConfig(WdDistribution* self, WdCode* configuration);
void WdDistribution_setConfigs(WdDistribution* self, uint size, WdCode** states, double* probas );

/* Construction - output (uni-dimentional element) */
uint WdDistribution_addOutput(WdDistribution* self, uint output, double probability);
uint WdDistribution_setOnOutput(WdDistribution* self, uint output);

/* modification */
void WdDistribution_switchPosibilities(WdDistribution* self, uint index1, uint index2);

/* Process */
void WdDistribution_sort( WdDistribution* self );
uint WdDistribution_mergeDoubles( WdDistribution* self );
void WdDistribution_reduceDimentionTo( WdDistribution* self, uint newDimention, uint* mask );

/* Morphing */
//WdDistribution* WdDistribution_createReducedTo( WdDistribution* self, uint newDimention, uint* mask );
void WdDistribution_asReductionOf( WdDistribution* self, WdDistribution* model, uint* mask ); // mask size need to be egal or greter than self dimention

/* Printing */
void WdDistribution_puts(WdDistribution* self); // print `self` on stdout
char* WdDistribution_wording(WdDistribution* self); // print `self` on `output`
char* WdDistribution_print(WdDistribution* self, char* output); // print `self` at the end of `output`
char* WdDistribution_printPosibility(WdDistribution* self, uint index, char* out); // print one posibility of distribution `self` at the end of `output`

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   W A N D A   C O N D I T I O N                                         *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a Bayesian Node (conditional probabilities over variable affectations) 
*/

typedef struct {
  uint outputSize;
  WdCode* parentRanges;
  WdTree* selector;
  uint distribSize, distribCapacity;
  WdDistribution* * distributions;
} WdCondition;

/* Constructor Destructor */
WdCondition* newWdCondition(uint outputSize, WdCode* parentRanges, WdDistribution* defaultDistrib);
WdCondition* newWdConditionBasic(uint domainSize, WdCode* parentRanges);
WdCondition* newWdConditionUndependant(uint outputSize);

WdCondition* WdCondition_create(WdCondition* self, uint outputSize, WdCode* parentRanges, WdDistribution* defaultDistrib);
WdCondition* WdCondition_createBasic(WdCondition* self, uint outputSize, WdCode* parentRanges);
WdCondition* WdCondition_createUndependant(WdCondition* self, uint outputSize);
WdCondition* WdCondition_distroy(WdCondition* self);

void deleteWdCondition(WdCondition* instance);

/* initialize */
void WdCondition_reinitialize(WdCondition* self, WdCode* parentRanges, WdDistribution* defaultDistrib);
//void WdCondition_initializeBasic(WdCondition* self);
//void WdCondition_initializeEquiprobable(WdCondition* self);

/* Accessor */
uint WdCondition_outputSize( WdCondition* self );
WdCode* WdCondition_parentRanges( WdCondition* self );
uint WdCondition_parentSize( WdCondition* self );
uint WdCondition_dimention( WdCondition* self );
WdDistribution* WdCondition_at( WdCondition* self, WdCode* configuration );
uint WdCondition_distributionIndexAt( WdCondition* self, WdCode* configuration );
WdDistribution* WdCondition_atKey( WdCondition* self, uint configKey );

/* Inferring */
WdDistribution* WdCondition_infer( WdCondition* self, WdDistribution* distribOverConfigurations );
WdDistribution* WdCondition_newDistributionByInfering( WdCondition* self, WdDistribution* distribOverConfigurations );
WdDistribution* WdCondition_newDistributionByInfering_mask(WdCondition* self, WdDistribution* longDistrib, WdCode* parentMask);

/* Test */

/* Construction*/
/* Construction tools*/
uint WdCondition_resizeDistributionCapacity( WdCondition* self, uint newCapacity );
uint WdCondition_at_set( WdCondition* self, WdCode* configuration, WdDistribution* distribution );

void WdCondition_at_addOutput_onProbability( WdCondition* self, WdCode* configutation, uint option, double probability );

/* Printing */
char* WdCondition_printCode(WdCondition* self, WdCode* code, char* output);
char* WdCondition_print(WdCondition* self, char* output);
char* WdCondition_printSep(WdCondition* self, char* output, char* separator);

char* WdCondition_wordingExtend( WdCondition* self ); // print `self` on `output`
char* WdCondition_printExtend(WdCondition* self, char* output); // print `self` at the end of `output`
char* WdCondition_printExtendSep(WdCondition* self, char* output, char* separator);

char* WdCondition_printIdentity( WdCondition* self, char* output ); // print `self` at the end of on `output`

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   W A N D A   T R A N S I T I O N
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a Bayesian Network composed of state, action and tramsitional nodes 
*/ 

typedef struct {
  uint stateDimention, actionDimention, overallDimention;
  WdNet* network;
  WdCondition* nodes;
  WdDistribution* transition;
} WdTransition;

/* Constructor Destructor */
WdTransition* newWdTransition( WdCode* state, WdCode* action );
WdTransition* newWdTransitionWithShift( WdCode* state, WdCode* action, WdCode* shift );
void deleteWdTransition(WdTransition* self);

/* fill and empty a structure */
WdTransition* WdTransition_create(WdTransition* self, WdCode* state, WdCode* action, WdCode* shift );
WdTransition* WdTransition_distroy(WdTransition* self);

/* Accessor */
WdDistribution* WdTransition_distribution( WdTransition* self );

uint WdTransition_stateDimention( WdTransition* self );
uint WdTransition_actionDimention( WdTransition* self );
uint WdTransition_shiftDimention( WdTransition* self );
uint WdTransition_overallDimention( WdTransition* self );

uint WdTransition_indexOfStateVariableT0( WdTransition* self, uint iVar );
uint WdTransition_indexOfStateVariableT1( WdTransition* self, uint iVar );
uint WdTransition_indexOfActionVariable( WdTransition* self, uint iVar );
uint WdTransition_indexOfShiftVariable( WdTransition* self, uint iVar );

WdCondition* WdTransition_nodeAt( WdTransition* self, uint iVar );
uint WdTransition_sizeAt( WdTransition* self, uint iVar );
WdCode* WdTransition_dependanciesAt( WdTransition* self, uint iVar );

/* Construction */
WdTransition* WdTransition_node_initialize( WdTransition* self, uint index, uint outputSize );
WdTransition* WdTransition_node_dependArray( WdTransition* self, uint index, uint parentSize, uint* parents );
WdTransition* WdTransition_node_depends( WdTransition* self, uint index, uint parentSize, ... );

/* Infering */
WdDistribution* WdTransition_newDistributionByInfering( WdTransition* self, WdDistribution* partialDistribution );
WdDistribution* WdTransition_inferFromState_andAction( WdTransition* self, WdCode* state, WdCode* action );

/* Printing */
char* WdTransition_print(WdTransition* self, char* output); // print `self` at the end of `output`
char* WdTransition_printSignature(WdTransition* self, char* output); // print `self` at the end of `output`
char* WdTransition_printDependency(WdTransition* self, char* output); // print `self` at the end of `output`

#endif
