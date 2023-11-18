/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   BbMm Probability - A library in KISS philosophy of *Bayesian-based Markov-models* probability manipulation.
 * 
 *   FEATURES:
 *       - BmDistribution      : Define a distribution of probabilities over configuration (codes)
 *       - BmCondition         : Define a Bayesian Node (conditional probabilities over variable affectations)
 *       - BmTransition        : Define a Dynamic Bayesian Network composed of state, action and tramsitional nodes 
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

#ifndef BBMM_PROBABILITY_H
#define BBMM_PROBABILITY_H

#include "bbmm-structures.h"

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   D I S T R I B U T I O N                                   *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a distribution of probabilities over configuration (codes)
*/

typedef struct {
  uint capacity, size;
  uint dimention;
  BmCode** configurations;
  double* probabilities;
} BmDistribution;

/* Constructor Destructor */
BmDistribution* newBmDistribution( uint dimention );
BmDistribution* newBmDistributionAs( BmDistribution* model );

void deleteBmDistribution(BmDistribution* instance);

/* fill and empty a structure */
BmDistribution* BmDistribution_create(BmDistribution* self, uint dimention);
BmDistribution* BmDistribution_createAs(BmDistribution* self, BmDistribution* model);
BmDistribution* BmDistribution_destroy(BmDistribution* self);

/* Switching double beffering */
void BmDistribution_switch(BmDistribution* self, BmDistribution* another);

/* Accessor */
uint BmDistribution_dimention(BmDistribution* self);
uint BmDistribution_size(BmDistribution* self);
BmCode* BmDistribution_configAt(BmDistribution* self, uint i );
uint BmDistribution_outputAt(BmDistribution* self, uint i );
double BmDistribution_probabilityAt(BmDistribution* self, uint i );
// double BmDistribution_probabilityOfState(BmDistribution* self, BmCode* configuration );

/* Test */

/* Construction */
void BmDistribution_initialize(BmDistribution* self, uint dimention);
void BmDistribution_initializeAs(BmDistribution* self, BmDistribution* model);
void BmDistribution_resizeCapacity(BmDistribution* self, uint newCapacity);
void BmDistribution_clear(BmDistribution* self);

/* Construction - configuration (Multi-dimentional element) */
uint BmDistribution_addConfig(BmDistribution* self, BmCode* configuration, double probability);
uint BmDistribution_setOnConfig(BmDistribution* self, BmCode* configuration);
void BmDistribution_setConfigs(BmDistribution* self, uint size, BmCode** states, double* probas );

/* Construction - output (uni-dimentional element) */
uint BmDistribution_addOutput(BmDistribution* self, uint output, double probability);
uint BmDistribution_setOnOutput(BmDistribution* self, uint output);

/* modification */
void BmDistribution_switchPosibilities(BmDistribution* self, uint index1, uint index2);

/* Process */
void BmDistribution_sort( BmDistribution* self );
uint BmDistribution_mergeDoubles( BmDistribution* self );
void BmDistribution_reduceDimentionTo( BmDistribution* self, uint newDimention, uint* mask );

/* Morphing */
//BmDistribution* BmDistribution_createReducedTo( BmDistribution* self, uint newDimention, uint* mask );
void BmDistribution_asReductionOf( BmDistribution* self, BmDistribution* model, uint* mask ); // mask size need to be egal or greter than self dimention

/* Printing */
void BmDistribution_puts(BmDistribution* self); // print `self` on stdout
char* BmDistribution_wording(BmDistribution* self); // print `self` on `output`
char* BmDistribution_print(BmDistribution* self, char* output); // print `self` at the end of `output`
char* BmDistribution_printPosibility(BmDistribution* self, uint index, char* out); // print one posibility of distribution `self` at the end of `output`

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   C O N D I T I O N                                         *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a Bayesian Node (conditional probabilities over variable affectations) 
*/

typedef struct {
  uint outputSize;
  BmCode* parentRanges;
  BmTree* selector;
  uint distribSize, distribCapacity;
  BmDistribution* * distributions;
} BmCondition;

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   T R A N S I T I O N
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a Bayesian Network composed of state, action and tramsitional nodes 
*/

typedef struct {
  uint stateDimention, actionDimention, overallDimention;
  BmBench* network;
  BmCondition* nodes;
  BmDistribution* transition;
} BmTransition;

/* Constructor Destructor */
BmTransition* newBmTransition( BmCode* state, BmCode* action );
BmTransition* newBmTransitionWithShift( BmCode* state, BmCode* action, BmCode* shift );
void deleteBmTransition(BmTransition* self);

/* fill and empty a structure */
BmTransition* BmTransition_create(BmTransition* self, BmCode* state, BmCode* action, BmCode* shift );
BmTransition* BmTransition_destroy(BmTransition* self);

/* Accessor */
BmDistribution* BmTransition_distribution( BmTransition* self );

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
BmTransition* BmTransition_node_initialize( BmTransition* self, uint index, uint outputSize );
BmTransition* BmTransition_node_dependArray( BmTransition* self, uint index, uint parentSize, uint* parents );
BmTransition* BmTransition_node_depends( BmTransition* self, uint index, uint parentSize, ... );

/* Infering */
BmDistribution* BmTransition_newDistributionByInfering( BmTransition* self, BmDistribution* partialDistribution );
BmDistribution* BmTransition_inferFromState_andAction( BmTransition* self, BmCode* state, BmCode* action );

/* Printing */
char* BmTransition_print(BmTransition* self, char* output); // print `self` at the end of `output`
char* BmTransition_printSignature(BmTransition* self, char* output); // print `self` at the end of `output`
char* BmTransition_printDependency(BmTransition* self, char* output); // print `self` at the end of `output`

#endif
