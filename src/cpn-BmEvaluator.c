#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   M O D E L  :  E V A L U A T O R
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor*/
BmEvaluator* newBmEvaluatorBasic( digit spaceDimention, digit numberOfCriteria )
{
    return newBmEvaluatorWith( newBmCode_all( spaceDimention, 2 ), numberOfCriteria );
}

BmEvaluator* newBmEvaluatorWith( BmCode* newSpace, digit numberOfCriteria )
{
    return BmEvaluator_createWith( newEmpty(BmEvaluator), newSpace, numberOfCriteria );
}

BmEvaluator* BmEvaluator_createWith( BmEvaluator* self, BmCode* newSpace, digit numberOfCriteria )
{
    self->space= newSpace;
    self->ccriteria= newEmptyArray( BmValueFct*, numberOfCriteria );
    self->masks= newEmptyArray( BmCode*, numberOfCriteria );
    self->weights= newBmVector( numberOfCriteria );
        
    for( digit i= 1 ; i <= numberOfCriteria ; ++i )
    {
        array_at_set( self->ccriteria, i, newBmValueFctBasic(1, 1) );
        array_at_set( self->masks, i, newBmCode( 0 ) );
        BmVector_at_set( self->weights, i, 1.0 );
    }
    
    self->size= numberOfCriteria;
    return self;
}

/* Destructor */
BmEvaluator* BmEvaluator_destroy( BmEvaluator* self)
{
    for( digit i = 1 ; i < self->size ; ++i )
    {
        deleteBmValueFct( array_at( self->ccriteria, i ) );
        deleteBmCode( array_at( self->masks, i ) );
    }

    deleteEmptyArray( self->ccriteria );
    deleteEmptyArray( self->masks );
    deleteBmVector( self->weights );
    deleteBmCode( self->space );
    return self;
}

void deleteBmEvaluator( BmEvaluator* self )
{
    BmEvaluator_destroy( self );
    free( self );
}

/* Initializer */
BmEvaluator* BmEvaluator_reinitCriterion( BmEvaluator* self, digit numberOfCriterion )
{
    BmCode* savedSpace= newBmCode(1);
    BmCode_switch( self->space, savedSpace );
    BmEvaluator_destroy(self);
    return BmEvaluator_createWith(
        self,
        savedSpace,
        numberOfCriterion
    );
}

/* Accessor */
BmCode* BmEvaluator_space( BmEvaluator* self )
{
    return self->space;
}

digit BmEvaluator_numberOfCriteria( BmEvaluator* self )
{
    return self->size;
}

BmValueFct* BmEvaluator_criterion( BmEvaluator* self, digit iCritirion )
{
    return array_at(self->ccriteria, iCritirion);
}

BmVector* BmEvaluator_weights( BmEvaluator* self )
{
    return self->weights;
}

double BmEvaluator_criterion_weight( BmEvaluator* self, digit iCritirion )
{
    return BmVector_value( self->weights, iCritirion );
}

BmCode* BmEvaluator_criterion_mask( BmEvaluator* self, digit iCritirion )
{
    return array_at( self->masks, iCritirion );
}


/* Process */
double BmEvaluator_process( BmEvaluator* self, BmCode* input )
{
    assert( BmCode_dimention(input) == BmCode_dimention(self->space) );
    double eval= 0.0;
    for( digit i= 1 ; i <= self->size ; ++i )
    {
        eval+= BmVector_value( self->weights, i )
            *  BmEvaluator_criterion_process( self, i, input );
    }
    return eval;
}

double BmEvaluator_criterion_process( BmEvaluator* self, digit iCriterion, BmCode* input )
{
    BmCode* critCode= BmCode_newBmCodeMask( input, array_at( self->masks, iCriterion ) );
    if( BmCode_dimention( critCode ) == 0 )
    {
        BmCode_reinit( critCode, 1 );
        BmCode_at_set( critCode, 1, 1 );
    }
    double v= BmValueFct_from( array_at( self->ccriteria, iCriterion ), critCode );
    return v;
}

double BmEvaluator_processState_action(BmEvaluator* self, BmCode* state, BmCode* action)
{
    BmCode* spaces[2]= {state, action};
    BmCode* merge= BmCode_createMerge( newEmpty(BmCode), 2, spaces );
    assert( BmCode_isIncluding( self->space,  merge ) );
    double eval= BmEvaluator_process( self, merge );
    deleteBmCode( merge );
    return eval;
}

double BmEvaluator_processState_action_state(BmEvaluator* self, BmCode* state, BmCode* action, BmCode* statePrime)
{
    BmCode* spaces[3]= {state, action, statePrime};
    BmCode* merge= BmCode_createMerge( newEmpty(BmCode), 3, spaces );
    assert( BmCode_isIncluding( self->space,  merge ) );
    double eval= BmEvaluator_process( self, merge );
    deleteBmCode( merge );
    return eval;
}

/* Construction */
BmValueFct* BmEvaluator_criterion_reinitWith( BmEvaluator* self, digit iCrit, BmCode* newDependenceMask, BmVector* newValues  )
{
    // Build critirion spapce:
    BmCode* critSpace= BmCode_newBmCodeMask( self->space, newDependenceMask );

    // initialize criterion:
    BmValueFct_reinitWith(
        array_at( self->ccriteria, iCrit ),
        critSpace,
        newValues
    );
    
    // record the mask:
    BmCode_destroy( array_at( self->masks, iCrit ) );
    array_at_set( self->masks, iCrit, newDependenceMask );

    // and go:
    return array_at( self->ccriteria, iCrit );   
}

void BmEvaluator_criterion_from_set( BmEvaluator* self, digit iCrit, BmCode* input, digit output )
{
    // index criteria:
    BmValueFct_from_set( array_at( self->ccriteria, iCrit ), input, output  );
}

void BmEvaluator_criterion_setWeight( BmEvaluator* self, digit iCritirion, double weight )
{
    BmVector_at_set( self->weights, iCritirion, weight );
}
