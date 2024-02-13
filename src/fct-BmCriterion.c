#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   F U N C T I N A L :  C R I T E R I O N
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor */
BmCriterion* newBmCriterionBasic( uint inputSize, uint ouputSize )
{
    return BmCriterion_createWith(
        newEmpty( BmCriterion ),
        newBmCode_all( inputSize, 2 ),
        newBmVector_all( ouputSize, 0.0 )
    );
}

BmCriterion* newBmCriterionWith( BmCode* newInputRanges, BmVector* newOutputs )
{
    return BmCriterion_createWith(
        newEmpty( BmCriterion ),
        newInputRanges,
        newOutputs
    );
}

BmCriterion* BmCriterion_createWith( BmCriterion* self, BmCode* newInputRanges, BmVector* newOutputs )
{
    assert( BmCode_dimention(newInputRanges) > (uint)0 );
    self->selector= newBmTreeWith( newInputRanges );
    BmTree_newBranch( self->selector, 1, 1 );
    self->outputs= newOutputs;
    return self;
}

/* Destructor */
BmCriterion* BmCriterion_destroy( BmCriterion* self )
{
    deleteBmTree( self->selector );
    deleteBmVector( self->outputs );
    return self;
}

void deleteBmCriterion( BmCriterion* instance )
{
    BmCriterion_destroy( instance );
    free( instance );
}

/* re-initializer */
uint BmCriterion_reinitWith( BmCriterion* self, BmCode* newInputRanges, BmVector* newOutputs )
{
    BmCriterion_destroy( self );
    BmCriterion_createWith( self, newInputRanges, newOutputs );
    return 1;
}

/* Accessor */
BmTree*   BmCriterion_selector( BmCriterion* self )
{
    return self->selector;
}

BmCode*   BmCriterion_inputRanges( BmCriterion* self )
{
    return BmTree_inputRanges( self->selector );
}

BmVector* BmCriterion_outputs( BmCriterion* self )
{
    return self->outputs;
}

double BmCriterion_from( BmCriterion* self, BmCode* input )
{
    return BmVector_at(
        self->outputs,
        BmTree_at( self->selector, input )
    );
}

/* Construction */
uint BmCriterion_ouputId_setValue( BmCriterion* self, uint ouputId, double ouputValue )
{
    return BmVector_at_set(
        self->outputs,
        ouputId,
        ouputValue
    );
    return ouputId;
}

uint BmCriterion_from_set( BmCriterion* self, BmCode* input, uint ouputId )
{
    return BmTree_at_set( self->selector, input, ouputId );   
}

/* Instance tools */
void BmCriterion_switch(BmCriterion* self, BmCriterion* doppelganger)
{
    // local copy:
    BmTree* selector= self->selector;
    BmVector* outputs= self->outputs;

    // self as doppelganger:
    self->selector= doppelganger->selector;
    self->outputs= doppelganger->outputs;

    // doppelganger as self:
    doppelganger->selector= selector;
    doppelganger->outputs= outputs;
}

/* Generating */
BmBench* BmCriterion_asNewBench( BmCriterion* self )
{
    BmBench* bench= BmTree_asNewBench( self->selector );
    uint iOutput= BmCode_dimention( self->selector->inputRanges ) + 1;
    uint benchSize= BmBench_size( bench );
    for( uint i= 1 ; i <= benchSize ; ++i )
    {
        uint outputId= BmCode_at( BmBench_at( bench, i), iOutput ); 
        BmBench_at_setValue(
            bench,
            i,
            BmVector_at( self->outputs, outputId )
        );
    }
    return bench;
}

/* Printing */
char* BmCriterion_print(BmCriterion* self, char* buffer)
{
    return BmCriterion_printSep( self, buffer, ",\n  ");
}

char* BmCriterion_printSep(BmCriterion* self, char* buffer, char* separator)
{
    strcat( buffer, "Selector:\n" );
    BmTree_print_sep( self->selector, buffer, separator );
    strcat( buffer, "\nOutputs:\n" );
    BmVector_print( self->outputs, buffer );
    return buffer;
}
