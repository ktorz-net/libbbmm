#include "bemage-pbb.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Constructor Destructor */
BmCondition* newBmCondition(uint outputSize, BmCode *parentRanges, BmDistribution* defaultDistrib)
{
    BmCondition* instance= malloc( sizeof(BmCondition) );
    BmCondition_create(instance, outputSize, parentRanges, defaultDistrib);
    return instance;
}

BmCondition* newBmConditionBasic(uint outputSize, BmCode *parentRanges)
{
    BmCondition* instance= malloc( sizeof(BmCondition) );
    BmCondition_createBasic(instance, outputSize, parentRanges);
    return instance;
}

BmCondition* newBmConditionUndependant(uint outputSize)
{
    BmCondition* instance= malloc( sizeof(BmCondition) );
    BmCondition_createUndependant(instance, outputSize);
    return instance;
}

void deleteBmCondition(BmCondition* instance)
{
    BmCondition_distroy( instance );
    free( instance );
}

/* Protected - to use with precaution */
BmCondition* BmCondition_create( BmCondition* self, uint outputSize, BmCode * parentsRanges, BmDistribution* defaultDistrib )
{
    assert( outputSize > 0 );
    assert( BmCode_size(parentsRanges) > 0 );
    
    //self->name= malloc( sizeof(char) * strlen(name)+1 );
    //strcpy(self->name, name);

    self->outputSize= outputSize;

    self->parentRanges= newBmCodeAs( parentsRanges );
    self->selector= newBmTreeInputSizeAndBound( 
        BmCode_size(parentsRanges), BmCode_numbers(parentsRanges),
        BmCode_product( self->parentRanges ) );
    
    self->distribCapacity= 1;
    self->distributions= malloc( sizeof(BmDistribution*) * self->distribCapacity );
    array_at_set( self->distributions, 1, newBmDistributionAs( defaultDistrib ) );
    self->distribSize= 1;

    return self;
}

BmCondition* BmCondition_createBasic(BmCondition* self, uint outputSize, BmCode* parentsRanges)
{
    BmDistribution* defaultOutput= newBmDistribution(1);
    BmDistribution_addOutput( defaultOutput, 1, 1.0 );
    BmCondition_create(self, outputSize, parentsRanges, defaultOutput);
    deleteBmDistribution( defaultOutput );
    return self;
}

BmCondition* BmCondition_createUndependant(BmCondition* self, uint outputSize)
{
    BmCode* parentRanges = newBmCode(1, 1);
    BmCondition_createBasic( self, outputSize, parentRanges );
    deleteBmCode( parentRanges );
    return self;
}

BmCondition* BmCondition_distroy(BmCondition* self)
{
    //free(self->name);
    for(uint i = 1 ; i <= self->distribSize ; ++i )
        deleteBmDistribution( array_at(self->distributions, i) );
    deleteBmCode( self->parentRanges );
    deleteBmTree( self->selector );
    return self;
}

/* initialize */
void BmCondition_reinitialize(BmCondition* self, BmCode* parentRanges, BmDistribution* defaultDistrib)
{
    uint domSize= self->outputSize;
    BmCode *copyParents= newBmCodeAs(parentRanges);
    BmDistribution *copyDistrib= newBmDistributionAs(defaultDistrib);

    BmCondition_distroy( self );
    BmCondition_create( self, domSize, copyParents, copyDistrib );
    deleteBmCode( parentRanges );
}


void BmCondition_reinitializeEquiprobable(BmCondition* self, BmCode* parentRanges)
{
    BmDistribution* distrib= newBmDistribution(1);
    BmCode* code= newBmCode(1);
    double proba= 1.0/self->outputSize;
    for( uint i = 1 ; i <= self->outputSize ; ++i  )
    {
        BmCode_at_set(code, 1, i);
        BmDistribution_addConfig(distrib, code, proba );
    }
    BmCondition_reinitialize( self, parentRanges, distrib );
    deleteBmDistribution(distrib);
    deleteBmCode(code);
}


/* Accessor */
uint BmCondition_outputSize( BmCondition* self )
{
    return self->outputSize;
}

BmCode* BmCondition_parentRanges( BmCondition* self )
{
    return self->parentRanges;
}

uint BmCondition_parentSize( BmCondition* self )
{
    return BmCode_product(self->parentRanges);
}

uint BmCondition_dimention( BmCondition* self )
{
    return BmCode_size(self->parentRanges);
}

BmDistribution* BmCondition_at( BmCondition* self, BmCode* configuration )
{
    return array_at(self->distributions, BmTree_at( self->selector, configuration) );
}

uint BmCondition_distributionIndexAt( BmCondition* self, BmCode* configuration )
{
    return BmTree_at( self->selector, configuration);
}

BmDistribution * BmCondition_atKey( BmCondition* self, uint configKey )
{
    BmCode* config= BmCode_newBmCodeOnKey( self->parentRanges, configKey );
    BmDistribution* distrib= BmCondition_at( self, config );
    deleteBmCode(config);
    return distrib;
}


BmDistribution* BmCondition_infer( BmCondition* self, BmDistribution* distribOverConfigurations )
{
    BmDistribution* newDistrib= BmCondition_newDistributionByInfering(self, distribOverConfigurations);
    BmDistribution_switch(distribOverConfigurations, newDistrib);
    deleteBmDistribution(newDistrib);
    return distribOverConfigurations;
}

BmDistribution* BmCondition_newDistributionByInfering(BmCondition* self, BmDistribution * distribOverConfigurations)
{
    assert( BmCondition_dimention(self) == distribOverConfigurations->dimention );
    uint dim= distribOverConfigurations->dimention;
    BmCode* mask= newBmCodeBasic( dim, 0 );

    for( uint i= 1 ; i <= dim ; ++i )
        BmCode_at_set( mask, i, i );
    
    BmDistribution* resultingDistribution= BmCondition_newDistributionByInfering_mask(self, distribOverConfigurations, mask);
    deleteBmCode( mask );

    return resultingDistribution; 
}

BmDistribution * BmCondition_newDistributionByInfering_mask(BmCondition* self, BmDistribution * longDistrib, BmCode * mask)
{
    // p uts("< BmCondition_inferMask");
    BmDistribution* newDistrib= newBmDistribution( longDistrib->dimention + 1 );
    BmDistribution_resizeCapacity(newDistrib, self->outputSize * BmDistribution_size(longDistrib) );

    // char buffer[2048];
    // p-rintf( "- longDistribution: %s\n", BmDistribution_str(longDistrib, buffer) );

    // foreach configuration in the distribution:
    uint numberOfCondition= BmDistribution_size(longDistrib);
    uint dim= BmCondition_dimention(self);
    BmCode* parentConf= newBmCodeBasic( dim, 0 );
    for( uint iCondition= 0 ; iCondition < numberOfCondition ; ++iCondition )
    {
        BmCode* newConfig= newBmCodeBasic( BmCode_size(longDistrib->configurations[iCondition])+1, 0 );
        BmCode_copyNumbers( newConfig, longDistrib->configurations[iCondition] );
        double probability= longDistrib->probabilities[iCondition];

        // p rintf( "- condition+: %s (%f)\n", BmCode_str(newConfig, buffer), probability );

        // get the parents' configuration:
        for( uint j= 1 ; j <= dim ; ++j )
            BmCode_at_set(
                parentConf, j,
                BmCode_at( newConfig, BmCode_at(mask, j) )
            );

        // p rintf( "- parents: %s\n", BmCode_str(parentConf, buffer) );
        // p rintf( "- output distribution: %s\n", BmDistribution_str( BmCondition_at( self, parentConf ), buffer) );

        // foreach ouput in the conditional distribution resulting from the parent'config:
        BmDistribution* outputDistrib= BmCondition_at( self, parentConf );
        for( uint iOutput= 0 ; iOutput < outputDistrib->size ; ++iOutput )
        {
            BmCode_at_set(
                newConfig, BmCode_size(newConfig),
                BmCode_at( outputDistrib->configurations[iOutput], 1 )
            );
            BmDistribution_addConfig(
                newDistrib, newConfig,
                probability * outputDistrib->probabilities[iOutput]
            );
        }
        //p rintf( "+ newDistribution: %s\n", BmDistribution_str(newDistrib, buffer) );
    }

    // p uts(">");
    return newDistrib;
}

/* Test */

/* Construction */
uint BmCondition_resizeDistributionCapacity( BmCondition* self, uint newCapacity )
{
    // Allocate new memory
    BmDistribution ** newDistrib= malloc( sizeof(BmDistribution*) * newCapacity+1 );

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

uint BmCondition_at_set( BmCondition* self, BmCode* configuration, BmDistribution* distribution )
{
    // Copy the distribution:
    if( self->distribSize+1 > self->distribCapacity )
        BmCondition_resizeDistributionCapacity( self, self->distribSize+1 );
    
    self->distribSize+= 1;
    array_at_set(self->distributions, self->distribSize, newBmDistributionAs( distribution ) );

    // Record it in the selector:
    BmTree_at_set(self->selector, configuration, self->distribSize);

    return self->distribSize;
}

void BmCondition_at_addOutput_onProbability( BmCondition* self, BmCode* configutation, uint option, double probability )
{
    BmDistribution_addOutput( BmCondition_at(self, configutation), option, probability );
}

/* Printing */
char* BmCondition_printCode(BmCondition* self, BmCode* code, char* output)
{
    uint inputSize= BmCondition_dimention(self);
    // Securitée:
    assert( BmCode_size(code) == inputSize+1 );

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
    BmDistribution_print( array_at(self->distributions, BmCode_at(code, inputSize+1) ), output );
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

    if( BmBench_size(collec) > 0 )
    {
        BmCondition_printCode( self, BmBench_at_item( collec, 1 ), output );
    }    
    for( uint i = 2 ; i <= BmBench_size(collec) ; ++i )
    {
        strcat( output, separator );
        BmCondition_printCode( self, BmBench_at_item( collec, i ), output );
    }
    strcat(output, "}");
    return output;
}

char* BmCondition_wordingExtend(BmCondition* self)
{
    char* output= bm_lineBuffer();
    strcpy(output, "");
    return BmCondition_printExtendSep(self,  output, ",\n");
}

char* BmCondition_printExtend(BmCondition* self, char* output)
{
    return BmCondition_printExtendSep(self,  output, ",\n");
}
char* BmCondition_strSep(BmCondition* self, char* output, char* separator)
{
    strcpy(output, "");
    return BmCondition_printExtendSep(self,  output, separator);
}

char* BmCondition_printExtendSep(BmCondition* self, char* output, char* separator)
{
    BmCondition_printIdentity(self, output);
    strcat(output, ": {");

    if( BmCode_size(self->parentRanges) >= 1 )
    {
        BmCode* config= BmCode_newBmCodeFirst( self->parentRanges );
        
        BmCode_print( config, output );
        strcat(output, ": ");
        BmDistribution_print( BmCondition_at(self, config), output );
        BmCode_nextCode( self->parentRanges, config );

        while( BmCode_isIncluding( self->parentRanges, config ) )
        {
            strcat(output, separator);
            BmCode_print( config, output );
            strcat(output, ": ");
            BmDistribution_print( BmCondition_at(self, config), output );
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
