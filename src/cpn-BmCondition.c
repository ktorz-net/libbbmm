#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Constructor */
BmCondition* newBmConditionBasic(digit range)
{
    return BmCondition_createBasic( newEmpty(BmCondition), range );
}

BmCondition* newBmConditionWith(digit range, BmCode* newInputRanges, BmBench* newDefaultDistrib)
{
    return BmCondition_createWith( newEmpty(BmCondition), range, newInputRanges, newDefaultDistrib );
}

BmCondition* BmCondition_createBasic( BmCondition* self, digit range )
{
    BmBench* distrib = newBmBench(1);
    BmBench_attachCode_vector( distrib, newBmCode_all(1, 1), newBmVector_all(1, 1.0) );
    return BmCondition_createWith( self, range,
        newBmCode_all(1, 1), distrib );
}

BmCondition* BmCondition_createWith( BmCondition* self, digit range, BmCode* newInputRanges, BmBench* newDefaultDistrib )
{
    assert( range > (digit)0 );
    assert( BmCode_dimention(newInputRanges) > (digit)0 );
    assert( BmBench_codeDimention(newDefaultDistrib) == (digit)1 );

    self->range= range;

    self->selector= newBmTreeWith( newInputRanges );
    
    self->distribCapacity= 1;
    self->distributions= newEmptyArray( BmBench*, self->distribCapacity );
    array_at_set( self->distributions, 1, newDefaultDistrib );
    self->distribSize= 1;

    return self;
}

/* Destructor */
BmCondition* BmCondition_destroy(BmCondition* self)
{
    for(digit i = 1 ; i <= self->distribSize ; ++i )
        deleteBmBench( array_at(self->distributions, i) );
    deleteEmptyArray( self->distributions );
    deleteBmTree( self->selector );
    return self;
}

void deleteBmCondition(BmCondition* instance)
{
    BmCondition_destroy( instance );
    free( instance );
}

/* re-initializer */
digit BmCondition_reinitWith( BmCondition* self, digit range, BmCode* newParents, BmBench* newDistrib )
{
    BmCondition_destroy( self );
    BmCondition_createWith( self, range, newParents, newDistrib );
    return 1;
}

digit BmCondition_reinitDistributionsWith( BmCondition* self, BmBench* newDistrib )
{
    digit range= BmCondition_range(self);
    BmCode* newParents= newBmCodeAs( BmCondition_parents(self) );
    return BmCondition_reinitWith( self, range, newParents, newDistrib );
}

/* Accessor */
digit BmCondition_range( BmCondition* self )
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
    digit iDistrib= BmTree_at( self->selector, configuration);
    assert( 0 < iDistrib && iDistrib <= self->distribSize );
    return array_at(self->distributions, iDistrib );
}

BmBench* BmCondition_fromKey( BmCondition* self, digit configKey )
{
    BmCode* config= BmCode_newBmCodeOnKey(
        BmCondition_parents(self),
        configKey );
    BmBench* distrib= BmCondition_from( self, config );
    deleteBmCode(config);
    return distrib;
}

digit BmCondition_distributionSize( BmCondition* self )
{
    return self->distribSize;
}

BmBench* BmCondition_distributionAt( BmCondition* self, digit iDistrib )
{
    return array_at(self->distributions, iDistrib );
}

/* Construction */
digit _BmCondition_resizeDistributionCapacity( BmCondition* self, digit newCapacity )
{
    // Allocate new memory
    BmBench ** newDistrib= newEmptyArray( BmBench*, newCapacity+1 );
    
    digit boundedSize= self->distribSize;
    if ( newCapacity < boundedSize )
        boundedSize= self->distribCapacity;
    
    // Copy
    for( digit i = 0 ; i < boundedSize ; ++i )
        newDistrib[i]= self->distributions[i];

    // Clean the reminders element
    for( digit i = boundedSize ; i < self->distribSize ; ++i )
        free( self->distributions[i] );

    // Update the structure:
    free(self->distributions);
    self->distributions= newDistrib;
    self->distribCapacity= newCapacity;
    self->distribSize= boundedSize;

    return boundedSize;
}

digit BmCondition_attach( BmCondition* self, BmBench* distribution )
{
    assert( BmBench_codeDimention(distribution) == (digit)1 );

    if( self->distribSize+1 > self->distribCapacity )
        _BmCondition_resizeDistributionCapacity( self, self->distribSize+1 );
    
    self->distribSize+= 1;
    array_at_set(self->distributions, self->distribSize, distribution );
    return self->distribSize;
}

digit BmCondition_from_attach( BmCondition* self, BmCode* configuration, BmBench* distribution )
{
    assert( BmCode_dimention( BmCondition_parents(self) ) == BmCode_dimention(configuration) );
    digit iDistrib= BmCondition_attach(self, distribution);
    
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
    digit dim= BmCode_dimention( BmCondition_parents(self) );
    BmCode* mask= newBmCode( dim );

    for( digit i= 1 ; i <= dim ; ++i )
        BmCode_at_set( mask, i, i );
    
    BmBench* resultingDistribution= BmCondition_newDistributionByInfering_mask(self, distribOverConfigurations, mask);
    deleteBmCode( mask );

    return resultingDistribution; 
}

BmBench* BmCondition_newDistributionByInfering_mask( BmCondition* self, BmBench* longDistrib, BmCode* mask )
{
    // Create new structure:
    digit selfDim= BmCode_dimention( BmCondition_parents(self) );
    digit longDim= BmCode_dimention( BmBench_codeAt(longDistrib, 1) );
    BmBench* newDistrib= newBmBench( self->range * BmBench_size(longDistrib) );

    // foreach configuration in the distribution:
    digit numberOfCondition= BmBench_size(longDistrib);
    BmCode* parentConf= newBmCode( selfDim );
    for( digit iCondition= 1 ; iCondition <= numberOfCondition ; ++iCondition )
    {
        BmCode* newConfig= newBmCode_all( longDim+1, 0 );
        BmCode* cond= BmBench_codeAt( longDistrib, iCondition);
        BmCode_copyNumbers( newConfig, cond );
        double probability= BmBench_valueAt( longDistrib, iCondition);

        // get the parents' configuration:
        for( digit j= 1 ; j <= selfDim ; ++j )
            BmCode_at_set(
                parentConf, j,
                BmCode_digit( newConfig, BmCode_digit( mask, j ) )
            );

        // foreach ouput in the conditional distribution resulting from the parent'config:
        BmBench* outputDistrib= BmCondition_from( self, parentConf );
        for( digit iOutput= 1 ; iOutput <= outputDistrib->size ; ++iOutput )
        {
            BmCode_at_set(
                newConfig, BmCode_dimention(newConfig),
                BmCode_digit( BmBench_codeAt( outputDistrib, iOutput), 1 )
            );
            BmBench_attachCode_vector(
                newDistrib,
                newBmCodeAs(newConfig),
                newBmVector_all(1, probability * BmBench_valueAt( outputDistrib, iOutput) )
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
    digit range= BmCondition_range(self);
    BmTree* selector= self->selector;
    digit distribSize= self->distribSize;
    digit distribCapacity= self->distribCapacity;
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
    digit iDistrib= BmCode_digit( code, BmCode_dimention(code) );
    digit inputSize= BmCode_dimention( BmCondition_parents(self) );
    
    // Security:
    assert( BmCode_dimention(code) == inputSize+1 );
    
    char tmp[64];
    strcat(output, "[");

    if( inputSize > 0 )
    {
        sprintf( tmp, "%u", BmCode_digit(code, 1) );
        strcat(output, tmp );
        
        for( digit i= 2 ; i <= inputSize ; ++i)
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
        self, BmBench_codeAt( collec, 1 ), output );

    // All the others: 
    for( digit i = 2 ; i <= BmBench_size(collec) ; ++i )
    {
        strcat( output, separator );
        _BmCondition_printCode_withDistribution(
            self, BmBench_codeAt( collec, i ), output );
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

    if( BmCode_dimention( parentRanges ) >= 1 )
    {
        BmCode* config= BmCode_newBmCodeFirst( parentRanges );
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
