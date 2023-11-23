/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   BbMm Model - Bayesian-based Markov-models
 * 
 *   FEATURES:
 *       - BmCondition    : Define a Bayesian Node (conditional probabilities over variable affectations)
 *       - BmTransition   : Define a Dynamic Bayesian Network as P(state' | state, action) 
 *       - BmReward       : A value function over multiple criteria
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

#ifndef BBMM_MODEL_H
#define BBMM_MODEL_H

#include "bbmm-structures.h"

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   M O D E L  :  C O N D I T I O N                             *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a Bayesian Node (conditional probabilities over variable affectations)
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint outputSize;
  BmCode* parentRanges;
  BmTree* selector;
  uint distribSize, distribCapacity;
  BmBench **distributions;
} BmCondition;

/* Constructor */
BmCondition* newBmConditionBasic(uint outputSize);
BmCondition* newBmConditionWith(uint domainSize, BmCode* newParentRanges, BmBench* newDefaultDistrib);

BmCondition* BmCondition_createBasic(BmCondition* self, uint outputSize);
BmCondition* BmCondition_createWith(BmCondition* self, uint domainSize, BmCode* newParentRanges, BmBench* newDefaultDistrib);

/* Destructor */
BmCondition* BmCondition_destroy(BmCondition* self);
void deleteBmCondition(BmCondition* instance);

/* re-initializer */
uint BmCondition_reinitWith( BmCondition* self, uint outputSize, BmCode* newParents, BmBench* newDistrib );
uint BmCondition_reinitDistributionsWith( BmCondition* self, BmBench* newDistrib );

/* Accessor */
uint BmCondition_output( BmCondition* self );
BmCode* BmCondition_parents( BmCondition* self );
BmBench* BmCondition_at( BmCondition* self, BmCode* configuration );
BmBench* BmCondition_atKey( BmCondition* self, uint configKey );
BmBench* BmCondition_distribution( BmCondition* self, uint iDistrib );

/* Construction */
uint BmCondition_at_attach( BmCondition* self, BmCode* configuration, BmBench* distribution );

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
 *   B b M m   M O D E L  :  T R A N S I T I O N                           *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a Bayesian Network composed of state, action and tramsitional nodes
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint stateDimention, actionDimention, overallDimention;
  BmBench* network;
  BmCondition* nodes;
  BmBench* transition;
} BmTransition;

/* Constructor*/
BmTransition* newBmTransition( BmCode* state, BmCode* action );
BmTransition* newBmTransitionShift( BmCode* state, BmCode* action, BmCode* shift );

BmTransition* BmTransition_create(BmTransition* self, BmCode* state, BmCode* action, BmCode* shift );

/* Destructor */
BmTransition* BmTransition_destroy(BmTransition* self);
void deleteBmTransition(BmTransition* self);

/* Accessor */
BmBench* BmTransition_distribution( BmTransition* self );

uint BmTransition_stateDimention( BmTransition* self );
uint BmTransition_actionDimention( BmTransition* self );
uint BmTransition_shiftDimention( BmTransition* self );
uint BmTransition_overallDimention( BmTransition* self );

uint BmTransition_indexOfStateVariableT0( BmTransition* self, uint iVar );
uint BmTransition_indexOfStateVariableT1( BmTransition* self, uint iVar );
uint BmTransition_indexOfActionVariable( BmTransition* self, uint iVar );
uint BmTransition_indexOfShiftVariable( BmTransition* self, uint iVar );

BmCondition* BmTransition_nodeAt( BmTransition* self, uint iVar );
uint BmTransition_sizeAt( BmTransition* self, uint iVar );
BmCode* BmTransition_dependanciesAt( BmTransition* self, uint iVar );

/* Construction */
BmCondition* BmTransition_node_reinitWith( BmTransition* self, uint index, BmCode* newDependenceMask, BmBench* newDistrib );

/* Infering */
BmBench* BmTransition_newDistributionByInfering( BmTransition* self, BmBench* partialDistribution );
BmBench* BmTransition_inferFromState_andAction( BmTransition* self, BmCode* state, BmCode* action );

/* Printing */
char* BmTransition_print(BmTransition* self, char* output); // print `self` at the end of `output`
char* BmTransition_printSignature(BmTransition* self, char* output); // print `self` at the end of `output`
char* BmTransition_printDependency(BmTransition* self, char* output); // print `self` at the end of `output`



/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   M O D E L  :  E V A L U A T O R                             *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a multi-critera Value function
 * (code -> value vector dot weight -> value)
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */



#endif // BBMM_MODEL_H