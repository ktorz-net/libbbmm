#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   M O D E L  :  T R A N S I T I O N                           *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor*/
BmInferer* newBmInferer( BmCode* variableSpace, uint inputDimention, uint outputDimention )
{
    return BmInferer_create( newEmpty(BmInferer), variableSpace, inputDimention, outputDimention );
}

BmInferer* newBmInfererStateAction( BmCode* stateSpace, BmCode* actionSpace )
{
    BmCode* overallSpace= newBmCodeMerge_list( 3, stateSpace, actionSpace, stateSpace );
    BmInferer* trans= BmInferer_create(
        newEmpty(BmInferer), overallSpace,
        BmCode_dimention( stateSpace ) + BmCode_dimention( actionSpace ),
        BmCode_dimention( stateSpace )
    );
    deleteBmCode( overallSpace );
    return trans;
}

BmInferer* newBmInfererStateActionShift( BmCode* stateSpace, BmCode* actionSpace, BmCode* shiftSpace )
{
    BmCode* overallSpace= newBmCodeMerge_list( 4, stateSpace, actionSpace, shiftSpace, stateSpace );
    BmInferer* trans= BmInferer_create(
        newEmpty(BmInferer), overallSpace,
        BmCode_dimention( stateSpace ) + BmCode_dimention( actionSpace ),
        BmCode_dimention( stateSpace )
    );
    deleteBmCode( overallSpace );
    return trans;
}

BmInferer* BmInferer_create( BmInferer* self, BmCode* variableSpace, uint inputDimention, uint outputDimention )
{
    self->inputDimention= inputDimention;
    self->outputDimention= outputDimention;
    self->overallDimention= BmCode_dimention(variableSpace);

    self->network= newBmBench( self->overallDimention );
    self->nodes= newEmptyArray( BmCondition, self->overallDimention );

    for( uint i = 1 ; i <= self->overallDimention ; ++i )
    {
        BmCondition_createBasic( array_on(self->nodes, i), BmCode_digit(variableSpace, i) );
        BmBench_attachCode( self->network, newBmCode( 0 ) );
    }

    self->distribution= newBmBench( self->overallDimention );
    return self;
}

/* Destructor */
BmInferer* BmInferer_destroy(BmInferer* self)
{
    for(uint i = 1 ; i <= self->overallDimention ; ++i )
        BmCondition_destroy( array_on(self->nodes, i) );
    free( self->nodes );
    deleteBmBench(self->network);
    deleteBmBench( self->distribution );
    return self;
}

void deleteBmInferer(BmInferer* self)
{
    BmInferer_destroy(self);
    free(self);
}


/* Accessor */
BmBench* BmInferer_distribution( BmInferer * self )
{
    return self->distribution;
}

uint BmInferer_inputDimention( BmInferer* self )
{
    return self->inputDimention;
}

uint BmInferer_outputDimention( BmInferer* self )
{
    return self->outputDimention;
}

uint BmInferer_shiftDimention( BmInferer* self )
{
    return self->overallDimention - ( self->inputDimention + self->outputDimention );
}

uint BmInferer_overallDimention( BmInferer* self )
{
    return self->overallDimention;
}

BmCondition* BmInferer_node( BmInferer * self, uint iNode )
{
    return (BmCondition*)array_on( self->nodes, iNode );
}

uint BmInferer_node_size( BmInferer * self, uint iNode )
{
    return ((BmCondition*)array_on( self->nodes, iNode ))->range;
}

BmCode* BmInferer_node_parents( BmInferer * self, uint iNode )
{
    return BmBench_codeAt(self->network, iNode);
}


/* Construction */
BmCondition* BmInferer_node_reinitWith( BmInferer* self, uint index, BmCode* newParents )
{
    // Reccord parent mask: dependency
    BmCode_switch( BmBench_codeAt( self->network, index ), newParents );
    deleteBmCode( newParents );
    BmCode* dependency= BmBench_codeAt( self->network, index );
    
    // Build dependance space:
    BmCode* depSpace= newBmCode( BmCode_dimention(dependency) );
    for( uint i= 1 ; i <= BmCode_dimention(dependency) ; ++i )
    {
        BmCode_at_set( depSpace, i,
            BmInferer_node_size(self, BmCode_digit(dependency, i)) );
    }

    // Re-initialize the condition
    BmCondition* condition= array_on(self->nodes, index);
    BmCondition_reinitWith( condition, condition->range, depSpace, newBmBench(1) );
    return condition;
}

BmCondition* BmInferer_reinitIndependantNode( BmInferer* self, uint index )
{
    return BmInferer_node_reinitWith( self, index, newBmCode(0) );
}

BmCondition* DEPRECIATED_BmInferer_node_reinitWith( BmInferer* self, uint index, BmCode* newDependenceList, BmBench* newDefaultDistrib )
{
    // Reccord parent mask: dependency
    BmCode_switch( BmBench_codeAt( self->network, index ), newDependenceList );
    deleteBmCode( newDependenceList );
    BmCode* dependency= BmBench_codeAt( self->network, index );
    
    // Build dependance space:
    BmCode* depSpace= newBmCode( BmCode_dimention(dependency) );
    for( uint i= 1 ; i <= BmCode_dimention(dependency) ; ++i )
    {
        BmCode_at_set( depSpace, i,
            BmInferer_node_size(self, BmCode_digit(dependency, i)) );
    }

    // Re-initialize the condition
    BmCondition* condition= array_on(self->nodes, index);
    BmCondition_reinitWith( condition, condition->range, depSpace, newDefaultDistrib );
    return condition;
}

/* Process */
BmBench* _BmInferer_setFomOverallDistribution(BmInferer * self, BmBench* overallDistrib)
{
    // Generate the mask of state' varaibles (i.e the last ones):
    BmCode* outputMask= newBmCode( self->outputDimention );
    uint LastNotOutput= self->overallDimention - self->outputDimention;
    for( uint i= 1 ; i <= self->outputDimention ; ++i )
    {
        BmCode_at_set( outputMask, i, LastNotOutput+i );
    }

    // generate transition by reducing the overall distribution:
    BmBench* buildTransition = newBmBench( self->distribution->capacity );
    BmBench_add_reducted( buildTransition, overallDistrib, outputMask );
    BmBench_sort( buildTransition, (fctptr_BmBench_compare)BmBench_is_codeGreater );


    // merge consecutive doubles:
    uint size= BmBench_size( buildTransition );
    BmBench_reinit( self->distribution, size );
    BmBench_attachCode_vector(
        self->distribution,
        newBmCodeAs( BmBench_codeAt(buildTransition, 1) ),
        newBmVectorAs( BmBench_vectorAt(buildTransition, 1) )
    );
    uint counter= 1;

    for( uint i= 2 ; i <= size ; ++i )
    {
        if ( BmCode_isEqualTo( BmBench_codeAt( self->distribution, counter ), BmBench_codeAt( buildTransition, i ) ) )
        {
            BmBench_at_setValue( self->distribution, counter,
                BmBench_valueAt( self->distribution, counter ) + BmBench_valueAt( buildTransition, i )
            );
        }
        else 
        {
            counter= BmBench_attachCode_vector(
                self->distribution,
                newBmCodeAs( BmBench_codeAt(buildTransition, i) ),
                newBmVectorAs( BmBench_vectorAt(buildTransition, i) )
            );            
        }
    }
    
    // and end:
    deleteBmBench( buildTransition );
    return self->distribution;
}

BmBench* BmInferer_process_newOverallDistribution( BmInferer* self, BmBench* inputDistribution )
{
    BmBench* constructionDistrib= newBmBenchAs( inputDistribution );

    uint configDimention = BmCode_dimention( BmBench_codeAt( inputDistribution, 1 ) );
    for( uint i= configDimention + 1 ; i <= self->overallDimention ; ++i )
    {
        // Infers dependency possibilities:
        BmBench* newDistrib= BmCondition_newDistributionByInfering_mask(
            BmInferer_node(self, i),
            constructionDistrib,
            BmInferer_node_parents(self, i) );
        deleteBmBench(constructionDistrib);
        constructionDistrib= newDistrib;
    }
    
    _BmInferer_setFomOverallDistribution(self, constructionDistrib);

    return constructionDistrib;
}

BmBench* BmInferer_process( BmInferer* self, BmBench* inputDistribution )
{
    // infer :
    BmBench* overall= BmInferer_process_newOverallDistribution(self, inputDistribution);
    deleteBmBench( overall );
    return self->distribution;
}

BmBench* BmInferer_processState_Action( BmInferer* self, BmCode* state, BmCode* action )
{
    // Set the initial determinist distribution :
    BmBench * inputDistribution= newBmBench( 1 );
    BmBench_attachCode_vector( inputDistribution,
        newBmCodeMerge_list( 2, state, action ),
        newBmVector_list(1, 1.0)
    );
    // infer :
    deleteBmBench( BmInferer_process_newOverallDistribution(self, inputDistribution) );

    // Clean:
    deleteBmBench( inputDistribution );
    return self->distribution;
}

/* Printing */
char* BmInferer_print(BmInferer* self, char* output)
{
    BmInferer_printStateActionSignature(self, output);
    strcat(output, " ");
    BmInferer_printDependency(self, output);
    return output;
}

char* BmInferer_printStateActionSignature(BmInferer* self, char* output)
{
    char buffer[64];

    uint stateDimention= self->outputDimention;
    uint actionDimention= self->inputDimention - self->outputDimention;

    // State:
    strcat(output, "[");
    if( stateDimention > 0 )
    {
        sprintf(buffer, "%u", BmInferer_node(self, 1)->range );
        strcat(output, buffer);
    }
    for( uint i= 2 ; i <= stateDimention ; ++i)
    {
        sprintf(buffer, ", %u", BmInferer_node(self, i)->range );
        strcat(output, buffer);
    }
    strcat(output, "]x[");

    // Action:
    uint stateActionDim= stateDimention + actionDimention;
    if( actionDimention > 0 )
    {
        sprintf(buffer, "%u", BmInferer_node(self, stateDimention+1)->range );
        strcat(output, buffer);
    }
    for( uint i= stateDimention+2 ; i <= stateActionDim ; ++i)
    {
        sprintf(buffer, ", %u", BmInferer_node(self, i)->range );
        strcat(output, buffer);
    }
    strcat(output, "](");

    // Shift:
    uint shiftDimention= BmInferer_shiftDimention(self);
    uint end= stateActionDim+shiftDimention;
    if( shiftDimention > 0 )
    {
        sprintf(buffer, "%u", BmInferer_node(self, stateActionDim+1)->range );
        strcat(output, buffer);
    }
    for( uint i= stateActionDim+2 ; i <= end ; ++i)
    {
        sprintf(buffer, ", %u", BmInferer_node(self, i)->range );
        strcat(output, buffer);
    }
    strcat(output, ")");

    return output;
}

char* BmInferer_printDependency(BmInferer* self, char* output)
{
    uint shiftStart= self->inputDimention + 1;
    uint shiftDimention= BmInferer_shiftDimention(self);
    uint shiftEnd= shiftStart+shiftDimention;

    // Shift:
    strcat(output, "|");
    if( shiftDimention > 0 )
    {
        strcat(output, " ");
        BmCode_print( BmBench_codeAt(self->network, shiftStart), output );
    }
    for( uint i= shiftStart+1 ; i < shiftEnd ; ++i)
    {
        strcat(output, ", ");
        BmCode_print( BmBench_codeAt(self->network, i), output );
    }
    strcat(output, " |");

    // future (state):
    if( self->outputDimention > 0 )
    {
        strcat(output, " ");
        BmCode_print( BmBench_codeAt(self->network, shiftEnd), output );
    }
    for( uint i= shiftEnd+1 ; i <= self->overallDimention ; ++i)
    {
        strcat(output, ", ");
        BmCode_print( BmBench_codeAt(self->network, i), output );
    }
    strcat(output, " |");

    return output;
}
