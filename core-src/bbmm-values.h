/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   BbMm Evaluation - A library in KISS philosophy of *Bayesian-based Markov-models* values manipulation.
 * 
 *   FEATURES:
 *       - BmVector       : a fixed size collection of values. 
 *       - BmGauge        : a distribution of values over configuration (codes)
 *       - BmEval         : a composed value function over an state and action space. 
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

#ifndef BBMM_VALUE_H
#define BBMM_VALUE_H

#include "bbmm-structures.h"

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   V A L U E  S :  V E C T O R 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint size;
  double * values;
} BmVector;

/* Constructor Destructor */
BmVector* newBmVectorBasic( uint size );
BmVector* newBmVector_values( uint size, double* values );
BmVector* newBmVector_list( uint size, double val1, ... );
BmVector* newBmVector_all( uint size, double* value );

BmVector* BmVector_createBasic( BmVector* self, uint size );
BmVector* BmVector_create_values( BmVector* self, uint size, double* values );
BmVector* BmVector_create_all( BmVector* self, uint size, double value );

BmVector* BmVector_distroy( BmVector* self );
void deleteBmVector( BmVector* self );

/* Initialize */
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

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   V A L U E S :  G A U G E
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a Simple Value function ( Code -> Value )
*/

typedef struct {
  BmTree* selector;
  BmVector* options;
} BmGauge;

/* Constructor Destructor */
BmGauge* newBmGaugeBasic( BmCode* input, uint optionSize );
BmGauge* newNmGaugeWith( BmTree* newSelector, BmVector* newOptions );

BmGauge* newBmGauge_options( BmCode* input, uint optionSize, double* options );

void deleteBmGauge( BmGauge* self );

BmGauge* BmGauge_createBasic( BmGauge* self, BmCode* input, uint optionSize );
BmGauge* BmGauge_createWith( BmGauge* self, BmTree* newSelector, BmVector* newOptions );

BmGauge* BmGauge_create_options( BmGauge* self, BmCode* input, uint optionSize, double* options );
BmGauge* BmGauge_distroy( BmGauge* self );

/* initialize */
//BmGauge* BmGauge_initializeBasic( BmGauge* self, BmCode* input, uint optionSize );
//BmGauge* BmGauge_initialize_options( BmGauge* self, BmCode* input, uint optionSize, double options );

/* Construction */
BmGauge* BmGauge_optionId_set(BmGauge* self, uint iOption, double value);
uint BmGauge_at_setOptionId( BmGauge* self, BmCode* code, uint iOption);

BmGauge* BmGauge_setList(BmGauge* self, uint number, ... );

/* Cleanning */

/* Accessor */
uint BmGauge_dimention( BmGauge* self );
uint BmGauge_optionSize( BmGauge* self );
double BmGauge_optionId(  BmGauge* self, uint iOption );

double BmGauge_at( BmGauge* self, BmCode* code); // Return the option value of a code.

/* Printing */
char* BmGauge_print( BmGauge* self, char* output);


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   V A L U E  S :  E V A L  
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a multi-critera Value function
  ( code -> value vector dot weight -> value )
*/

typedef struct {
  BmCode* variables;
  uint gaugeSize;
  BmGauge ** gauges;
  BmCode ** masks;
  BmVector* weights;
} BmEval ;

/* Constructor Destructor */
BmEval* newBmEvalBasic( uint codeDimention, uint numberOfGauges );
BmEval* newBmEvalWith( BmCode* newVariables, uint numberOfGauges );

BmEval* BmEval_createWith( BmEval* self, BmCode* newVariables, uint numberOfGauges );

void deleteBmEval( BmEval* self );
BmEval* BmEval_distroy( BmEval* self);

/* Construction */
BmEval* BmEval_initializeGauges( BmEval* self, uint gaugeSize );
//BmEval* BmEval_gaugeAt_initializeWith( BmEval* self, uint gaugeId, BmCode* newMask, BmVector* newOptions )
BmEval* BmEval_gaugeAt_initList( BmEval* self, uint gaugeId, uint optionSize, uint varSize, uint var1, ... );
BmEval* BmEval_weightAt_set( BmEval* self, uint gaugeId, double weight );

/* Cleanning */


/* Accessor */
uint BmEval_dimention( BmEval* self ); // dimention of input code.
uint BmEval_gaugeSize( BmEval* self ); // number of gauges.
BmGauge* BmEval_gaugeAt( BmEval* self, uint iGauge );

double BmEval_weightAt( BmEval* self, uint gaugeId);

/* Evaluation */
double BmEval_valueOf( BmEval* self, BmCode* code );

/* Printing */



#endif
