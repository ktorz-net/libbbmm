#include "bbmm-values.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   V A L U E F C T
 *
 *  Define a multi critera Value function from state and action space
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Private Function : */
BmEval* _BmEval_gaugesCreate( BmEval* self, uint gaugeSize );
BmEval* _BmEval_gaugesDestroy( BmEval* self );

/* Constructor Destructor */
BmEval* newBmEvalBasic( uint codeDimention, uint numberOfGauges  )
{
    return newBmEvalWith( newBmCode_all( codeDimention, 2 ), numberOfGauges );
}

BmEval* newBmEvalWith( BmCode* newVariables, uint numberOfGauges )
{
    return BmEval_createWith( newEmpty(BmEval), newVariables, numberOfGauges );
}

BmEval* BmEval_createWith( BmEval* self, BmCode* newVariables, uint numberOfGauges )
{
    self->variables= newVariables;
    _BmEval_gaugesCreate( self, numberOfGauges );
    return self;
}

void deleteBmEval( BmEval* self )
{
    BmEval_destroy( self );
    free( self );
}

BmEval* BmEval_destroy( BmEval* self )
{
    _BmEval_gaugesDestroy(self);
    deleteBmCode( self->variables );
    return self;
}

/* Constructor Destructor: Private */
BmEval* _BmEval_gaugesCreate( BmEval* self, uint gaugeSize )
{
    double outputs[1]= {0.0};
    self->gauges= newEmptyArray( BmGauge*, gaugeSize );
    self->masks= newEmptyArray( BmCode*, gaugeSize );
    self->weights= newBmVectorBasic( gaugeSize );

    BmCode* mask= newBmCode( BmCode_dimention( self->variables ) );
    for( uint i= 1 ; i <= BmCode_dimention( self->variables ) ; ++i )
        BmCode_at_set( mask, i , i );
        
    for( uint i= 1 ; i <= gaugeSize ; ++i )
    {
        array_at_set(
            self->gauges, i,
            newBmGauge_options( self->variables, 1, outputs)
        );
        array_at_set( self->masks, i, newBmCodeAs( mask ) );
        BmVector_at_set( self->weights, i, 1.0 );
    }
    
    deleteBmCode( mask );
    self->gaugeSize= gaugeSize;
    return self;
}

BmEval* _BmEval_gaugesDestroy( BmEval* self )
{
    for( uint i = 1 ; i < self->gaugeSize ; ++i )
    {
        deleteBmGauge( array_at( self->gauges, i ) );
        deleteBmCode( array_at( self->masks, i ) );
    }

    deleteEmptyArray( self->gauges );
    deleteEmptyArray( self->masks );
    deleteBmVector( self->weights );

    return self;
}

/* Construction */
BmEval* BmEval_initializeGauges( BmEval* self, uint gaugeSize )
{
    _BmEval_gaugesDestroy( self );
    _BmEval_gaugesCreate( self, gaugeSize );
    return self;
}

BmEval* BmEval_gaugeAt_initList( BmEval* self, uint gaugeId, uint optionSize, uint varSize, uint var1, ... )
{
    uint variables[varSize];

    // Build variable array from args
    va_list ap;
    variables[0]= var1;
    va_start(ap, var1); 
    for ( uint i = 1 ; i < varSize ; ++i )
    {
        variables[i]= va_arg(ap, uint);
    }
    va_end(ap);

    BmCode* dependencies= newBmCode(varSize);
    for( uint i= 1 ; i <= varSize ; ++i )
        BmCode_at_set( dependencies, i, BmCode_at( self->variables, variables[i-1] ) );

    // initialize gauge:
    BmGauge_destroy( array_at( self->gauges, gaugeId ) );
    BmGauge_createBasic( 
        array_at( self->gauges, gaugeId ),
        dependencies, optionSize );
    
    BmCode_destroy( array_at( self->masks, gaugeId ) );
    BmCode_create_numbers( array_at( self->masks, gaugeId ), varSize, variables );

    deleteBmCode( dependencies );
    return self;
}

BmEval* BmEval_weightAt_set( BmEval* self, uint gaugeId, double weight )
{
    BmVector_at_set( self->weights, gaugeId, weight );
    return self;
}

/* Cleanning */

/* Accessor */
uint BmEval_dimention( BmEval* self )
{
    return BmCode_dimention( self->variables );
}

uint BmEval_gaugeSize( BmEval* self )
{
    return self->gaugeSize;
}

BmGauge* BmEval_gaugeAt( BmEval* self, uint iGauge )
{
    return array_at( self->gauges, iGauge );
}

double BmEval_weightAt( BmEval* self, uint gaugeId)
{
    return BmVector_at( self->weights, gaugeId );
}

/* Process */
double BmEval_valueOf( BmEval* self, BmCode* code )
{
    double value= 0.0;
    for( uint iGauge=  1 ; iGauge <= self->gaugeSize ; ++iGauge )
    {
        BmCode* gaugeCode= BmCode_newBmCodeMask( code, array_at( self->masks, iGauge ) ); 
        value+= BmVector_at( self->weights, iGauge )
                * BmGauge_at( array_at( self->gauges, iGauge ) , gaugeCode );
        deleteBmCode( gaugeCode );
    }
    return value;
}

/* Printing */
