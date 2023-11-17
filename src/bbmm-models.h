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

/* instance basics */
void BmCondition_switch(BmCondition* self, BmCondition* doppelganger);

/* re-initializer */
void BmCondition_reinit( BmCondition* self, uint outputSize, BmCode* parentRanges, BmBench* defaultDistrib );
void BmCondition_reinitEquiprobable( BmCondition* self, uint outputSize, BmCode* parentRanges );

void BmCondition_reiniteDefaultDistrib( BmCondition* self, BmBench* defaultDistrib );
void BmCondition_setEquiprobable( BmCondition* self );

/* Accessor */
uint BmCondition_outputSize( BmCondition* self );
BmCode* BmCondition_parentRanges( BmCondition* self );
uint BmCondition_parentSize( BmCondition* self );
uint BmCondition_dimention( BmCondition* self );
BmBench* BmCondition_at( BmCondition* self, BmCode* configuration );
uint BmCondition_distributionIndexAt( BmCondition* self, BmCode* configuration );
BmBench* BmCondition_atKey( BmCondition* self, uint configKey );

/* Inferring */
BmBench* BmCondition_infer( BmCondition* self, BmBench* distribOverConfigurations );
BmBench* BmCondition_newDistributionByInfering( BmCondition* self, BmBench* distribOverConfigurations );
BmBench* BmCondition_newDistributionByInfering_mask(BmCondition* self, BmBench* longDistrib, BmCode* parentMask);

/* Test */

/* Construction*/

/* Construction tools*/
uint BmCondition_resizeDistributionCapacity( BmCondition* self, uint newCapacity );

uint BmCondition_at_set( BmCondition* self, BmCode* configuration, BmBench* distribution );
uint BmCondition_at_readOrder_set( BmCondition* self, BmCode* configuration, BmCode* configOrder, BmBench* distribution );

void BmCondition_at_addOutput_onProbability( BmCondition* self, BmCode* configutation, uint option, double probability );

/* Printing */
char* _BmCondition_printCode_withDistribution(BmCondition* self, BmCode* code, uint iDistirb, char* output);
char* BmCondition_print(BmCondition* self, char* output);
char* BmCondition_printSep(BmCondition* self, char* output, char* separator);

char* BmCondition_wordingExtend( BmCondition* self ); // print `self` on `output`
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


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   M O D E L  :  R E W A R D                                   *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a multi-critera Value function
 * (code -> value vector dot weight -> value)
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#endif // BBMM_MODEL_H