#include "bbmm-probabilities.h"

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
    BmCondition_destroy( instance );
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
    self->selector= newBmTree( parentsRanges,  BmCode_product( self->parentRanges ) );
    
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
    BmCode* parentRanges = newBmCode_all(1, 1);
    BmCondition_createBasic( self, outputSize, parentRanges );
    deleteBmCode( parentRanges );
    return self;
}

BmCondition* BmCondition_destroy(BmCondition* self)
{
    //free(self->name);
    for(uint i = 1 ; i <= self->distribSize ; ++i )
        deleteBmDistribution( array_at(self->distributions, i) );
    deleteBmCode( self->parentRanges );
    deleteBmTree( self->selector );
    return self;
}

/* instance basics */
void BmCondition_switch(BmCondition* self, BmCondition* doppelganger)
{
    // local copy:
    uint outputSize= self->outputSize;
    BmCode* parentRanges= self->parentRanges;
    BmTree* selector= self->selector;
    uint distribSize= self->distribSize;
    uint distribCapacity= self->distribCapacity;
    BmDistribution* * distributions= self->distributions;

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

/* initialize */
void BmCondition_initialize(BmCondition* self, uint outputSize, BmCode* parentRanges, BmDistribution* defaultDistrib)
{
    BmCondition* newSelf= newBmCondition( outputSize, parentRanges, defaultDistrib );
    BmCondition_switch( self, newSelf );
    deleteBmCondition(newSelf);
}

void BmCondition_initializeEquiprobable( BmCondition* self, uint outputSize, BmCode* parentRanges )
{
    BmDistribution* distrib= newBmDistribution(1);
    BmCode* code= newBmCode_all(1, 0);
    double proba= 1.0/outputSize;
    for( uint i = 1 ; i <= outputSize ; ++i  )
    {
        BmCode_at_set(code, 1, i);
        BmDistribution_addConfig(distrib, code, proba );
    }
    BmCondition_initialize( self, outputSize, parentRanges, distrib );
    deleteBmDistribution(distrib);
    deleteBmCode(code);
}

void BmCondition_reinitializeDefaultDistrib(BmCondition* self, BmDistribution* defaultDistrib)
{
    BmCondition* newSelf= newBmCondition( self->outputSize, self->parentRanges, defaultDistrib );
    BmCondition_switch( self, newSelf );
    deleteBmCondition(newSelf);
}

void BmCondition_reinitializeEquiprobable( BmCondition* self )
{
    BmCondition_initializeEquiprobable( self, self->outputSize, self->parentRanges );
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
    uint iDistrib= BmTree_at( self->selector, configuration);
    assert( 0 < iDistrib && iDistrib <= self->distribSize );
    return array_at(self->distributions, iDistrib );
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
    BmCode* mask= newBmCode( dim );

    for( uint i= 1 ; i <= dim ; ++i )
        BmCode_at_set( mask, i, i );
    
    BmDistribution* resultingDistribution= BmCondition_newDistributionByInfering_mask(self, distribOverConfigurations, mask);
    deleteBmCode( mask );

    return resultingDistribution; 
}

BmDistribution * BmCondition_newDistributionByInfering_mask(BmCondition* self, BmDistribution * longDistrib, BmCode * mask)
{
    // puts("< BmCondition_inferMask");
    BmDistribution* newDistrib= newBmDistribution( longDistrib->dimention + 1 );
    BmDistribution_resizeCapacity(newDistrib, self->outputSize * BmDistribution_size(longDistrib) );

    // char buffer[2048];
    // p-rintf( "- longDistribution: %s\n", BmDistribution_str(longDistrib, buffer) );

    // foreach configuration in the distribution:
    uint numberOfCondition= BmDistribution_size(longDistrib);
    uint dim= BmCondition_dimention(self);
    BmCode* parentConf= newBmCode( dim );
    for( uint iCondition= 0 ; iCondition < numberOfCondition ; ++iCondition )
    {
        BmCode* newConfig= newBmCode_all( BmCode_size(longDistrib->configurations[iCondition])+1, 0 );
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
    assert( BmCondition_dimention(self) == BmCode_size(configuration) );
    if( self->distribSize+1 > self->distribCapacity )
        BmCondition_resizeDistributionCapacity( self, self->distribSize+1 );
    
    self->distribSize+= 1;
    array_at_set(self->distributions, self->distribSize, newBmDistributionAs( distribution ) );
    BmTree_at_set(self->selector, configuration, self->distribSize);

    return self->distribSize;
}

uint BmCondition_at_readOrder_set( BmCondition* self, BmCode* configuration, BmCode* configOrder, BmDistribution* distribution )
{
    assert( BmCondition_dimention(self) == BmCode_size(configuration) );
    if( self->distribSize+1 > self->distribCapacity )
        BmCondition_resizeDistributionCapacity( self, self->distribSize+1 );
    
    self->distribSize+= 1;
    array_at_set(self->distributions, self->distribSize, newBmDistributionAs( distribution ) );
    BmTree_at_readOrder_set(self->selector, configuration, configOrder, self->distribSize);
    BmTree_at_set(self->selector, configuration, self->distribSize);

    return self->distribSize;
}

void BmCondition_at_addOutput_onProbability( BmCondition* self, BmCode* configutation, uint option, double probability )
{
    BmDistribution_addOutput( BmCondition_at(self, configutation), option, probability );
}

/* Printing */
char* _BmCondition_printCode_withDistribution(BmCondition* self, BmCode* code, uint iDistrib, char* output)
{
    uint inputSize= BmCondition_dimention(self);
    
    // Security:
    assert( BmCode_size(code) == inputSize );

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
    BmDistribution_print( array_at(self->distributions, iDistrib ), output );

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
        uint iItem= BmBench_attachLast(collec, newBmCode_all( BmCondition_dimention(self), 0));
        BmBench_at_tag(collec, iItem, 1 );
    }

    // First or unique one: 
    _BmCondition_printCode_withDistribution(
        self, BmBench_at( collec, 1 ), BmBench_tagAt( collec, 1 ), output );

    // All the others: 
    for( uint i = 2 ; i <= BmBench_size(collec) ; ++i )
    {
        strcat( output, separator );
        _BmCondition_printCode_withDistribution(
            self, BmBench_at( collec, i ), BmBench_tagAt( collec, i ), output );
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
