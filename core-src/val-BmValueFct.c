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

    //critNumber;
    //BmCriteria ** criteria;
    //BmCode ** masks;
    //double * factors;
    return self;
}
BmValueFct* BmValueFct_distroy( BmValueFct* self)
{
    deleteBmCode( self->variable );
    return self;
}

/* initialize */

/* Construction */


/* Cleanning */

/* Accessor */

/* Printing */