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
    self->criteria= newEmptyArray( BmTree*, numberOfCriteria );
    self->critValues= newEmptyArray( BmVector*, numberOfCriteria );
    self->masks= newEmptyArray( BmCode*, numberOfCriteria );
    self->weights= newBmVector( numberOfCriteria );
        
    for( uint i= 1 ; i <= numberOfCriteria ; ++i )
    {
        array_at_set( self->criteria, i, newBmTree(0) );
        array_at_set( self->critValues, i, newBmVector(1) );
        array_at_set( self->masks, i, newBmCode( 0 ) );
        BmVector_at_set( self->weights, i, 1.0 );
    }
    
    self->criteriaSize= numberOfCriteria;
    return self;
}

/* Destructor */
BmEvaluator* BmEvaluator_destroy( BmEvaluator* self)
{
    for( uint i = 1 ; i < self->criteriaSize ; ++i )
    {
        deleteBmTree( array_at( self->criteria, i ) );
        deleteBmVector( array_at( self->critValues, i ) );
        deleteBmCode( array_at( self->masks, i ) );
    }

    deleteEmptyArray( self->criteria );
    deleteEmptyArray( self->critValues );
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
BmEvaluator* BmEvaluator_reinitCriterion( BmEvaluator* self, uint numberOfCriterion )
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

uint BmEvaluator_numberOfCriteria( BmEvaluator* self )
{
    return self->criteriaSize;
}

BmTree* BmEvaluator_crit( BmEvaluator* self, uint iCritirion )
{
    return array_at(self->criteria, iCritirion);
}

BmVector* BmEvaluator_crit_values( BmEvaluator* self, uint iCritirion )
{
    return array_at(self->critValues, iCritirion);
}

BmVector* BmEvaluator_weights( BmEvaluator* self )
{
    return self->weights;
}

double BmEvaluator_crit_weight( BmEvaluator* self, uint iCritirion )
{
    return BmVector_at( self->weights, iCritirion );
}


/* Process */
double BmEvaluator_process( BmEvaluator* self, BmCode* input )
{
    double eval= 0.0;
    for( uint i= 1 ; i <= self->criteriaSize ; ++i )
    {
        eval+= BmVector_at( self->weights, i )
            *  BmEvaluator_crit_process( self, i, input );
    }
    return eval;
}

double BmEvaluator_processState_action(BmEvaluator* self, BmCode* state, BmCode* action)
{
    BmCode* merge= newBmCodeMergeList( 2, state, action );
    assert( BmCode_isIncluding( self->space,  merge ) );
    double eval= BmEvaluator_process( self, merge );
    deleteBmCode( merge );
    return eval;
}

double BmEvaluator_processState_action_state(BmEvaluator* self, BmCode* state, BmCode* action, BmCode* statePrime)
{
    BmCode* merge= newBmCodeMergeList( 3, state, action, statePrime );
    assert( BmCode_isIncluding( self->space,  merge ) );
    double eval= BmEvaluator_process( self, merge );
    deleteBmCode( merge );
    return eval;
}

double BmEvaluator_crit_process( BmEvaluator* self, uint iCriterion, BmCode* input )
{
    BmCode* critCode= BmCode_newBmCodeMask( input, array_at( self->masks, iCriterion ) );
    uint outputIndex= BmTree_at( array_at( self->criteria, iCriterion ), critCode );
    double value= BmVector_at( array_at( self->critValues, iCriterion ), outputIndex );
    deleteBmCode( critCode );
    return value;
}


/* Construction */
BmTree* BmEvaluator_crit_reinitWith( BmEvaluator* self, uint iCrit, BmCode* newDependenceMask, BmVector* newValues  )
{
    // initialize criterion:
    BmTree* criterion= array_at( self->criteria, iCrit );
    
    // make blanc page:
    BmTree_destroy( criterion );
    deleteBmVector( array_at( self->critValues, iCrit ) );
    
    // Initialize the structure:
    BmCode* critSpace= BmCode_newBmCodeMask( self->space, newDependenceMask );
    BmTree_createWhith( criterion, critSpace );
    array_at_set( self->critValues, iCrit, newValues );
    
    // record the mask:
    BmCode_destroy( array_at( self->masks, iCrit ) );
    array_at_set( self->masks, iCrit, newDependenceMask );

    // and go:
    return criterion;   
}

void BmEvaluator_crit_at_set( BmEvaluator* self, uint iCrit, BmCode* option, uint output )
{
    // index criteria:
    BmTree* criterion= array_at( self->criteria, iCrit );
    BmTree_at_set( criterion, option, output );
}

void BmEvaluator_crit_setWeight( BmEvaluator* self, uint iCritirion, double weight )
{
    BmVector_at_set( self->weights, iCritirion, weight );
}
