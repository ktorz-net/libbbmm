#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Constructor */
BmDecision* newBmDecisionBasic( uint inputSize )
{
    return BmDecision_createWith( newEmpty(BmDecision), newBmCode_all(inputSize, 2), newBmBench(1) );
}

BmDecision* newBmDecisionWith( BmCode* newInputRanges, BmBench* newOutputs )
{
    return BmDecision_createWith( newEmpty(BmDecision), newInputRanges, newOutputs );
}

BmDecision* BmDecision_createWith( BmDecision* self, BmCode* newInputRanges, BmBench* newOutputs )
{
    assert( BmCode_dimention(newInputRanges) > (uint)0 );
    self->selector= newBmTreeWith( newInputRanges );
    BmTree_newBranch_on( self->selector, 1, 1 );
    self->outputs= newOutputs;
    return self;
}

/* Destructor */
BmDecision* BmDecision_destroy( BmDecision* self )
{
    deleteBmTree( self->selector );
    deleteBmBench( self->outputs );
    return self;
}

void deleteBmDecision( BmDecision* instance )
{
    BmDecision_destroy( instance );
    free( instance );
}

/* re-initializer */
BmDecision* BmDecision_reinitWith( BmDecision* self, BmCode* newInputRanges, BmBench* newOutputs )
{
    assert( BmBench_vectorDimention( newOutputs ) == 1 );
    //TODO: assert( BmBench_isHomogene() );
    BmDecision_destroy( self );
    BmDecision_createWith( self, newInputRanges, newOutputs );
    return self;
}

BmDecision* BmDecision_reinitWithDefault( BmDecision* self, BmCode* newInputRanges, BmCode* newDefaultOutput, double defaultValue )
{
    BmBench* ben= newBmBench_codeDim_vectorDim( 16, BmCode_dimention( newDefaultOutput ), 1 );
    BmBench_attachCode_vector( ben, newDefaultOutput, newBmVector_list(1, defaultValue) );
    return BmDecision_reinitWith( self, newInputRanges, ben );
}

/* Accessor */
BmTree*   BmDecision_selector( BmDecision* self )
{
    return self->selector;
}

BmBench* BmDecision_outputs( BmDecision* self )
{
    return self->outputs;
}

uint BmDecision_from( BmDecision* self, BmCode* input )
{
    return BmTree_at( self->selector, input);
}

BmCode* BmDecision_codeFrom( BmDecision* self, BmCode* input )
{
    return BmBench_codeAt( self->outputs, BmTree_at( self->selector, input) );
}

double BmDecision_valueFrom( BmDecision* self, BmCode* input )
{
    return BmBench_valueAt( self->outputs, BmTree_at( self->selector, input) );
}

/* Construction */
uint BmDecision_attachOuput( BmDecision* self, BmCode* newOuputCode, double ouputValue )
{
    return BmBench_attachCode_vector( self->outputs, newOuputCode, newBmVector_list(1, ouputValue) );
}

uint BmDecision_from_set( BmDecision* self, BmCode* input, uint ouputId )
{
    return BmTree_at_set( self->selector, input, ouputId );
}


/* Instance tools */
void BmDecision_switch(BmDecision* self, BmDecision* doppelganger)
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
char* BmDecision_print( BmDecision* self, char* buffer )
{
    return BmDecision_printSep( self, buffer, ",\n  ");
}

char* BmDecision_printSep( BmDecision* self, char* buffer, char* separator )
{
    strcat( buffer, "Selector:\n" );
    BmTree_print_sep( self->selector, buffer, separator );
    strcat( buffer, "\nOutputs:\n" );
    BmBench_print( self->outputs, buffer );
    return buffer;
}

