#include "bbmm-probabilities.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   T R A N S I T I O N
 *
 * Define a Bayesian Network composed of state, action and tramsitional nodes 
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */


/* Constructor Destructor */
BmTransition* newBmTransition( BmCode* state, BmCode* action )
{
    BmCode* shift= newBmCode(0);
    BmTransition* trans= BmTransition_create( newEmpty(BmTransition), state, action, shift );
    deleteBmCode(shift);
    return trans;
}

BmTransition* newBmTransitionWithShift( BmCode* state, BmCode* action, BmCode* shift )
{
    return BmTransition_create( newEmpty(BmTransition), state, action, shift );
}

void deleteBmTransition(BmTransition * self)
{
    BmTransition_destroy(self);
    free(self);
}

/* fill and empty a structure */
BmTransition* BmTransition_create(BmTransition* self, BmCode* state, BmCode* action, BmCode* shift )
{
    self->stateDimention= BmCode_size(state);
    self->actionDimention= BmCode_size(action);
    uint shiftDimention= BmCode_size(shift);
    self->overallDimention= self->stateDimention*2 + self->actionDimention + shiftDimention;

    self->network= newBmBench( self->overallDimention );
    self->nodes= newEmptyArray( BmCondition, self->overallDimention );

    uint o= 1;
    for( uint i = 1 ; i <= self->stateDimention ; ++i )
    {
        BmCondition_createUndependant( array_on(self->nodes, o), BmCode_at(state, i) );
        BmBench_attachLast( self->network, newBmCode( 0 ) );
        ++o;
    }
    for( uint i = 1 ; i <= self->actionDimention ; ++i )
    {
        BmCondition_createUndependant( array_on(self->nodes, o), BmCode_at(action, i) );
        BmBench_attachLast( self->network, newBmCode( 0 ) );
        ++o;
    }
    for( uint i = 1 ; i  <= shiftDimention ; ++i )
    {
        BmCondition_createUndependant( array_on(self->nodes, o), BmCode_at(shift, i) );
        BmBench_attachLast( self->network, newBmCode( 0 ) );
        ++o;
    }
    for( uint i = 1 ; i <= self->stateDimention ; ++i )
    {
        BmCondition_createUndependant( array_on(self->nodes, o), BmCode_at(state, i) );
        BmBench_attachLast( self->network, newBmCode( 0 ) );
        ++o;
    }

    self->transition= newBmDistribution( self->stateDimention );
    BmDistribution_resizeCapacity(self->transition, self->overallDimention );
    return self;
}

BmTransition* BmTransition_destroy(BmTransition* self)
{
    for(uint i = 1 ; i <= self->overallDimention ; ++i )
        BmCondition_destroy( array_on(self->nodes, i) );
    free( self->nodes );
    deleteBmBench(self->network);
    deleteBmDistribution( self->transition );
    return self;
}

/* Accessor */
BmDistribution* BmTransition_distribution( BmTransition * self )
{
    return self->transition;
}

uint BmTransition_stateDimention( BmTransition * self )
{
    return self->stateDimention;
}

uint BmTransition_actionDimention( BmTransition * self )
{
    return self->actionDimention;
}

uint BmTransition_shiftDimention( BmTransition * self )
{
    return self->overallDimention - (self->stateDimention*2 + self->actionDimention);
}

uint BmTransition_overallDimention( BmTransition * self )
{
    return self->overallDimention;
}

uint BmTransition_indexOfStateVariableT0( BmTransition * self, uint iVar )
{
    assert(iVar <= self->stateDimention );
    return iVar;
}

uint BmTransition_indexOfStateVariableT1( BmTransition * self, uint iVar )
{
    return self->overallDimention - self->stateDimention +  iVar;
}

uint BmTransition_indexOfActionVariable( BmTransition * self, uint iVar )
{
    return self->stateDimention + iVar;
}

uint BmTransition_indexOfShiftVariable( BmTransition * self, uint iVar )
{
    return self->stateDimention + self->actionDimention + iVar;
}

uint BmTransition_sizeAt( BmTransition * self, uint iVar )
{
    return ((BmCondition*)array_on( self->nodes, iVar ))->outputSize;
}


BmCondition* BmTransition_nodeAt( BmTransition * self, uint iVar )
{
    return (BmCondition*)array_on( self->nodes, iVar );
}

BmCode* BmTransition_dependanciesAt( BmTransition * self, uint iVar )
{
    return BmBench_at(self->network, iVar);
}

/* Construction */
BmTransition* BmTransition_node_initialize( BmTransition* self, uint index, uint outputSize )
{
    BmCondition_destroy( array_on(self->nodes, index) );
    BmCondition_createUndependant( array_on(self->nodes, index), outputSize );
    return self;
}

BmTransition* BmTransition_node_dependArray( BmTransition* self, uint index, uint parentSize, uint* parents )
{
    BmCode* dependancy= newBmCode_numbers(parentSize, parents);
    BmCode_sort( dependancy );
    BmCode_copy( BmBench_at( self->network, index), dependancy );

    BmCode* parentRanges= newBmCode( BmCode_size(dependancy) );
    for( uint i= 1 ; i <= BmCode_size(dependancy) ; ++i )
    {
        BmCondition* parentNode= array_on(self->nodes, BmCode_at(dependancy, i) );
        BmCode_at_set( parentRanges, i, parentNode->outputSize );
    }

    BmDistribution* distrib= newBmDistribution( 1 );
    BmCondition* cond= array_on(self->nodes, index);
    BmCondition_initializeEquiprobable( cond, cond->outputSize , parentRanges );
    deleteBmDistribution(distrib);
    deleteBmCode(dependancy);
    return self;
}

BmTransition* BmTransition_node_depends( BmTransition * self, uint index, uint parentSize, ... )
{
    uint parents[parentSize];
    // Build words array from args
    va_list ap;
    va_start(ap, parentSize); 
    for ( uint i = 0 ; i < parentSize ; ++i )
    {
        parents[i]= va_arg(ap, uint);
    }
    va_end(ap);
    // Create the instance
    return BmTransition_node_dependArray( self, index, parentSize, parents );
}

/* Infering */

BmDistribution* BmTransition_setFomOverallDistribution(BmTransition * self, BmDistribution* overallDistrib)
{
    BmDistribution_clear( self->transition );
    // Generate the mask of state' varaibles (i.e the last ones):
    uint statePrimeMask[ self->stateDimention ];
    uint firstStatePrimeVariable= self->overallDimention - self->stateDimention;
    for(uint i= 0 ; i < self->stateDimention ; ++i)
        statePrimeMask[i]= firstStatePrimeVariable+i;
    
    // generate transition by reducing the overall distribution:
    BmDistribution_asReductionOf( self->transition, overallDistrib, statePrimeMask );
    return self->transition;
}

BmDistribution* BmTransition_newDistributionByInfering( BmTransition * self, BmDistribution* configDistribution )
{
    BmDistribution* constructionDistrib= newBmDistributionAs( configDistribution );
    for( uint i= configDistribution->dimention + 1 ; i <= self->overallDimention ; ++i )
    {
        // Infers dependency possibilities:
        BmDistribution* newDistrib= BmCondition_newDistributionByInfering_mask(
            BmTransition_nodeAt(self, i),
            constructionDistrib,
            BmTransition_dependanciesAt(self, i) );
        deleteBmDistribution(constructionDistrib);
        constructionDistrib= newDistrib;
    }
    
    BmTransition_setFomOverallDistribution(self, constructionDistrib);
    return constructionDistrib;
}

BmDistribution* BmTransition_inferFromState_andAction( BmTransition * self, BmCode* state, BmCode* action )
{
    // Set initial configuration :
    BmCode* startConf= newBmCode( BmCode_size(state) +  BmCode_size(action) );

    for( uint i=1 ; i <= BmCode_size(state) ; ++i )
        BmCode_at_set( startConf, i, BmCode_at(state, i) );
    for( uint i=1 ; i <= BmCode_size(action) ; ++i )
        BmCode_at_set( startConf,  BmCode_size(state)+i, BmCode_at(action, i) );

    // Set a initial determinist distribution :
    BmDistribution * distrib= newBmDistribution( BmCode_size(startConf) );
    BmDistribution_addConfig(distrib, startConf, 1.0);

    // infer :
    deleteBmDistribution( BmTransition_newDistributionByInfering(self, distrib) );
    
    // Clean :
    deleteBmDistribution(distrib);
    deleteBmCode( startConf );

    return self->transition;
}

/* Printing */
char* BmTransition_print(BmTransition* self, char* output)
{
    BmTransition_printSignature(self, output);
    strcat(output, " ");
    BmTransition_printDependency(self, output);
    return output;
}

char* BmTransition_printSignature(BmTransition* self, char* output)
{
    char buffer[64];

    //stateDimention, actionDimention, overallDimention

    // State:
    strcat(output, "[");
    if( self->stateDimention > 0 )
    {
        sprintf(buffer, "%u", BmTransition_nodeAt(self, 1)->outputSize );
        strcat(output, buffer);
    }
    for( uint i= 2 ; i <= self->stateDimention ; ++i)
    {
        sprintf(buffer, ", %u", BmTransition_nodeAt(self, i)->outputSize );
        strcat(output, buffer);
    }
    strcat(output, "]x[");

    // Action:
    uint stateActionDim= self->stateDimention + self->actionDimention;
    if( self->actionDimention > 0 )
    {
        sprintf(buffer, "%u", BmTransition_nodeAt(self, self->stateDimention+1)->outputSize );
        strcat(output, buffer);
    }
    for( uint i= self->stateDimention+2 ; i <= stateActionDim ; ++i)
    {
        sprintf(buffer, ", %u", BmTransition_nodeAt(self, i)->outputSize );
        strcat(output, buffer);
    }
    strcat(output, "](");

    // Shift:
    uint shiftDimention= BmTransition_shiftDimention(self);
    uint end= stateActionDim+shiftDimention;
    if( shiftDimention > 0 )
    {
        sprintf(buffer, "%u", BmTransition_nodeAt(self, stateActionDim+1)->outputSize );
        strcat(output, buffer);
    }
    for( uint i= stateActionDim+2 ; i <= end ; ++i)
    {
        sprintf(buffer, ", %u", BmTransition_nodeAt(self, i)->outputSize );
        strcat(output, buffer);
    }
    strcat(output, ")");

    return output;
}

char* BmTransition_printDependency(BmTransition* self, char* output)
{
    uint shiftStart= self->stateDimention + self->actionDimention + 1;
    uint shiftDimention= BmTransition_shiftDimention(self);
    uint shiftEnd= shiftStart+shiftDimention;

    // Shift:
    strcat(output, "|");
    if( shiftDimention > 0 )
    {
        strcat(output, " ");
        BmCode_print( BmBench_at(self->network, shiftStart), output );
    }
    for( uint i= shiftStart+1 ; i < shiftEnd ; ++i)
    {
        strcat(output, ", ");
        BmCode_print( BmBench_at(self->network, i), output );
    }
    strcat(output, " |");

    // future (state):
    if( self->stateDimention > 0 )
    {
        strcat(output, " ");
        BmCode_print( BmBench_at(self->network, shiftEnd), output );
    }
    for( uint i= shiftEnd+1 ; i <= self->overallDimention ; ++i)
    {
        strcat(output, ", ");
        BmCode_print( BmBench_at(self->network, i), output );
    }
    strcat(output, " |");

    return output;
}
