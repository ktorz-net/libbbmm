#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Constructor */
BmCondition* newBmConditionBasic(uint range)
{
    return BmCondition_createBasic( newEmpty(BmCondition), range );
}

BmCondition* newBmConditionWith(uint range, BmCode* newInputRanges, BmBench* newDefaultDistrib)
{
    return BmCondition_createWith( newEmpty(BmCondition), range, newInputRanges, newDefaultDistrib );
}

BmCondition* BmCondition_createBasic( BmCondition* self, uint range )
{
    BmBench* distrib = newBmBench( 1 );
    BmBench_attachLast( distrib, newBmCode_list(1, 1), 1.0 );
    return BmCondition_createWith( self, range,
        newBmCode_all(1, 1), distrib );
}

BmCondition* BmCondition_createWith( BmCondition* self, uint range, BmCode* newInputRanges, BmBench* newDefaultDistrib )
{
    assert( range > (uint)0 );
    assert( BmCodeDimention(newInputRanges) > (uint)0 );
    assert( BmBenchDimention(newDefaultDistrib) == (uint)1 );

    self->range= range;

    self->selector= newBmTreeWith( newInputRanges );
    
    self->distribCapacity= 1;
    self->distributions= newEmptyArray( BmBench*, self->distribCapacity );
    array_at_set( self->distributions, 1, newDefaultDistrib );
    self->distribSize= 1;

    return self;
}

/* Destructor */
BmCondition* BmConditiondestroy(BmCondition* self)
{
    for(uint i = 1 ; i <= self->distribSize ; ++i )
        deleteBmBench( array_at(self->distributions, i) );
    deleteEmptyArray( self->distributions );
    deleteBmTree( self->selector );
    return self;
}

void deleteBmCondition(BmCondition* instance)
{
    BmConditiondestroy( instance );
    free( instance );
}

/* re-initializer */
uint BmCondition_reinitWith( BmCondition* self, uint range, BmCode* newParents, BmBench* newDistrib )
{
    BmConditiondestroy( self );
    BmCondition_createWith( self, range, newParents, newDistrib );
    return 1;
}

uint BmCondition_reinitDistributionsWith( BmCondition* self, BmBench* newDistrib )
{
    uint range= BmCondition_range(self);
    BmCode* newParents= newBmCodeAs( BmCondition_parents(self) );
    return BmCondition_reinitWith( self, range, newParents, newDistrib );
}

/* Accessor */
uint BmCondition_range( BmCondition* self )
{
    return self->range;
}

BmTree* BmCondition_selector( BmCondition* self )
{
    return self->selector;
}

BmCode* BmCondition_parents( BmCondition* self )
{
    return self->selector->inputRanges;
}

BmBench* BmCondition_from( BmCondition* self, BmCode* configuration )
{
    uint iDistrib= BmTree_at( self->selector, configuration);
    assert( 0 < iDistrib && iDistrib <= self->distribSize );
    return array_at(self->distributions, iDistrib );
}

BmBench* BmCondition_fromKey( BmCondition* self, uint configKey )
{
    BmCode* config= BmCode_newBmCodeOnKey(
        BmCondition_parents(self),
        configKey );
    BmBench* distrib= BmCondition_from( self, config );
    deleteBmCode(config);
    return distrib;
}

uint BmCondition_distributionSize( BmCondition* self )
{
    return self->distribSize;
}

BmBench* BmCondition_distributionAt( BmCondition* self, uint iDistrib )
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

uint BmCondition_attach( BmCondition* self, BmBench* distribution )
{
    assert( BmBenchDimention(distribution) == (uint)1 );

    if( self->distribSize+1 > self->distribCapacity )
        _BmCondition_resizeDistributionCapacity( self, self->distribSize+1 );
    
    self->distribSize+= 1;
    array_at_set(self->distributions, self->distribSize, distribution );
    return self->distribSize;
}

uint BmCondition_from_attach( BmCondition* self, BmCode* configuration, BmBench* distribution )
{
    assert( BmCodeDimention( BmCondition_parents(self) ) == BmCodeDimention(configuration) );
    uint iDistrib= BmCondition_attach(self, distribution);
    
    BmTree_at_set(self->selector, configuration, iDistrib);

    return iDistrib;
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
    uint dim= BmCodeDimention( BmCondition_parents(self) );
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
    uint selfDim= BmCodeDimention( BmCondition_parents(self) );
    uint longDim= BmCodeDimention( BmBench_at(longDistrib, 1) );
    BmBench* newDistrib= newBmBench( self->range * BmBench_size(longDistrib) );

    // foreach configuration in the distribution:
    uint numberOfCondition= BmBench_size(longDistrib);
    BmCode* parentConf= newBmCode( selfDim );
    for( uint iCondition= 1 ; iCondition <= numberOfCondition ; ++iCondition )
    {
        BmCode* newConfig= newBmCode_all( longDim+1, 0 );
        BmCode* cond= BmBench_at( longDistrib, iCondition);
        BmCode_copyNumbers( newConfig, cond );
        double probability= BmBench_valueAt( longDistrib, iCondition);

        // get the parents' configuration:
        for( uint j= 1 ; j <= selfDim ; ++j )
            BmCode_at_set(
                parentConf, j,
                BmCode_digit( newConfig, BmCode_digit( mask, j ) )
            );

        // foreach ouput in the conditional distribution resulting from the parent'config:
        BmBench* outputDistrib= BmCondition_from( self, parentConf );
        for( uint iOutput= 1 ; iOutput <= outputDistrib->size ; ++iOutput )
        {
            BmCode_at_set(
                newConfig, BmCodeDimention(newConfig),
                BmCode_digit( BmBench_at( outputDistrib, iOutput), 1 )
            );
            BmBench_attachLast(
                newDistrib, newBmCodeAs(newConfig),
                probability * BmBench_valueAt( outputDistrib, iOutput)
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
    uint range= BmCondition_range(self);
    BmTree* selector= self->selector;
    uint distribSize= self->distribSize;
    uint distribCapacity= self->distribCapacity;
    BmBench* * distributions= self->distributions;

    // self as doppelganger:
    self->range= doppelganger->range;
    self->selector= doppelganger->selector;
    self->distribSize= doppelganger->distribSize;
    self->distribCapacity= doppelganger->distribCapacity;
    self->distributions= doppelganger->distributions;

    // doppelganger as self:
    doppelganger->range= range;
    doppelganger->selector= selector;
    doppelganger->distribSize= distribSize;
    doppelganger->distribCapacity= distribCapacity;
    doppelganger->distributions= distributions;
}

/* Printing */
char* _BmCondition_printCode_withDistribution(BmCondition* self, BmCode* code, char* output)
{
    uint iDistrib= BmCode_digit( code, BmCodeDimention(code) );
    uint inputSize= BmCodeDimention( BmCondition_parents(self) );
    
    // Security:
    assert( BmCodeDimention(code) == inputSize+1 );
    
    char tmp[64];
    strcat(output, "[");

    if( inputSize > 0 )
    {
        sprintf( tmp, "%u", BmCode_digit(code, 1) );
        strcat(output, tmp );
        
        for( uint i= 2 ; i <= inputSize ; ++i)
        {
            sprintf( tmp, "%u", BmCode_digit(code, i) );
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
        strcat(output, "}");
        return output;
    }

    // First or unique one: 
    _BmCondition_printCode_withDistribution(
        self, BmBench_at( collec, 1 ), output );

    // All the others: 
    for( uint i = 2 ; i <= BmBench_size(collec) ; ++i )
    {
        strcat( output, separator );
        _BmCondition_printCode_withDistribution(
            self, BmBench_at( collec, i ), output );
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
    BmCode* parentRanges= BmCondition_parents(self);

    if( BmCodeDimention( parentRanges ) >= 1 )
    {
        BmCode* config= BmCodeNewBmCodeFirst( parentRanges );
        BmCode_print( config, output );
        strcat(output, ": ");
        BmBench_print( BmCondition_from(self, config), output );
        BmCode_nextCode( parentRanges, config );

        while( BmCode_isIncluding( parentRanges, config ) )
        {
            strcat(output, separator);
            BmCode_print( config, output );
            strcat(output, ": ");
            BmBench_print( BmCondition_from(self, config), output );
            BmCode_nextCode( parentRanges, config );
        }

        deleteBmCode(config);
    }

    strcat(output, "}");
    return output;
}

char* BmCondition_printIdentity(BmCondition* self, char* output)
{
    char buffer[1024];
    BmCode_print( BmCondition_parents(self), output );
    sprintf( buffer, "->[%d]", self->range );
    strcat( output, buffer );
    return output;
}
