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
BmGauge* newBmGaugeBasic( BmCode* input, uint optionSize )
{
    return BmGauge_createBasic( newEmpty(BmGauge), input, optionSize );
}

BmGauge* newNmGaugeWith( BmTree* newSelector, BmVector* newOptions )
{
    return BmGauge_createWith( newEmpty(BmGauge), newSelector, newOptions );
}

BmGauge* newBmGauge_options( BmCode* input, uint optionSize, double* options )
{
    return BmGauge_create_options( newEmpty(BmGauge), input, optionSize, options );
}

void deleteBmGauge( BmGauge* self )
{
    BmGauge_destroy(self);
    free(self);
}

BmGauge* BmGauge_createBasic( BmGauge* self, BmCode* input, uint optionSize )
{
    self->selector= newBmTree( input, optionSize );
    self->options= newBmVectorBasic(optionSize);
    return self;
}
BmGauge* BmGauge_createWith( BmGauge* self, BmTree* newSelector, BmVector* newOptions )
{
    self->selector= newSelector;
    self->options= newOptions;
    return self;
}

BmGauge* BmGauge_create_options( BmGauge* self, BmCode* input, uint optionSize, double* options )
{
    BmGauge_createBasic(self, input, optionSize);
    for( uint i=0 ; i < optionSize ; ++i )
    {
        BmVector_at_set( self->options, i+1, options[i] );
    }
    return self;
}

BmGauge* BmGauge_destroy( BmGauge* self)
{
    deleteBmVector( self->options );
    deleteBmTree( self->selector );
    return self;
}

/* Construction */
BmGauge* BmGauge_optionId_set(BmGauge* self, uint iOption, double value)
{
    BmVector_at_set( self->options, iOption, value );
    return self;
}

uint BmGauge_at_setOptionId( BmGauge* self, BmCode* code, uint iOption)
{
    return BmTree_at_set( self->selector, code, iOption );
}


BmGauge* BmGauge_setList(BmGauge* self, uint number, ... )
{
    BmCode* code= newBmCodeBasic( BmGauge_dimention(self) );
    // Build number code -> iOption tuples 
    va_list ap;
    va_start(ap, number); 
    for ( uint i = 0 ; i < number ; ++i )
    {
        for ( uint iVar = 1 ; iVar <= BmCode_size(code) ; ++iVar )
        {
            BmCode_at_set(code, iVar, va_arg(ap, uint ) );
        }
        BmGauge_at_setOptionId( self, code, va_arg(ap, uint ) );
    }
    va_end(ap);
    // clean:
    deleteBmCode( code );
    return self;
}

/* Accessor */
uint BmGauge_dimention( BmGauge* self )
{
    return BmCode_size( self->selector->input );
}

uint BmGauge_optionSize( BmGauge* self )
{
    return self->selector->optionBound-1;
}

double BmGauge_optionId(  BmGauge* self, uint iOption )
{
    return BmVector_at( self->options, iOption );
}

double BmGauge_at( BmGauge* self, BmCode* code)
{
    uint iOption= BmTree_at( self->selector, code );
    return BmGauge_optionId(self, iOption);
}


/* Printing */
char* BmGauge_print( BmGauge* self, char* output)
{
    // prepare options
    uint optionsSize= BmGauge_optionSize(self);
    char** optionsStr= newEmptyArray( char*, optionsSize );
    for( uint i= 1 ; i <= optionsSize ; ++i )
    {
        array_at_set( optionsStr, i, newEmptyArray( char*, 32 ); )
        sprintf( array_at( optionsStr, i ), "%f", BmVector_at(self->options, i) );
    }

    // print
    BmTree_print_sep_options( self->selector, output, ",\n", optionsStr );
    
    // clear
    for( uint i= 1 ; i <= optionsSize ; ++i )
        deleteEmptyArray( array_at(optionsStr, i) );
    deleteEmptyArray( optionsStr );

    return output;
}
