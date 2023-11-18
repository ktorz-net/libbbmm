#include "bbmm-structures.h"
#include "bbmm-models.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Constructor */
BmCondition* newBmConditionBasic(uint outputSize)
{
    return BmCondition_createBasic( newEmpty(BmCondition), outputSize );
}

BmCondition* newBmConditionWith(uint domainSize, BmCode* newParentRanges, BmBench* newDefaultDistrib)
{
    return BmCondition_createWith( newEmpty(BmCondition), domainSize, newParentRanges, newDefaultDistrib );
}

BmCondition* BmCondition_createBasic( BmCondition* self, uint outputSize )
{
    BmBench* distrib = newBmBench( 1 );
    BmBench_attachLast( distrib, newBmCode_list(1, 1), 0, 1.0 );
    return BmCondition_createWith( self, outputSize,
        newBmCode_all(1, 1), distrib );
}

BmCondition* BmCondition_createWith( BmCondition* self, uint domainSize, BmCode* newParentRanges, BmBench* newDefaultDistrib )
{
    assert( domainSize > (uint)0 );
    assert( BmCode_dimention(newParentRanges) > (uint)0 );

    self->outputSize= domainSize;

    self->parentRanges= newParentRanges;
    self->selector= newBmTreeWith( newParentRanges,  BmCode_product( self->parentRanges ) );
    
    self->distribCapacity= 1;
    self->distributions= newEmptyArray( BmBench*, self->distribCapacity );
    array_at_set( self->distributions, 1, newDefaultDistrib );
    self->distribSize= 1;

    return self;
}

/* Destructor */
BmCondition* BmCondition_destroy(BmCondition* self)
{
    //free(self->name);
    for(uint i = 1 ; i <= self->distribSize ; ++i )
        deleteBmBench( array_at(self->distributions, i) );
    deleteEmptyArray( self->distributions );
    deleteBmCode( self->parentRanges );
    deleteBmTree( self->selector );
    return self;
}

void deleteBmCondition(BmCondition* instance)
{
    BmCondition_destroy( instance );
    free( instance );
}



/* Accessor */
BmCode* BmCondition_parents( BmCondition* self )
{
    return self->parentRanges;
}

BmBench* BmCondition_at( BmCondition* self, BmCode* configuration )
{
    uint iDistrib= BmTree_at( self->selector, configuration);
    assert( 0 < iDistrib && iDistrib <= self->distribSize );
    return array_at(self->distributions, iDistrib );
}

BmBench* BmCondition_atKey( BmCondition* self, uint configKey )
{
    BmCode* config= BmCode_newBmCodeOnKey( self->parentRanges, configKey );
    BmBench* distrib= BmCondition_at( self, config );
    deleteBmCode(config);
    return distrib;
}