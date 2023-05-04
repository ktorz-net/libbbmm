#include "bbmm-pbb.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Constructor Destructor */
BmDistribution * newBmDistribution(uint dimention)
{
    BmDistribution* self= newEmpty(BmDistribution);
    BmDistribution_create(self, dimention);
    return self;
}

BmDistribution * newBmDistributionAs( BmDistribution * model )
{
    BmDistribution* self= newEmpty(BmDistribution);
    BmDistribution_createAs( self, model );
    return self;
}

void deleteBmDistribution(BmDistribution *self)
{
    BmDistribution_distroy(self);
    free(self);
}

/* Protected - to use with precaution */
BmDistribution * BmDistribution_create(BmDistribution* self, uint dimention)
{
    self->capacity= 1;
    self->configurations= malloc( sizeof(BmCode*) * self->capacity );
    self->probabilities= malloc( sizeof(double) * self->capacity );

    for(uint i=0; i < self->capacity; ++i)
    {
        self->configurations[i]= NULL;
        self->probabilities[i]= 0.0;
    }

    self->size= 0;
    self->dimention= dimention;

    return self;
}

BmDistribution * BmDistribution_createAs(BmDistribution* self, BmDistribution* model)
{
    BmDistribution_create(self, model->dimention);

    BmDistribution_resizeCapacity( self, model->size );
    for( uint i = 0 ; i < model->size ; ++i )
        BmDistribution_addConfig(self, model->configurations[i], model->probabilities[i] );

    assert( self->size == model->size );
    return self;
}

BmDistribution* BmDistribution_distroy(BmDistribution *self)
{
    for( uint i = 0 ; i < self->size ; ++i )
        deleteBmCode( self->configurations[i] );
    free(self->configurations);
    free(self->probabilities);
    return self; 
}

/* Switching double beffering */
void BmDistribution_switch(BmDistribution *self, BmDistribution *another)
{
    BmDistribution tmp;
    tmp= (*self);
    (*self)= (*another);
    (*another)= tmp;
}
/* tnp    
  uint capacity, size;
  uint dimention;
  uint ** states;
  double * probabilities;
}*/

/* Getter */
uint BmDistribution_dimention(BmDistribution *self)
{
    return self->dimention;
}

uint BmDistribution_size(BmDistribution *self)
{
    return self->size;
}

BmCode* BmDistribution_configAt(BmDistribution *self, uint i )
{
    return (self->configurations[i]);
}

uint BmDistribution_outputAt(BmDistribution *self, uint i )
{
    return BmCode_at( self->configurations[i], 1);
}

double BmDistribution_probabilityAt(BmDistribution *self, uint i )
{
    return self->probabilities[i];
}

/* Test */

/* Modification */
void BmDistribution_initialize(BmDistribution *self, uint dimention)
{
    BmDistribution_distroy( self );
    BmDistribution_create(self, dimention);
}

void BmDistribution_initializeAs( BmDistribution *self, BmDistribution * model )
{
    BmDistribution_distroy( self );
    BmDistribution_createAs( self, model );
}

void BmDistribution_resizeCapacity(BmDistribution *self, uint newCapacity)
{
    // Allocate new memory
    BmCode** newStIndexes= malloc( sizeof(BmCode*) * newCapacity );
    double * newProbabilities= malloc( sizeof(double) * newCapacity );

    uint boundedSize= self->size;
    if ( newCapacity < boundedSize )
        boundedSize= self->capacity;
    
    // Copy
    for( uint i = 0 ; i < boundedSize ; ++i )
    {
        newStIndexes[i]= self->configurations[i];
        newProbabilities[i]= self->probabilities[i];
    }

    // Clean the reminders element
    for( uint i = boundedSize ; i < self->size ; ++i )
    {
        deleteBmCode( self->configurations[i] );
    }

    // Update the structure:
    free(self->configurations);
    free(self->probabilities);
    self->configurations= newStIndexes;
    self->probabilities= newProbabilities;
    self->capacity= newCapacity;
    self->size= boundedSize;
}

void BmDistribution_clear(BmDistribution *self)
{
    while( self->size > 0 )
    {
        self->size-= 1;
        deleteBmCode( self->configurations[self->size] );
    }
}

/* Construction - configuration (Multi-dimentional element) */

uint BmDistribution_addConfig(BmDistribution *self, BmCode* configuration, double probability )
{
    assert( BmCode_size(configuration) == BmDistribution_dimention(self) );

    if( self->capacity < self->size+1 )
        BmDistribution_resizeCapacity( self, self->capacity*2 );

    self->configurations[self->size]= newBmCodeAs( configuration );
    self->probabilities[self->size]= probability;

    self->size+= 1;

    return (self->size-1);
}

uint BmDistribution_setOnConfig( BmDistribution *self, BmCode* configuration )
{
    BmDistribution_clear(self);
    BmDistribution_resizeCapacity(self, 1);
    return BmDistribution_addConfig(self, configuration, 1.0);
}

void BmDistribution_setConfigs(BmDistribution *self, uint size, BmCode** configurations, double * probas )
{
    BmDistribution_clear(self);
    BmDistribution_resizeCapacity(self, size);

    for( uint i= 0 ; i < size ; ++i )
    {
        BmDistribution_addConfig( self, configurations[i], probas[i] );
    }
}

/* Construction - output (uni-dimentional element) */

uint BmDistribution_addOutput(BmDistribution *self, uint output, double probability)
{
    BmCode* config= newBmCode_all( 1, output );
    uint r= BmDistribution_addConfig(self, config, probability);
    deleteBmCode( config );
    return r;
}

uint BmDistribution_setOnOutput(BmDistribution *self, uint output)
{
    BmCode* config= newBmCode_all(1, output );
    uint r= BmDistribution_setOnConfig(self, config);
    deleteBmCode( config );
    return r;
}

void BmDistribution_switchPosibilities(BmDistribution *self, uint index1, uint index2)
{
    BmCode* tmpSt= self->configurations[index1];
    double tmpPb= self->probabilities[index1];
    self->configurations[index1]= self->configurations[index2];
    self->probabilities[index1]= self->probabilities[index2];
    self->configurations[index2]= tmpSt;
    self->probabilities[index2]= tmpPb;
}

/* Process */                
void BmDistribution_sort( BmDistribution *self )
{

    BmCode *stI, *stLast;
    
    bool go= 1;
    while( go )
    {
        go= 0;
        for(uint i= 1 ; i < self->size ; ++i )
        {
            stI= self->configurations[i];
            stLast= self->configurations[i-1];
            if( BmCode_isGreaterThan( stLast, stI ) )
            {
                go= 1;
                BmDistribution_switchPosibilities( self, i-1, i);
            }
        }
    }
}

uint BmDistribution_mergeDoubles( BmDistribution *self )
{
    uint count= 0;

    BmCode *stI, *stLast;
     
    for( uint i= 1 ; i < self->size ; ++i )
    {
        uint last= i-1-count;
        
        stI= self->configurations[i];
        stLast= self->configurations[last];

        if( BmCode_isEqualTo(stI, stLast) )
        {
            self->probabilities[last]+= self->probabilities[i];
            count+= 1;
        }
        else if( count )
        {
            BmDistribution_switchPosibilities(self, last+1, i);
        }
    }
    self->size = self->size - count;

    return count;
}

void BmDistribution_reduceDimentionTo(BmDistribution *self, uint newDimention, uint * mask )
{
    BmDistribution *newOne= newBmDistribution( newDimention );
    BmDistribution_resizeCapacity( newOne, self->size );
    BmDistribution_asReductionOf( newOne, self, mask );
    BmDistribution_switch(self, newOne);
    deleteBmDistribution(newOne);
}

/* Morphing */
void BmDistribution_asReductionOf( BmDistribution *self, BmDistribution *model, uint * mask )
{
    BmCode* state= newBmCode_all( self->dimention, 0 );
    for( uint iPosibility = 0; iPosibility < model->size ; ++iPosibility  )
    {
        for( uint i= 0 ; i < self->dimention ; ++i )
            BmCode_at_set( state, i+1, BmCode_at(model->configurations[iPosibility], mask[i]) );
        BmDistribution_addConfig( self, state, model->probabilities[iPosibility] );
    }
    BmDistribution_sort( self );
    BmDistribution_mergeDoubles( self );
    deleteBmCode( state );
}

/* Printing */

void BmDistribution_puts(BmDistribution* self)
{
    char out[1024]= "";
    puts( BmDistribution_print(self, out) );
}

char* BmDistribution_wording(BmDistribution* self)
{
    char* output= bm_lineBuffer();
    strcpy(output, "");
    return BmDistribution_print(self, output);
}

char* BmDistribution_print(BmDistribution* self, char* out)
{
    strcat(out, "{");

    if( self->size > 0 )
        BmDistribution_printPosibility(self, 0, out);

    for( uint i= 1 ; i < self->size ; ++i)
    {
        strcat(out, " ; ");
        BmDistribution_printPosibility(self, i, out);
    }
    strcat(out, "}");
    return out;
}

char* BmDistribution_strPosibility(BmDistribution* self, uint index, char* out)
{
    strcpy(out, "");
    return BmDistribution_printPosibility(self, index, out);
}

char* BmDistribution_printPosibility(BmDistribution* self, uint index, char* out)
{
    char buffer[32];
    strcat(out, "[");
    if( self->dimention > 0 )
    {
        sprintf( buffer, "%u", BmCode_at(self->configurations[index], 1));
        strcat(out, buffer);
    }
    for( uint i= 2 ; i <= self->dimention ; ++i)
    {
        strcat(out, ", ");
        sprintf( buffer, "%u", BmCode_at(self->configurations[index], i) );
        strcat(out, buffer);
    }
    strcat(out, "]: ");
    bm_printDouble( self->probabilities[index], out );
    return out;
}
