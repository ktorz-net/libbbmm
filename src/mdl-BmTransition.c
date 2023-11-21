#include "bbmm-models.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   M O D E L  :  T R A N S I T I O N                           *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor*/
BmTransition* newBmTransition( BmCode* state, BmCode* action )
{
    BmCode* shift= newBmCode(0);
    BmTransition* trans= BmTransition_create( newEmpty(BmTransition), state, action, shift );
    deleteBmCode(shift);
    return trans;
}

BmTransition* newBmTransitionShift( BmCode* state, BmCode* action, BmCode* shift )
{
    return BmTransition_create( newEmpty(BmTransition), state, action, shift );
}

BmTransition* BmTransition_create(BmTransition* self, BmCode* state, BmCode* action, BmCode* shift )
{
    self->stateDimention= BmCode_dimention(state);
    self->actionDimention= BmCode_dimention(action);
    uint shiftDimention= BmCode_dimention(shift);
    self->overallDimention= self->stateDimention*2 + self->actionDimention + shiftDimention;

    self->network= newBmBench( self->overallDimention );
    self->nodes= newEmptyArray( BmCondition, self->overallDimention );

    uint o= 1;
    for( uint i = 1 ; i <= self->stateDimention ; ++i )
    {
        BmCondition_createBasic( array_on(self->nodes, o), BmCode_at(state, i) );
        BmBench_attach( self->network, newBmCode( 0 ) );
        ++o;
    }
    for( uint i = 1 ; i <= self->actionDimention ; ++i )
    {
        BmCondition_createBasic( array_on(self->nodes, o), BmCode_at(action, i) );
        BmBench_attach( self->network, newBmCode( 0 ) );
        ++o;
    }
    for( uint i = 1 ; i  <= shiftDimention ; ++i )
    {
        BmCondition_createBasic( array_on(self->nodes, o), BmCode_at(shift, i) );
        BmBench_attach( self->network, newBmCode( 0 ) );
        ++o;
    }
    for( uint i = 1 ; i <= self->stateDimention ; ++i )
    {
        BmCondition_createBasic( array_on(self->nodes, o), BmCode_at(state, i) );
        BmBench_attach( self->network, newBmCode( 0 ) );
        ++o;
    }

    self->transition= newBmBench( self->overallDimention );
    return self;
}

/* Destructor */
BmTransition* BmTransition_destroy(BmTransition* self)
{
    for(uint i = 1 ; i <= self->overallDimention ; ++i )
        BmCondition_destroy( array_on(self->nodes, i) );
    free( self->nodes );
    deleteBmBench(self->network);
    deleteBmBench( self->transition );
    return self;
}

void deleteBmTransition(BmTransition* self)
{
    BmTransition_destroy(self);
    free(self);
}


/* Accessor */
BmBench* BmTransition_distribution( BmTransition * self )
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
BmCondition* BmTransition_node_reinitWith( BmTransition* self, uint index, BmCode* newDependenceMask, BmBench* newDistrib )
{
    // Reccord parent mask: dependency
    BmCode_switch( BmBench_at( self->network, index ), newDependenceMask );
    deleteBmCode( newDependenceMask );
    BmCode* dependency= BmBench_at( self->network, index );
    
    // Build dependance space:
    BmCode* depSpace= newBmCode( BmCode_dimention(dependency) );
    for( uint i= 1 ; i <= BmCode_dimention(dependency) ; ++i )
    {
        BmCode_at_set( depSpace, i,
            BmTransition_sizeAt(self, BmCode_at(dependency, i)) );
    }

    // Re-initialize the condition
    BmCondition* condition= array_on(self->nodes, index);
    BmCondition_reinitWith( condition, condition->outputSize, depSpace, newDistrib );
    return condition;
}

/* Infering */
BmBench* BmTransition_setFomOverallDistribution(BmTransition * self, BmBench* overallDistrib)
{
    // Generate the mask of state' varaibles (i.e the last ones):
    BmCode* statePrimeMask= newBmCode( self->stateDimention );
    uint firstStatePrimeVariable= self->overallDimention - self->stateDimention-1;
    for( uint i= 1 ; i <= self->stateDimention ; ++i )
    {
        BmCode_at_set( statePrimeMask, i, firstStatePrimeVariable+i );
    }

    // generate transition by reducing the overall distribution:
    BmBench* buildTransition = newBmBench( self->transition->capacity );
    BmBench_add_reducted( buildTransition, overallDistrib, statePrimeMask );
    BmBench_sort( buildTransition, (fctptr_BmBench_compare)BmBench_isGreater );

    // merge consecutive identic output:
    uint size= BmBench_size( buildTransition );
    BmBench_reinit( self->transition, size );
    BmBench_attachLast( self->transition,
        newBmCodeAs( BmBench_at( buildTransition, 1 ) ),
        0, BmBench_valueAt( buildTransition, 1 )
    );
    uint counter= 1;

    for( uint i= 2 ; i <= size ; ++i )
    {
        if ( BmCode_isEqualTo( BmBench_at( self->transition, counter ), BmBench_at( buildTransition, i ) ) )
        {
            BmBench_at_value( self->transition, counter,
                BmBench_valueAt( self->transition, counter ) + BmBench_valueAt( buildTransition, i )
            );
        }
        else 
        {
            counter= BmBench_attachLast( self->transition,
                newBmCodeAs( BmBench_at( buildTransition, i ) ),
                0, BmBench_valueAt( buildTransition, i ) );            
        }
    }
    
    // and end:
    deleteBmBench( buildTransition );
    return self->transition;
}

BmBench* BmTransition_newDistributionByInfering( BmTransition * self, BmBench* configDistribution )
{
    BmBench* constructionDistrib= newBmBenchAs( configDistribution );
    uint configDimention = BmCode_dimention( BmBench_at( configDistribution, 1 ) );
    for( uint i= configDimention + 1 ; i <= self->overallDimention ; ++i )
    {
        // Infers dependency possibilities:
        BmBench* newDistrib= BmCondition_newDistributionByInfering_mask(
            BmTransition_nodeAt(self, i),
            constructionDistrib,
            BmTransition_dependanciesAt(self, i) );
        deleteBmBench(constructionDistrib);
        constructionDistrib= newDistrib;
    }
    
    BmTransition_setFomOverallDistribution(self, constructionDistrib);
    return constructionDistrib;
}

BmBench* BmTransition_inferFromState_andAction( BmTransition * self, BmCode* state, BmCode* action )
{
    // Set initial configuration :
    BmCode* startConf= newBmCode( BmCode_dimention(state) +  BmCode_dimention(action) );

    for( uint i=1 ; i <= BmCode_dimention(state) ; ++i )
        BmCode_at_set( startConf, i, BmCode_at(state, i) );
    for( uint i=1 ; i <= BmCode_dimention(action) ; ++i )
        BmCode_at_set( startConf,  BmCode_dimention(state)+i, BmCode_at(action, i) );

    // Set a initial determinist distribution :
    BmBench * distrib= newBmBench( BmCode_dimention(startConf) );
    BmBench_attachLast(distrib, startConf, 0, 1.0);

    // infer :
    deleteBmBench( BmTransition_newDistributionByInfering(self, distrib) );
    
    // Clean :
    deleteBmBench(distrib);

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
