#include "bbmm-probabilities.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   V A L U E
 *
 * Define a Value function ( Code -> Value ) Based on tree selector
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */


/* Constructor Destructor */
BmValue* newBmValueBasic( BmCode* input, uint optionSize )
{
    return BmValue_createBasic( newEmpty(BmValue), input, optionSize );
}

//BmValue* newBmValue_options( BmCode* input, uint optionSize, double* options );

void deleteBmValue( BmValue* self )
{
    BmValue_distroy(self);
    free(self);
}

BmValue* BmValue_createBasic( BmValue* self, BmCode* input, uint optionSize )
{
    self->selector= newBmTree( input, optionSize );
    self->options= newEmptyArray(double, optionSize);
    return self;
}

//BmValue* BmValue_create_options( BmValue* self, uint optionSize, double options );

BmValue* BmValue_distroy( BmValue* self)
{
    deleteEmptyArray( self->options );
    deleteBmTree( self->selector );
    return self;
}

/* Construction */
BmValue* BmValue_optionId_set(BmValue* self, uint iOption, double value)
{
    array_at_set( self->options, iOption, value );
    return self;
}

/* Accessor */
uint BmValue_dimention( BmValue* self )
{
    return BmCode_size( self->selector->input );
}

uint BmValue_optionSize( BmValue* self )
{
    return self->selector->optionBound-1;
}

double BmValue_optionId(  BmValue* self, uint iOption )
{
    return array_at( self->options, iOption );
}
