/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 *   BbMm Evaluation - A library in KISS philosophy of *Bayesian-based Markov-models* values manipulation.
 * 
 *   FEATURES:
 *       - BmMatrice         : Define a 'BmBench' of values. 
 *       - BmCriteria        : Define a distribution of values over configuration (codes)
 *       - BmReward          : Define a composed value function over an state and action space. 
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
 *   B b M m   V A L U E  S :  M A T R I C E
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
 
*/


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   V A L U E S :  C R I T E R I A
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
  Define a Value function ( Code -> Value ) 
*/

typedef struct {
  BmTree* selector;
  double* options;
} BmCriteria;

/* Constructor Destructor */
BmCriteria* newBmCriteriaBasic( BmCode* input, uint optionSize );
BmCriteria* newBmCriteria_options( BmCode* input, uint optionSize, double* options );
void deleteBmCriteria( BmCriteria* self );

BmCriteria* BmCriteria_createBasic( BmCriteria* self, BmCode* input, uint optionSize );
BmCriteria* BmCriteria_create_options( BmCriteria* self, BmCode* input, uint optionSize, double* options );
BmCriteria* BmCriteria_distroy( BmCriteria* self);

/* initialize */
//BmCriteria* BmCriteria_initializeBasic( BmCriteria* self, BmCode* input, uint optionSize );
//BmCriteria* BmCriteria_initialize_options( BmCriteria* self, BmCode* input, uint optionSize, double options );

/* Construction */
BmCriteria* BmCriteria_optionId_set(BmCriteria* self, uint iOption, double value);
uint BmCriteria_at_setOptionId( BmCriteria* self, BmCode* code, uint iOption);

BmCriteria* BmCriteria_setList(BmCriteria* self, uint number, ... );


/* Cleanning */

/* Accessor */
uint BmCriteria_dimention( BmCriteria* self );
uint BmCriteria_optionSize( BmCriteria* self );
double BmCriteria_optionId(  BmCriteria* self, uint iOption );

double BmCriteria_at( BmCriteria* self, BmCode* code); // Return the option value of a code.

/* Printing */
char* BmCriteria_print( BmCriteria* self, char* output);


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   V A L U E S :  
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*
 
*/


#endif
