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


BmCriteria* BmCriteria_setList(BmCriteria* self, uint number, ... )
{
    BmCode* code= newBmCodeBasic( BmCriteria_dimention(self) );
    // Build number code -> iOption tuples 
    va_list ap;
    va_start(ap, number); 
    for ( uint i = 0 ; i < number ; ++i )
    {
        for ( uint iVar = 1 ; iVar <= BmCode_size(code) ; ++iVar )
        {
            BmCode_at_set(code, iVar, va_arg(ap, uint ) );
        }
        BmCriteria_at_setOptionId( self, code, va_arg(ap, uint ) );
    }
    va_end(ap);
    // clean:
    deleteBmCode( code );
    return self;
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


/* Printing */
char* BmCriteria_print( BmCriteria* self, char* output)
{
    // prepare options
    uint optionsSize= BmCriteria_optionSize(self);
    char** optionsStr= newEmptyArray( char*, optionsSize );
    for( uint i= 1 ; i <= optionsSize ; ++i )
    {
        array_at_set( optionsStr, i, newEmptyArray( char*, 32 ); )
        sprintf( array_at( optionsStr, i ), "%f", array_at(self->options, i) );
    }

    // print
    BmTree_print_sep_options( self->selector, output, ",\n", optionsStr );
    
    // clear
    for( uint i= 1 ; i <= optionsSize ; ++i )
        deleteEmptyArray( array_at(optionsStr, i) );
    deleteEmptyArray( optionsStr );

    return output;
}
