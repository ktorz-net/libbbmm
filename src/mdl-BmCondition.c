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
    BmBench_attachLast( distrib, newBmCode_list(1, 1), 1.0 );
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

/* re-initializer */
uint BmCondition_reinitWith( BmCondition* self, uint outputSize, BmCode* newParents, BmBench* newDistrib )
{
    BmCondition_destroy( self );
    BmCondition_createWith( self, outputSize, newParents, newDistrib );
    return 1;
}

uint BmCondition_reinitDistributionsWith( BmCondition* self, BmBench* newDistrib )
{
    uint outputSize= self->outputSize;
    BmCode* newParents= newBmCodeAs( self->parentRanges );
    return BmCondition_reinitWith( self, outputSize, newParents, newDistrib );
}

/* Accessor */
uint BmCondition_output( BmCondition* self )
{
    return self->outputSize;
}

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
BmBench* BmCondition_distribution( BmCondition* self, uint iDistrib )
{
    return array_at(self->distributions, iDistrib );
}

/* Construction */
uint _BmCondition_resizeDistributionCapacity( BmCondition* self, uint newCapacity )
{
    // Allocate new memory
    BmBench ** newDistrib= newEmptyArray( BmBench*, newCapacity+1 );
    
    uint boundedSize= self->distribSize;
    if ( newCapacity < boundedSize )
        boundedSize= self->distribCapacity;
    
    // Copy
    for( uint i = 0 ; i < boundedSize ; ++i )
        newDistrib[i]= self->distributions[i];

    // Clean the reminders element
    for( uint i = boundedSize ; i < self->distribSize ; ++i )
        free( self->distributions[i] );

    // Update the structure:
    free(self->distributions);
    self->distributions= newDistrib;
    self->distribCapacity= newCapacity;
    self->distribSize= boundedSize;

    return boundedSize;
}

uint BmCondition_at_attach( BmCondition* self, BmCode* configuration, BmBench* distribution )
{
    assert( BmCode_dimention(self->parentRanges) == BmCode_dimention(configuration) );
    if( self->distribSize+1 > self->distribCapacity )
        _BmCondition_resizeDistributionCapacity( self, self->distribSize+1 );
    
    self->distribSize+= 1;
    array_at_set(self->distributions, self->distribSize, distribution );
    BmTree_at_set(self->selector, configuration, self->distribSize);

    return self->distribSize;
}

BmBench* BmCondition_infer( BmCondition* self, BmBench* distribOverConfigurations )
{
    BmBench* newDistrib= BmCondition_newDistributionByInfering(self, distribOverConfigurations);
    BmBench_switch(distribOverConfigurations, newDistrib);
    deleteBmBench(newDistrib);
    return distribOverConfigurations;
}

BmBench* BmCondition_newDistributionByInfering( BmCondition* self, BmBench* distribOverConfigurations )
{
    uint dim= BmCode_dimention(self->parentRanges);
    BmCode* mask= newBmCode( dim );

    for( uint i= 1 ; i <= dim ; ++i )
        BmCode_at_set( mask, i, i );
    
    BmBench* resultingDistribution= BmCondition_newDistributionByInfering_mask(self, distribOverConfigurations, mask);
    deleteBmCode( mask );

    return resultingDistribution; 
}

BmBench* BmCondition_newDistributionByInfering_mask( BmCondition* self, BmBench* longDistrib, BmCode* mask )
{
    // Create new structure:
    uint selfDim= BmCode_dimention(self->parentRanges);
    uint longDim= BmCode_dimention( BmBench_at_code(longDistrib, 1) );
    BmBench* newDistrib= newBmBench( self->outputSize * BmBench_size(longDistrib) );

    // foreach configuration in the distribution:
    uint numberOfCondition= BmBench_size(longDistrib);
    BmCode* parentConf= newBmCode( selfDim );
    for( uint iCondition= 1 ; iCondition <= numberOfCondition ; ++iCondition )
    {
        BmCode* newConfig= newBmCode_all( longDim+1, 0 );
        BmCode* cond= BmBench_at_code( longDistrib, iCondition);
        BmCode_copyNumbers( newConfig, cond );
        double probability= BmBench_at_value( longDistrib, iCondition);

        // get the parents' configuration:
        for( uint j= 1 ; j <= selfDim ; ++j )
            BmCode_at_set(
                parentConf, j,
                BmCode_at( newConfig, BmCode_at( mask, j ) )
            );

        // foreach ouput in the conditional distribution resulting from the parent'config:
        BmBench* outputDistrib= BmCondition_at( self, parentConf );
        for( uint iOutput= 1 ; iOutput <= outputDistrib->size ; ++iOutput )
        {
            BmCode_at_set(
                newConfig, BmCode_dimention(newConfig),
                BmCode_at( BmBench_at_code( outputDistrib, iOutput), 1 )
            );
            BmBench_attachLast(
                newDistrib, newBmCodeAs(newConfig),
                probability * BmBench_at_value( outputDistrib, iOutput)
            );
        }
        deleteBmCode( newConfig );
    }

    return newDistrib;
}

/* Instance tools */
void BmCondition_switch(BmCondition* self, BmCondition* doppelganger)
{
    // local copy:
    uint outputSize= self->outputSize;
    BmCode* parentRanges= self->parentRanges;
    BmTree* selector= self->selector;
    uint distribSize= self->distribSize;
    uint distribCapacity= self->distribCapacity;
    BmBench* * distributions= self->distributions;

    // self as doppelganger:
    self->outputSize= doppelganger->outputSize;
    self->parentRanges= doppelganger->parentRanges;
    self->selector= doppelganger->selector;
    self->distribSize= doppelganger->distribSize;
    self->distribCapacity= doppelganger->distribCapacity;
    self->distributions= doppelganger->distributions;

    // doppelganger as self:
    doppelganger->outputSize= outputSize;
    doppelganger->parentRanges= parentRanges;
    doppelganger->selector= selector;
    doppelganger->distribSize= distribSize;
    doppelganger->distribCapacity= distribCapacity;
    doppelganger->distributions= distributions;
}

/* Printing */
char* _BmCondition_printCode_withDistribution(BmCondition* self, BmCode* code, char* output)
{
    uint iDistrib= BmCode_at( code, BmCode_dimention(code) );
    uint inputSize= BmCode_dimention( self->parentRanges );
    
    // Security:
    assert( BmCode_dimention(code) == inputSize+1 );
    
    char tmp[64];
    strcat(output, "[");

    if( inputSize > 0 )
    {
        sprintf( tmp, "%u", BmCode_at(code, 1) );
        strcat(output, tmp );
        
        for( uint i= 2 ; i <= inputSize ; ++i)
        {
            sprintf( tmp, "%u", BmCode_at(code, i) );
            strcat(output, ", ");
            strcat(output, tmp );
        }
    }

    strcat(output, "]: " );
    BmBench_print( array_at(self->distributions, iDistrib ), output );

    return output;
}

char* BmCondition_print(BmCondition* self, char* output)
{
    return BmCondition_printSep( self, output, ",\n  ");
}

char* BmCondition_printSep(BmCondition* self, char* output, char* separator)
{
    BmCondition_printIdentity(self, output);
    strcat(output, ": {");

    BmBench* collec = BmTree_asNewBench( self->selector );

    if( BmBench_size(collec) == 0 )
    {
        BmBench_attach( collec, newBmCode_all( BmCode_dimention( self->parentRanges ), 0) );
    }

    // First or unique one: 
    _BmCondition_printCode_withDistribution(
        self, BmBench_at_code( collec, 1 ), output );

    // All the others: 
    for( uint i = 2 ; i <= BmBench_size(collec) ; ++i )
    {
        strcat( output, separator );
        _BmCondition_printCode_withDistribution(
            self, BmBench_at_code( collec, i ), output );
    }
    strcat(output, "}");
    return output;
}

char* BmCondition_printExtend(BmCondition* self, char* output)
{
    return BmCondition_printExtendSep(self,  output, ",\n");
}

char* BmCondition_printExtendSep(BmCondition* self, char* output, char* separator)
{
    BmCondition_printIdentity(self, output);
    strcat(output, ": {");

    if( BmCode_dimention(self->parentRanges) >= 1 )
    {
        BmCode* config= BmCode_newBmCodeFirst( self->parentRanges );
        BmCode_print( config, output );
        strcat(output, ": ");
        BmBench_print( BmCondition_at(self, config), output );
        BmCode_nextCode( self->parentRanges, config );

        while( BmCode_isIncluding( self->parentRanges, config ) )
        {
            strcat(output, separator);
            BmCode_print( config, output );
            strcat(output, ": ");
            BmBench_print( BmCondition_at(self, config), output );
            BmCode_nextCode( self->parentRanges, config );
        }

        deleteBmCode(config);
    }

    strcat(output, "}");
    return output;
}

char* BmCondition_printIdentity(BmCondition* self, char* output)
{
    char buffer[1024];
    BmCode_print( self->parentRanges, output );
    sprintf( buffer, "->[%d]", self->outputSize );
    strcat( output, buffer );
    return output;
}
