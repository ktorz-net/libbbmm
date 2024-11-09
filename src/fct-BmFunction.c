#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Constructor */
BmFunction* newBmFunctionBasic( digit inputSize )
{
    return BmFunction_createWith( newEmpty(BmFunction), newBmCode_all(inputSize, 2), newBmBench(1) );
}

BmFunction* newBmFunctionWith( BmCode* newInputRanges, BmBench* newOutputs )
{
    return BmFunction_createWith( newEmpty(BmFunction), newInputRanges, newOutputs );
}

BmFunction* BmFunction_createWith( BmFunction* self, BmCode* newInputRanges, BmBench* newOutputs )
{
    assert( BmCode_dimention(newInputRanges) > (digit)0 );
    self->selector= newBmTreeWith( newInputRanges );
    BmTree_newBranch_full( self->selector, 1, 1 );
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
BmFunction* BmFunction_reinitWith( BmFunction* self, BmCode* newInputRanges, BmBench* newOutputs )
{
    assert( BmBench_vectorDimention( newOutputs ) == 1 );
    //TODO: assert( BmBench_isHomogene() );
    BmFunction_destroy( self );
    BmFunction_createWith( self, newInputRanges, newOutputs );
    return self;
}

BmFunction* BmFunction_reinitWithDefault( BmFunction* self, BmCode* newInputRanges, BmCode* newDefaultOutput, double defaultValue )
{
    BmBench* ben= newBmBench_codeDim_vectorDim( 16, BmCode_dimention( newDefaultOutput ), 1 );
    BmBench_attachCode_vector( ben, newDefaultOutput, newBmVector_all(1, defaultValue) );
    return BmFunction_reinitWith( self, newInputRanges, ben );
}

/* Accessor */
BmTree*   BmFunction_selector( BmFunction* self )
{
    return self->selector;
}

digit BmFunction_inputDimention( BmFunction* self )
{
    return BmTree_dimention( self->selector );
}

BmCode* BmFunction_inputRanges( BmFunction* self )
{
    return BmTree_inputRanges( self->selector );
}

digit BmFunction_outputSize( BmFunction* self )
{
    return BmBench_size( self->outputs );
}

BmBench* BmFunction_outputs( BmFunction* self )
{
    return self->outputs;
}

digit BmFunction_from( BmFunction* self, BmCode* input )
{
    return BmTree_at( self->selector, input);
}

BmCode* BmFunction_codeFrom( BmFunction* self, BmCode* input )
{
    return BmBench_codeAt( self->outputs, BmTree_at( self->selector, input) );
}

double BmFunction_valueFrom( BmFunction* self, BmCode* input )
{
    return BmBench_valueAt( self->outputs, BmTree_at( self->selector, input) );
}

/* Construction */
digit BmFunction_attachOuput( BmFunction* self, BmCode* newOuputCode, double ouputValue )
{
    return BmBench_attachCode_vector( self->outputs, newOuputCode, newBmVector_all(1, ouputValue) );
}

digit BmFunction_from_set( BmFunction* self, BmCode* input, digit ouputId )
{
    return BmTree_at_set( self->selector, input, ouputId );
}


/* Instance tools */
void BmFunction_switch(BmFunction* self, BmFunction* doppelganger)
{
    // local copy:
    BmTree* selector= self->selector;
    BmBench* outputs= self->outputs;

    // self as doppelganger:
    self->selector= doppelganger->selector;
    self->outputs= doppelganger->outputs;

    // doppelganger as self:
    doppelganger->selector= selector;
    doppelganger->outputs= outputs;
}

/* Printing */
char* BmFunction_print( BmFunction* self, char* buffer )
{
    return BmFunction_printSep( self, buffer, ",\n  ");
}

char* BmFunction_printSep( BmFunction* self, char* buffer, char* separator )
{
    strcat( buffer, "Selector:\n" );
    BmTree_print_sep( self->selector, buffer, separator );
    strcat( buffer, "\nOutputs:\n" );
    BmBench_print( self->outputs, buffer );
    return buffer;
}

