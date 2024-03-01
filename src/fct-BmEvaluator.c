#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   M O D E L  :  E V A L U A T O R
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor*/
BmEvaluator* newBmEvaluatorBasic( uint spaceDimention, uint numberOfCriteria )
{
    return newBmEvaluatorWith( newBmCode_all( spaceDimention, 2 ), numberOfCriteria );
}

BmEvaluator* newBmEvaluatorWith( BmCode* newSpace, uint numberOfCriteria )
{
    return BmEvaluator_createWith( newEmpty(BmEvaluator), newSpace, numberOfCriteria );
}

BmEvaluator* BmEvaluator_createWith( BmEvaluator* self, BmCode* newSpace, uint numberOfCriteria )
{
    self->space= newSpace;
    self->ccriteria= newEmptyArray( BmCriterion*, numberOfCriteria );
    self->masks= newEmptyArray( BmCode*, numberOfCriteria );
    self->weights= newBmVector( numberOfCriteria );
        
    for( uint i= 1 ; i <= numberOfCriteria ; ++i )
    {
        array_at_set( self->ccriteria, i, newBmCriterionBasic(1, 1) );
        array_at_set( self->masks, i, newBmCode( 0 ) );
        BmVector_at_set( self->weights, i, 1.0 );
    }
    
    self->size= numberOfCriteria;
    return self;
}

/* Destructor */
BmEvaluator* BmEvaluatordestroy( BmEvaluator* self)
{
    for( uint i = 1 ; i < self->size ; ++i )
    {
        deleteBmCriterion( array_at( self->ccriteria, i ) );
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
    BmEvaluatordestroy( self );
    free( self );
}

/* Initializer */
BmEvaluator* BmEvaluator_reinitCriterion( BmEvaluator* self, uint numberOfCriterion )
{
    BmCode* savedSpace= newBmCode(1);
    BmCode_switch( self->space, savedSpace );
    BmEvaluatordestroy(self);
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

uint BmEvaluator_numberOfCriteria( BmEvaluator* self )
{
    return self->size;
}

BmCriterion* BmEvaluator_criterion( BmEvaluator* self, uint iCritirion )
{
    return array_at(self->ccriteria, iCritirion);
}

BmVector* BmEvaluator_weights( BmEvaluator* self )
{
    return self->weights;
}

double BmEvaluator_criterion_weight( BmEvaluator* self, uint iCritirion )
{
    return BmVector_value( self->weights, iCritirion );
}

BmCode* BmEvaluator_criterion_mask( BmEvaluator* self, uint iCritirion )
{
    return array_at( self->masks, iCritirion );
}


/* Process */
double BmEvaluator_process( BmEvaluator* self, BmCode* input )
{
    assert( BmCodeDimention(input) == BmCodeDimention(self->space) );
    double eval= 0.0;
    for( uint i= 1 ; i <= self->size ; ++i )
    {
        eval+= BmVector_value( self->weights, i )
            *  BmEvaluator_criterion_process( self, i, input );
    }
    return eval;
}

double BmEvaluator_criterion_process( BmEvaluator* self, uint iCriterion, BmCode* input )
{
    BmCode* critCode= BmCode_newBmCodeMask( input, array_at( self->masks, iCriterion ) );
    if( BmCodeDimention( critCode ) == 0 )
    {
        BmCode_reinit_list( critCode, 1, 1 );
    }
    double v= BmCriterion_from( array_at( self->ccriteria, iCriterion ), critCode );
    return v;
}

double BmEvaluator_processState_action(BmEvaluator* self, BmCode* state, BmCode* action)
{
    BmCode* merge= newBmCodeMerge_list( 2, state, action );
    assert( BmCode_isIncluding( self->space,  merge ) );
    double eval= BmEvaluator_process( self, merge );
    deleteBmCode( merge );
    return eval;
}

double BmEvaluator_processState_action_state(BmEvaluator* self, BmCode* state, BmCode* action, BmCode* statePrime)
{
    BmCode* merge= newBmCodeMerge_list( 3, state, action, statePrime );
    assert( BmCode_isIncluding( self->space,  merge ) );
    double eval= BmEvaluator_process( self, merge );
    deleteBmCode( merge );
    return eval;
}

/* Construction */
BmCriterion* BmEvaluator_criterion_reinitWith( BmEvaluator* self, uint iCrit, BmCode* newDependenceMask, BmVector* newValues  )
{
    // Build critirion spapce:
    BmCode* critSpace= BmCode_newBmCodeMask( self->space, newDependenceMask );

    // initialize criterion:
    BmCriterion_reinitWith(
        array_at( self->ccriteria, iCrit ),
        critSpace,
        newValues
    );
    
    // record the mask:
    BmCodedestroy( array_at( self->masks, iCrit ) );
    array_at_set( self->masks, iCrit, newDependenceMask );

    // and go:
    return array_at( self->ccriteria, iCrit );   
}

void BmEvaluator_criterion_from_set( BmEvaluator* self, uint iCrit, BmCode* input, uint output )
{
    // index criteria:
    BmCriterion_from_set( array_at( self->ccriteria, iCrit ), input, output  );
}

void BmEvaluator_criterion_setWeight( BmEvaluator* self, uint iCritirion, double weight )
{
    BmVector_at_set( self->weights, iCritirion, weight );
}
