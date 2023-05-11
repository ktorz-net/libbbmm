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


/* Constructor Destructor */
BmValueFct* newBmValueFctBasic( BmCode* stateSpace, BmCode* actionSpace)
{
    return BmValueFct_createBasic( newEmpty(BmValueFct), stateSpace, actionSpace );
}

void deleteBmValueFct( BmValueFct* self )
{
    BmValueFct_distroy( self );
    free( self );
}

BmValueFct* BmValueFct_createBasic( BmValueFct* self, BmCode* stateSpace, BmCode* actionSpace)
{
    self->stateDimention= BmCode_size(stateSpace);
    self->actionDimention= BmCode_size(actionSpace);
    self->variable= newBmCodeBasic( self->stateDimention+self->actionDimention  );
    for( uint i= 1 ; i <= self->stateDimention ; ++i )
    {
        BmCode_at_set( self->variable, i, BmCode_at( stateSpace, i ) );
    }

    for( uint i= 1 ; i <= self->actionDimention ; ++i )
    {
        BmCode_at_set( self->variable, self->stateDimention+i, BmCode_at( actionSpace, i ) );
    }

    double outputs[1]= {0.0};
    self->criteria= newEmptyArray( BmCriteria*, 1);
    self->masks= newEmptyArray( BmCode*, 1);
    
    array_at_set(self->criteria, 1, newBmCriteria_options( self->variable, 1, outputs) );
    BmCode* mask= newBmCodeBasic( self->stateDimention+self->actionDimention );
    for( uint i= 1 ; i <= self->stateDimention+self->actionDimention ; ++i )
        BmCode_at_set( mask, i , i );
    array_at_set(self->masks, 1, mask );

    self->weights= newEmptyArray( double, 1 );
    array_at_set( self->weights, 1, 1.0 );

    self->critNumber= 1;
    return self;
}

BmValueFct* BmValueFct_distroy( BmValueFct* self)
{
    for( uint i = 1 ; i < self->critNumber ; ++i )
    {
        deleteBmCriteria( array_at( self->criteria, i ) );
        deleteBmCode( array_at( self->masks, i ) );
    }

    deleteEmptyArray( self->criteria );
    deleteEmptyArray( self->masks );
    deleteEmptyArray( self->weights );

    deleteBmCode( self->variable );
    return self;
}

/* initialize */

/* Construction */


/* Cleanning */

/* Accessor */

/* Printing */