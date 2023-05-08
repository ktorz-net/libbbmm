#include "bbmm-values.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   C R I T E R I A
 *
 * Define a Value function ( Code -> Value ) Based on tree selector
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor Destructor */
BmCriteria* newBmCriteriaBasic( BmCode* input, uint optionSize )
{
    return BmCriteria_createBasic( newEmpty(BmCriteria), input, optionSize );
}

BmCriteria* newBmCriteria_options( BmCode* input, uint optionSize, double* options )
{
    return BmCriteria_create_options( newEmpty(BmCriteria), input, optionSize, options );
}

void deleteBmCriteria( BmCriteria* self )
{
    BmCriteria_distroy(self);
    free(self);
}

BmCriteria* BmCriteria_createBasic( BmCriteria* self, BmCode* input, uint optionSize )
{
    self->selector= newBmTree( input, optionSize );
    self->options= newEmptyArray(double, optionSize);
    return self;
}

BmCriteria* BmCriteria_create_options( BmCriteria* self, BmCode* input, uint optionSize, double* options )
{
    BmCriteria_createBasic(self, input, optionSize);
    for( uint i=0 ; i < optionSize ; ++i )
    {
        array_at_set( self->options, i+1, options[i] );
    }
    return self;
}

BmCriteria* BmCriteria_distroy( BmCriteria* self)
{
    deleteEmptyArray( self->options );
    deleteBmTree( self->selector );
    return self;
}

/* Construction */
BmCriteria* BmCriteria_optionId_set(BmCriteria* self, uint iOption, double value)
{
    array_at_set( self->options, iOption, value );
    return self;
}

uint BmCriteria_at_setOptionId( BmCriteria* self, BmCode* code, uint iOption)
{
    return BmTree_at_set( self->selector, code, iOption );
}

/* Accessor */
uint BmCriteria_dimention( BmCriteria* self )
{
    return BmCode_size( self->selector->input );
}

uint BmCriteria_optionSize( BmCriteria* self )
{
    return self->selector->optionBound-1;
}

double BmCriteria_optionId(  BmCriteria* self, uint iOption )
{
    return array_at( self->options, iOption );
}

double BmCriteria_at( BmCriteria* self, BmCode* code)
{
    uint iOption= BmTree_at( self->selector, code );
    return BmCriteria_optionId(self, iOption);
}
