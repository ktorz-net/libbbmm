#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   F U N C T I N A L :  C R I T E R I O N
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor */
BmValueFct* newBmValueFctBasic( uint inputSize, uint ouputSize )
{
    return BmValueFct_createWith(
        newEmpty( BmValueFct ),
        newBmCode_all( inputSize, 2 ),
        newBmVector_all( ouputSize, 0.0 )
    );
}

BmValueFct* newBmValueFctWith( BmCode* newInputRanges, BmVector* newOutputs )
{
    return BmValueFct_createWith(
        newEmpty( BmValueFct ),
        newInputRanges,
        newOutputs
    );
}

BmValueFct* BmValueFct_createWith( BmValueFct* self, BmCode* newInputRanges, BmVector* newOutputs )
{
    assert( BmCode_dimention(newInputRanges) > (uint)0 );
    self->selector= newBmTreeWith( newInputRanges );
    BmTree_newBranch_full( self->selector, 1, 1 );
    self->outputs= newOutputs;
    return self;
}

/* Destructor */
BmValueFct* BmValueFct_destroy( BmValueFct* self )
{
    deleteBmTree( self->selector );
    deleteBmVector( self->outputs );
    return self;
}

void deleteBmValueFct( BmValueFct* instance )
{
    BmValueFct_destroy( instance );
    free( instance );
}

/* re-initializer */
uint BmValueFct_reinitWith( BmValueFct* self, BmCode* newInputRanges, BmVector* newOutputs )
{
    BmValueFct_destroy( self );
    BmValueFct_createWith( self, newInputRanges, newOutputs );
    return 1;
}

/* Accessor */
BmTree*   BmValueFct_selector( BmValueFct* self )
{
    return self->selector;
}

uint BmValueFct_inputDimention( BmValueFct* self )
{
    return BmTree_dimention( self->selector );
}

uint BmValueFct_outputSize( BmValueFct* self )
{
    return BmVector_dimention( self->outputs );
}

BmCode*   BmValueFct_inputRanges( BmValueFct* self )
{
    return BmTree_inputRanges( self->selector );
}

BmVector* BmValueFct_outputs( BmValueFct* self )
{
    return self->outputs;
}

double BmValueFct_from( BmValueFct* self, BmCode* input )
{
    return BmVector_value(
        self->outputs,
        BmTree_at( self->selector, input )
    );
}

/* Construction */
uint BmValueFct_addValue( BmValueFct* self, double ouputValue )
{
    uint newDimention= BmVector_dimention( self->outputs ) + 1;
    BmVector_redimention( self->outputs, newDimention);
    BmVector_at_set( self->outputs, newDimention, ouputValue );
    return newDimention;
}

uint BmValueFct_ouputId_setValue( BmValueFct* self, uint ouputId, double ouputValue )
{
    BmVector_at_set( self->outputs, ouputId, ouputValue );
    return ouputId;
}

uint BmValueFct_from_set( BmValueFct* self, BmCode* input, uint ouputId )
{
    return BmTree_at_set( self->selector, input, ouputId );   
}

/* Instance tools */
void BmValueFct_switch(BmValueFct* self, BmValueFct* doppelganger)
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
BmBench* BmValueFct_asNewBench( BmValueFct* self )
{
    BmBench* bench= BmTree_asNewBench( self->selector );
    uint iOutput= BmCode_dimention( self->selector->inputRanges ) + 1;
    uint benchSize= BmBench_size( bench );
    for( uint i= 1 ; i <= benchSize ; ++i )
    {
        uint outputId= BmCode_digit( BmBench_codeAt( bench, i), iOutput ); 
        BmBench_at_setValue(
            bench,
            i,
            BmVector_value( self->outputs, outputId )
        );
    }
    return bench;
}

/* Printing */
char* BmValueFct_print(BmValueFct* self, char* buffer)
{
    return BmValueFct_printSep( self, buffer, ",\n  ");
}

char* BmValueFct_printSep(BmValueFct* self, char* buffer, char* separator)
{
    strcat( buffer, "Selector:\n" );
    BmTree_print_sep( self->selector, buffer, separator );
    strcat( buffer, "\nOutputs:\n" );
    BmVector_print( self->outputs, buffer );
    return buffer;
}
