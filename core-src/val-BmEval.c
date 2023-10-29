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
BmEval* newBmEvalBasic( uint codeDimention, uint defaultCodeSize )
{
    return BmEval_createBasic( newEmpty(BmEval), codeDimention, defaultCodeSize );
}

BmEval* newBmEvalSpace( BmCode* codeSpace )
{
    return BmEval_createSpace( newEmpty(BmEval), codeSpace );
}

BmEval* newBmEvalStateAction( BmCode* stateSpace, BmCode* actionSpace)
{
    return BmEval_createStateAction( newEmpty(BmEval), stateSpace, actionSpace );
}

BmEval* newBmEvalStateActionState( BmCode* stateSpace, BmCode* actionSpace)
{
    return BmEval_createStateActionState( newEmpty(BmEval), stateSpace, actionSpace );
}

void deleteBmEval( BmEval* self )
{
    BmEval_distroy( self );
    free( self );
}

BmEval* BmEval_createBasic( BmEval* self, uint variableDim, uint defaultVarSize )
{
    self->variable= newBmCode_all( variableDim, defaultVarSize );

    double outputs[1]= {0.0};
    self->criteria= newEmptyArray( BmCriteria*, 1);
    self->masks= newEmptyArray( BmCode*, 1);
    
    array_at_set(self->criteria, 1, newBmCriteria_options( self->variable, 1, outputs) );
    BmCode* mask= newBmCodeBasic( BmCode_size( self->variable ) );
    for( uint i= 1 ; i <= BmCode_size( self->variable ) ; ++i )
        BmCode_at_set( mask, i , i );
    array_at_set(self->masks, 1, mask );

    self->weights= newEmptyArray( double, 1 );
    array_at_set( self->weights, 1, 1.0 );

    self->critNumber= 1;
    return self;
}

BmEval* BmEval_createSpace( BmEval* self, BmCode* codeSpace )
{
    BmEval_createBasic( self, BmCode_size( codeSpace ), 2 );
    for( uint i= 1 ; i <= BmCode_size( codeSpace ) ; ++i )
    {
        BmCode_at_set( self->variable, i, BmCode_at( codeSpace, i ) );
    }
    return self;
}

BmEval* BmEval_createStateAction( BmEval* self, BmCode* stateSpace, BmCode* actionSpace)
{
    uint stateDimention= BmCode_size(stateSpace);
    uint actionDimention= BmCode_size(actionSpace);
    self->variable= newBmCodeBasic( stateDimention+actionDimention );
    for( uint i= 1 ; i <= stateDimention ; ++i )
    {
        BmCode_at_set( self->variable, i, BmCode_at( stateSpace, i ) );
    }

    for( uint i= 1 ; i <= actionDimention ; ++i )
    {
        BmCode_at_set( self->variable, stateDimention+i, BmCode_at( actionSpace, i ) );
    }

    double outputs[1]= {0.0};
    self->criteria= newEmptyArray( BmCriteria*, 1);
    self->masks= newEmptyArray( BmCode*, 1);
    
    array_at_set(self->criteria, 1, newBmCriteria_options( self->variable, 1, outputs) );
    BmCode* mask= newBmCodeBasic( BmCode_size( self->variable ) );
    for( uint i= 1 ; i <= BmCode_size( self->variable ) ; ++i )
        BmCode_at_set( mask, i , i );
    array_at_set(self->masks, 1, mask );

    self->weights= newEmptyArray( double, 1 );
    array_at_set( self->weights, 1, 1.0 );

    self->critNumber= 1;
    return self;
}

BmEval* BmEval_createStateActionState( BmEval* self, BmCode* stateSpace, BmCode* actionSpace)
{
    uint stateDimention= BmCode_size(stateSpace);
    uint actionDimention= BmCode_size(actionSpace);
    BmEval_createBasic( self, stateDimention*2+actionDimention, 2);

    self->variable= newBmCodeBasic( BmCode_size( self->variable )  );
    for( uint i= 1 ; i <= stateDimention ; ++i )
    {
        BmCode_at_set( self->variable, i, BmCode_at( stateSpace, i ) );
        BmCode_at_set(
            self->variable, stateDimention+actionDimention+i,
            BmCode_at( stateSpace, i )
        );
    }

    for( uint i= 1 ; i <= actionDimention ; ++i )
    {
        BmCode_at_set( self->variable, stateDimention+i, BmCode_at( actionSpace, i ) );
    }
    return self;
}

BmEval* BmEval_distroy( BmEval* self)
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