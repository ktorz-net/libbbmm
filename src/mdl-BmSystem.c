/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B B M M  S Y S T E M
 *
 * Referring to Markov Decision process.
 * A model is tuple of state and action spaces, transition and a reward function.
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#include "bbmm-models.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/*   B M  S Y S T E M   */


/* Constructor Destructor */
BmSystem* newBmSystemEmpty( uint dimState, uint dimAction, uint dimShift )
{
    return BmSystem_createEmpty( newEmpty(BmDomain), dimState, dimAction, dimShift );
}

BmSystem* newBmSystem( uint dimState, BmDomain ** stateDoms, uint dimAction, BmDomain ** actionDoms, uint dimShift, BmDomain ** shiftDomains )
{
    return BmSystem_create( newEmpty(BmDomain), dimState, stateDoms, dimAction, actionDoms, dimShift, shiftDomains );
}

void deleteBmSystem(BmSystem* self)
{
    BmSystem_destroy(self);
    free(self);
}

void deleteDeepBmSystem(BmSystem* self)
{
    BmSystem_deepDestroy(self);
    free(self);
}

/* fill and empty a structure */
BmSystem* BmSystem_createEmpty( BmSystem* self, uint dimState, uint dimAction, uint dimShift )
{
    self->state= newBmSpaceEmpty( dimState );
    self->action= newBmSpaceEmpty( dimAction );
    self->shift= newBmSpaceEmpty( dimShift );

    BmCode* stateCode= newBmCode_all(dimState, 1);
    BmCode* actionCode= newBmCode_all(dimAction, 1);
    BmCode* shiftCode= newBmCode_all(dimShift, 1);

    self->transition= newBmTransitionWithShift( stateCode, actionCode, shiftCode );
    //WdValueFct *reward;

    deleteBmCode(stateCode);
    deleteBmCode(actionCode);
    deleteBmCode(shiftCode);

    self->futureName= newEmptyArray( char*, dimState );
    for( uint i= 1 ; i <= dimState ; ++i )
    {
        array_at_set(self->futureName, i, malloc(4));
        strcpy( array_at( self->futureName, i ), "'" );
    }

    return self;
}

BmSystem* BmSystem_create( BmSystem* self, uint dimState, BmDomain ** stateDoms, uint dimAction, BmDomain ** actionDoms, uint dimShift, BmDomain ** shiftDoms )
{
    char* stateNames[dimState];
    for( uint i = 0 ; i < dimState ; ++i )
    {
        stateNames[i]= malloc( 32 );
        sprintf( stateNames[i], "S%i", i+1 );
    }

    self->state= newBmSpace( dimState, stateNames, stateDoms );

    char* actionNames[dimAction];
    for( uint i = 0 ; i < dimAction ; ++i )
    {
        actionNames[i]= malloc( 32 );
        sprintf( actionNames[i], "A%i", i );
    }
    self->action= newBmSpace( dimAction, actionNames, actionDoms );

    char* shiftNames[dimShift];
    for( uint i = 0 ; i < dimShift ; ++i )
    {
        shiftNames[i]= malloc( 32 );
        sprintf( shiftNames[i], "X%i", i );
    }
    self->shift= newBmSpace( dimShift, shiftNames, shiftDoms );

    BmCode* stateCode= BmSpace_asNewBmCode(self->state);
    BmCode* actionCode= BmSpace_asNewBmCode(self->state);
    BmCode* shiftCode= BmSpace_asNewBmCode(self->state);

    self->transition= newBmTransitionWithShift( stateCode, actionCode, shiftCode );
    //WdValueFct *reward;

    for( uint i = 0 ; i < dimState ; ++i )
    {
        free(stateNames[i]);
    }
    for( uint i = 0 ; i < dimAction ; ++i )
    {
        free(actionNames[i]);
    }
    for( uint i = 0 ; i < dimShift ; ++i )
    {
        free(shiftNames[i]);
    }

    deleteBmCode(stateCode);
    deleteBmCode(actionCode);
    deleteBmCode(shiftCode);

    self->futureName= newEmptyArray( char*, dimState );
    for( uint i= 1 ; i <= dimState ; ++i )
    {
        array_at_set( self->futureName, i, malloc( sizeof( BmSpace_variable_name(self->state, i) )+4) );
        strcpy( array_at( self->futureName, i ), BmSpace_variable_name(self->state, i) );
        strcat( array_at( self->futureName, i ), "'" );
    }

    return self;
}

BmSystem* BmSystem_deepDestroy( BmSystem* self )
{
    /* Get the Different domains in spaces */
    BmDomain* differentDomains[ BmSpace_dimention(self->state)+BmSpace_dimention(self->action)+BmSpace_dimention(self->shift) ];
    uint nbDiffDom= BmSystem_feedWithDomains(self, differentDomains, 0);

    /* Delete the Different domains */
    for( uint i = 0 ; i < nbDiffDom ; ++i )
        deleteBmDomain( differentDomains[i] );

    /* Terminate */
    BmSystem_destroy(self);
    return self;
}

BmSystem* BmSystem_destroy( BmSystem* self )
{
    uint dimState= BmSpace_dimention(self->state);
    for( uint i= 1 ; i < dimState ; ++i )
        free( array_at(self->futureName, i) );
    free( self->futureName );

    deleteBmTransition( self->transition );
    deleteBmSpace( self->state );
    deleteBmSpace( self->action );
    deleteBmSpace( self->shift );

    return self;
}

/* Initialize variable */
uint BmSystem_attachStateVariable( BmSystem* self, char * name, BmDomain* domain )
{
    // Get next available index
    uint dim= BmSpace_dimention( self->state);
    assert( dim > 0 );
    uint iVar= 1;
    while( iVar <= dim && array_at( (self->state->varDomains), iVar ) != NULL )
        iVar+= 1;
    assert( iVar <= dim );

    // Create the variable
    BmSpace_attachVariable( self->state, iVar, name, domain );
    BmTransition_node_initialize( self->transition, BmTransition_indexOfStateVariableT0( self->transition, iVar), BmDomain_size(domain) );
    BmTransition_node_initialize( self->transition, BmTransition_indexOfStateVariableT1( self->transition, iVar), BmDomain_size(domain) );

    // record future name:
    free( array_at( self->futureName, iVar ) );
    array_at_set( self->futureName, iVar, malloc( sizeof( name )+4 ) );
    strcpy( array_at( self->futureName, iVar ), name );
    strcat( array_at( self->futureName, iVar ), "'" );

    return iVar;
}

uint  BmSystem_attachActionVariable( BmSystem* self, char * name, BmDomain* domain )
{
    // Get next available index
    uint dim= BmSpace_dimention( self->action);
    assert( dim > 0 );
    uint iVar= 1;
    while( iVar <= dim && array_at( (self->action->varDomains), iVar ) != NULL )
        iVar+= 1;
    assert( iVar <= dim );

    // Create the variable
    BmSpace_attachVariable( self->action, iVar, name, domain );
    BmTransition_node_initialize( self->transition, BmTransition_indexOfActionVariable( self->transition, iVar), BmDomain_size(domain) );

    return iVar;
}

uint  BmSystem_attachShiftVariable( BmSystem* self, char * name, BmDomain* domain )
{
    // Get next available index
    uint dim= BmSpace_dimention( self->shift);
    assert( dim > 0 );
    uint iVar= 1;
    while( iVar <= dim && array_at( (self->shift->varDomains), iVar ) != NULL )
        iVar+= 1;
    assert( iVar <= dim );

    // Create the variable
    BmSpace_attachVariable( self->shift, iVar, name, domain );
    BmTransition_node_initialize( self->transition, BmTransition_indexOfShiftVariable( self->transition, iVar), BmDomain_size(domain) );

    return iVar;
}

BmCode* BmSystem_variable_dependOnArray( BmSystem* self, char * varName, uint parentSize, char ** parentNames )
{
    // get variable index
    uint iNode= BmSystem_variable_nodeId(self, varName);

    // Build the array of parent indexes
    uint parentNodes[parentSize];
    for( uint i = 0 ; i < parentSize ; ++i )
    {
        parentNodes[i]= BmSystem_variable_nodeId(self, parentNames[i]);
    }

    BmTransition_node_dependArray( self->transition, iNode, parentSize, parentNodes );
    
    return BmBench_at(self->transition->network, iNode);
}

BmCode* BmSystem_variable_dependOn( BmSystem* self, char * varName, uint parentSize, ... )
{
    char* parents[parentSize];
    // Build parents array from args
    va_list ap;
    va_start(ap, parentSize); 
    for ( uint i = 0 ; i < parentSize ; ++i )
    {
        parents[i]= va_arg(ap, char* );
    }
    va_end(ap);
    // Create the dependancy
    return BmSystem_variable_dependOnArray(self, varName, parentSize, parents);
}

/* Initialize transition probability */
BmDistribution* BmSystem_nodeId_newBmDistribution(BmSystem* self, uint id, uint numberOfOutputs, char** outputs, double* probabilities )
{
    BmDistribution* distrib= newBmDistribution(1);
    for ( uint i = 0 ; i < numberOfOutputs ; ++i )
    {
        uint iOutput= BmDomain_searchStr( BmSystem_nodeId_domain(self, id), outputs[i] );
        BmDistribution_addOutput(distrib, iOutput, probabilities[i]);
    }
    return distrib;
}

BmSystem* BmSystem_variable_initializeProbabilities( BmSystem* self, char * varName, uint numberOfOutputs, ... )
{
    // Get variable and parent Id
    uint id= BmSystem_variable_nodeId(self, varName);

    char* outputs[numberOfOutputs];
    double probabilities[numberOfOutputs];

    // Read the list of outputs and associate probabilities
    va_list ap;
    va_start(ap, numberOfOutputs); 
    for ( uint i = 0 ; i < numberOfOutputs ; ++i )
    {
        outputs[i]= va_arg(ap, char*);
        probabilities[i]= va_arg(ap, double);
    }
    va_end(ap);

    // Initialize condition BmDistribution
    BmDistribution* distrib= BmSystem_nodeId_newBmDistribution(self, id, numberOfOutputs, outputs, probabilities );
    BmCondition* c= BmTransition_nodeAt( self->transition, id );
    BmCondition_initialize( c, c->outputSize, c->parentRanges, distrib );
    deleteBmDistribution( distrib );

    return self;
}

BmSystem* BmSystem_variable_addProbabilities( BmSystem* self, char * varName, uint numberOfParents, ... )
{
    // Get variable Id
    uint id= BmSystem_variable_nodeId(self, varName);

    // Generate parent state:
    BmCondition* condition= BmTransition_nodeAt( self->transition, id );
    BmCode* config= newBmCode_all( BmCondition_dimention(condition), 0);
    BmCode* configOrder= newBmCode_all( BmCondition_dimention(condition), 0);

    // Read the list of parents and associate state
    va_list ap;
    va_start(ap, numberOfParents); 
    for ( uint i = 1 ; i <= numberOfParents ; ++i )
    {
        char* parent= va_arg(ap, char*);
        uint parentId= BmSystem_nodeId_parentId(self, id, parent);
        char* state= va_arg(ap, char*);
        uint iState= BmDomain_searchStr( BmSystem_nodeId_domain(self, parentId), state );

        BmCode_at_set( config, parentId, iState );
        BmCode_at_set( configOrder, i, parentId );
    }

    // Read the list of outputs and associate probabilities
    uint numberOfOutputs= va_arg(ap, uint);
    char* outputs[numberOfOutputs];
    double probabilities[numberOfOutputs];

    for ( uint i = 0 ; i < numberOfOutputs ; ++i )
    {
        outputs[i]= va_arg(ap, char*);
        probabilities[i]= va_arg(ap, double);
    }
    va_end(ap);

    // Add condition BmDistribution
    BmDistribution* distrib= BmSystem_nodeId_newBmDistribution(self, id, numberOfOutputs, outputs, probabilities );
    BmCondition_at_readOrder_set( BmTransition_nodeAt( self->transition, id ), config, configOrder, distrib );

    deleteBmDistribution( distrib );
    deleteBmCode(config);
    deleteBmCode(configOrder);
    return self;
}

/* Test */
uint BmSystem_isValid(BmSystem* self)
{
    uint okSpace= BmSpace_isValid( self->state );
    uint okAction= BmSpace_isValid( self->action );
    uint okShift= BmSpace_isValid( self->shift );
    return okSpace && okAction && okShift;
}

/* initialization */

/* Update */

/* Accessor */
BmSpace* BmSystem_stateSpace(BmSystem* self)
{
    return self->state;
}

BmSpace* BmSystem_actionSpace(BmSystem* self)
{
    return self->action;
}

BmSpace* BmSystem_shiftSpace(BmSystem* self)
{
    return self->shift;
}

BmTransition* BmSystem_transition(BmSystem* self)
{
    return self->transition;
}


BmCondition* BmSystem_variable(BmSystem* self, char * varName)
{
    uint varId= BmSystem_variable_nodeId(self, varName);
    return BmTransition_nodeAt( BmSystem_transition(self), varId );
}

uint BmSystem_variable_nodeId(BmSystem* self, char * varName)
{
    // Search state variable
    uint iVar= BmSpace_variableName_index( self->state, varName );
    if( iVar )
        return iVar;

    uint iTrans= BmSpace_dimention( self->state );
    
    // Search action variable
    iVar= BmSpace_variableName_index( self->action, varName );
    if( iVar )
        return iTrans+iVar;

    iTrans+= BmSpace_dimention( self->action );

    // Search shift variable
    iVar= BmSpace_variableName_index( self->shift, varName );
    if( iVar )
        return iTrans+iVar;

    iTrans+= BmSpace_dimention( self->shift );

    // Must be a future state variable...
    uint varNameLen= strlen(varName);
    assert( varName[ varNameLen-1 ] == '\'' );
    
    char smallName[varNameLen+2];
    strcpy( smallName, varName );
    smallName[ varNameLen-1 ]= '\0';

    iVar= BmSpace_variableName_index( self->state, smallName );

    assert( iVar <=  BmSpace_dimention( self->state ) );
    
    return iTrans+iVar;
}

uint BmSystem_nodeId_parentId(BmSystem* self, uint id, char* parent)
{
    return BmCode_search(
        BmBench_at( self->transition->network, id),
        BmSystem_variable_nodeId(self, parent)
    );
}

char* BmSystem_nodeId_variableName(BmSystem* self, uint id)
{
    uint stateDim= BmTransition_stateDimention( self->transition );
    uint actionDim= BmTransition_actionDimention( self->transition );
    uint shiftDim= BmTransition_shiftDimention( self->transition );

    // is a state variable
    if( id <= stateDim )
        return  BmSpace_variable_name( self->state, id );

    id-=  stateDim;
    
    // is an action variable
    if( id <= actionDim )
        return  BmSpace_variable_name( self->action, id );

    id-=  actionDim;
    
    // is a shift variable
    if( id <= shiftDim )
        return  BmSpace_variable_name( self->shift, id );

    id-=  shiftDim;

    // Must be a future state variable...
    assert( id <= stateDim );
    return array_at( self->futureName, id );
}

uint BmSystem_nodeId_parentSize(BmSystem* self, uint id)
{
    return BmCode_dimention( BmTransition_dependanciesAt( self->transition, id) );
}

BmDomain* BmSystem_nodeId_domain(BmSystem* self, uint id)
{
    uint stateDim= BmTransition_stateDimention( self->transition );
    uint actionDim= BmTransition_actionDimention( self->transition );
    uint shiftDim= BmTransition_shiftDimention( self->transition );

    // is a state variable
    if( id <= stateDim )
        return  BmSpace_variable_domain( self->state, id );

    id-=  stateDim;
    
    // is an action variable
    if( id <= actionDim )
        return  BmSpace_variable_domain( self->action, id );

    id-=  actionDim;
    
    // is a shift variable
    if( id <= shiftDim )
        return  BmSpace_variable_domain( self->shift, id );

    id-=  shiftDim;

    // Must be a future state variable...
    assert( id <= stateDim );
    return  BmSpace_variable_domain( self->state, id );
}

/* Generators */
uint BmSystem_feedWithDomains( BmSystem* self, BmDomain** domainsBuffer, uint nboDomainsInBuffer )
{
    uint nbDiffDom= BmSpace_feedWithDomains(self->state, domainsBuffer, nboDomainsInBuffer);
    nbDiffDom= BmSpace_feedWithDomains(self->action, domainsBuffer, nbDiffDom);
    nbDiffDom= BmSpace_feedWithDomains(self->shift, domainsBuffer, nbDiffDom);
    return nbDiffDom;
}

/* State */

/* Printing */
char* BmSystem_nodeId_printIdentity( BmSystem* self, uint nodeId, char* output )
{
    BmCode* parents= BmBench_at(self->transition->network, nodeId);
    char buffer[1024];

    sprintf( buffer, "%s.%u(",
        BmSystem_nodeId_variableName(self, nodeId), 
        BmTransition_sizeAt( self->transition, nodeId ) );
    strcat(output, buffer );

    if( BmCode_dimention(parents) > 0 )
    {
        sprintf( buffer, "%s.%u",
        BmSystem_nodeId_variableName(self, BmCode_at(parents, 1)), 
        BmTransition_sizeAt( self->transition, BmCode_at(parents, 1) ) );
        strcat(output, buffer );
                
        for( uint i= 2 ; i <= BmCode_dimention(parents) ; ++i)
        {
        sprintf( buffer, ", %s.%u",
        BmSystem_nodeId_variableName(self, BmCode_at(parents, i)), 
        BmTransition_sizeAt( self->transition, BmCode_at(parents, i) ) );
        strcat(output, buffer );
        }
    }
    strcat(output, ")");
    return output;
}

char* BmSystem_printNetwork( BmSystem* self, char* output )
{
    BmBench* net= self->transition->network;

    if( net->size > 0 )
    {
        BmSystem_nodeId_printIdentity( self, 1, output );
    }
    for( uint i= 2 ; i <= net->size ; ++i)
    {
        strcat(output, ", " );
        BmSystem_nodeId_printIdentity( self, i, output );
    }

    return output;
}

char* _BmCondition_printCode_inDomain(BmCondition* cdt, BmCode* code, BmDomain** parentDoms, char* output)
{
    uint inputSize= BmCondition_dimention(cdt);
    // Security:
    assert( BmCode_dimention(code) == inputSize );

    strcat(output, "[");
    for( uint i= 1 ; i <= inputSize ; ++i)
    {
        if( BmCode_at(code, i) == 0 )
            strcat( output, "-" );
        else
            strcat( output, BmDomain_strAt( array_at(parentDoms, i), BmCode_at(code, i) ) );
        strcat(output, ", ");
    }
    output[strlen(output)-2]= '\0'; 
    strcat(output, "]: " );
    return output;
}

char* BmSystem_printVariable( BmSystem* self, char* varName, char* output )
{
    uint nodeId= BmSystem_variable_nodeId(self, varName);
    
    // get parents' spaces
    BmCode* parents= BmTransition_dependanciesAt( self->transition, nodeId);
    BmDomain* pDom[ BmCode_dimention(parents) ];
    for( uint i= 1; i <= BmCode_dimention(parents); ++i )
    {
        array_at_set( pDom, i, BmSystem_nodeId_domain(self, i) );
    }
    
    BmSystem_nodeId_printIdentity(self, nodeId, output);
    strcat(output, ": {");

    BmCondition* cdt= BmTransition_nodeAt( self->transition, nodeId );
    BmBench* collec = BmTree_asNewBench( cdt->selector );

    if( BmBench_size(collec) == 0 )
    {
        uint iItem= BmBench_attachLast(collec, newBmCode_all( BmCondition_dimention(cdt), 0));
        BmBench_at_tag( collec, iItem, 1 );
    }
    
    _BmCondition_printCode_inDomain(cdt, BmBench_at( collec, 1 ), pDom, output );
    BmDistribution_print( array_at(cdt->distributions, BmBench_tagAt(collec, 1) ), output );
    for( uint i = 2 ; i <= BmBench_size(collec) ; ++i )
    {
        strcat( output, ",\n  " );
        _BmCondition_printCode_inDomain(cdt, BmBench_at( collec, i ), pDom, output );
        BmDistribution_print( array_at(cdt->distributions, BmBench_tagAt(collec, i) ), output );
    }

    strcat(output, "}");
    deleteBmBench(collec);
    return output;
}
