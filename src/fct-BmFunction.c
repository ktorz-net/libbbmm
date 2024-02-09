#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Constructor */
BmFunction* newBmFunctionBasic( uint inputSize )
{
    return BmFunction_createWith( newEmpty(BmFunction), newBmCode_all(inputSize, 2), newBmBench(1) );
}

BmFunction* newBmFunctionWith( BmCode* newInputRanges, BmBench* newOutputs )
{
    return BmFunction_createWith( newEmpty(BmFunction), newInputRanges, newOutputs );
}

BmFunction* BmFunction_createWith( BmFunction* self, BmCode* newInputRanges, BmBench* newOutputs )
{
    assert( BmCode_dimention(newInputRanges) > (uint)0 );
    self->selector= newBmTreeWith( newInputRanges );
    BmTree_newBranch( self->selector, 1, 1 );
    self->outputs= newOutputs;
    return self;
}

/* Destructor */
BmFunction* BmFunction_destroy( BmFunction* self )
{
    deleteBmTree( self->selector );
    deleteBmBench( self->outputs );
    return self;
}

void deleteBmFunction( BmFunction* instance )
{
    BmFunction_destroy( instance );
    free( instance );
}

/* re-initializer */
uint BmFunction_reinitWith( BmFunction* self, BmCode* newInputRanges, BmBench* newOutputs )
{
    BmFunction_destroy( self );
    BmFunction_createWith( self, newInputRanges, newOutputs );
    return 1;
}

/* Accessor */
BmTree*   BmFunction_selector( BmFunction* self )
{
    return self->selector;
}

BmBench* BmFunction_outputs( BmFunction* self )
{
    return self->outputs;
}

uint BmFunction_from( BmFunction* self, BmCode* input )
{
    return BmTree_at( self->selector, input);
}

BmCode* BmFunction_codeFrom( BmFunction* self, BmCode* input )
{
    return BmBench_at( self->outputs, BmTree_at( self->selector, input) );
}

double BmFunction_valueFrom( BmFunction* self, BmCode* input )
{
    return BmBench_valueAt( self->outputs, BmTree_at( self->selector, input) );
}
