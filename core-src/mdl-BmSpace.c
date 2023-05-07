/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   S P A C E   A N D   S T A T E
 *
 * Represent a multi-dimentional variable (state)
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#include "bbmm-models.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/*   B M S P A C E */
 
/* Constructor Destructor */
BmSpace* newBmSpaceBasic()
{
    return BmSpace_createEmpty( newEmpty(BmSpace), 0 );
}

BmSpace* newBmSpace( uint dim, char ** variableNames, BmDomain ** variableDomains )
{
    return BmSpace_create( newEmpty(BmSpace), dim, variableNames, variableDomains );
}

BmSpace* newBmSpaceEmpty( uint dim )
{
    return BmSpace_createEmpty( newEmpty(BmSpace), dim );
}

void deleteBmSpace(BmSpace* self)
{
    BmSpace_distroy(self);
    free(self);
}

void deleteDeepBmSpace(BmSpace* self)
{
    BmSpace_distroyDeep(self);
    free(self);
}

/* Fill and empty a structure */
BmSpace* BmSpace_create( BmSpace* self, uint dim, char ** variableNames, BmDomain ** variableDomains )
{
    self->varNames= newEmptyArray( char*, dim );
    self->varDomains= newEmptyArray( BmDomain*, dim );
    for( uint i = 0; i < dim ; ++i )
    {
        BmSpace_attachVariable(self, i+1, variableNames[i], variableDomains[i] );
    }
    self->dimention= dim;
    return self;
}

BmSpace* BmSpace_createEmpty( BmSpace* self, uint dim )
{
    self->varNames= newEmptyArray( char*, dim );
    self->varDomains= newEmptyArray( BmDomain*, dim );
    for( uint i = 0; i < dim ; ++i )
    {
        self->varNames[i]= NULL;
        self->varDomains[i]= NULL;
    }
    self->dimention= dim;
    return self;
}

BmSpace* BmSpace_distroy( BmSpace* self )
{
    deleteEmptyArray( self->varDomains );
    for( uint i = 0; i < self->dimention ; ++i )
    {
        if(  self->varNames[i] != NULL )
            free( self->varNames[i] );
    }
    deleteEmptyArray( self->varNames );
    return self;
}

BmSpace* BmSpace_distroyDeep( BmSpace* self )
{
    /* Delete all diferents domains */
    BmDomain* differentDomains[ BmSpace_dimention(self) ];
    uint nbDiffDom= BmSpace_feedWithDomains( self, differentDomains, 0 );
    for( uint i = 0 ; i < nbDiffDom ; ++i )
        deleteBmDomain( differentDomains[i] );

    /* Distroy self */
    return BmSpace_distroy(self);
}

/* Attach elements */
BmSpace* BmSpace_attachVariable( BmSpace* self, uint i, char* name, BmDomain* domain )
{
    char* newName= malloc( sizeof(char) * strlen(name)+2 );
    strcpy( newName, name );

    array_at_set( self->varNames, i, newName );
    array_at_set( self->varDomains, i, domain );

    return self;
}

/* Test */
uint BmSpace_isValid(BmSpace* self)
{
    uint yes= true;
    for( uint i = 0; yes && i < self->dimention ; ++i )
        yes= yes && ( self->varNames[i] != NULL )
            && ( self->varDomains[i] != NULL ); 
    return yes;
}

/* Accessor */
uint BmSpace_dimention(BmSpace* self)
{
    return self->dimention;
}

char* BmSpace_variable_name( BmSpace* self, int i )
{
    return array_at( self->varNames, i );
}

BmDomain * BmSpace_variable_domain( BmSpace* self, int i )
{
    return array_at( self->varDomains, i );
}

uint BmSpace_variable_domainSize(BmSpace* self, int i)
{
    return BmDomain_size( BmSpace_variable_domain( self, i ) );
}

ulong BmSpace_size(BmSpace* self)
{
    BmCode* code= BmSpace_asNewBmCode(self);
    ulong s= BmCode_product( code );
    deleteBmCode( code );
    return s;
}

BmCode* BmSpace_key_toNewCode(BmSpace* self, ulong key)
{
    BmCode* spaceCode= BmSpace_asNewBmCode(self);
    BmCode* code= BmCode_newBmCodeOnKey(spaceCode, key);
    deleteBmCode(spaceCode);
    return code;
}

/* Search */
uint BmSpace_variableName_index(BmSpace* self, char* varName)
{
    uint dim= BmSpace_dimention( self );
    uint iVar= 1;
    while( iVar <= dim && strcmp( varName, BmSpace_variable_name( self, iVar ) ) )
        iVar+= 1;
    if( iVar <= dim )
    {
        return iVar;
    }
    return 0;
}

/* Generators */
uint BmSpace_feedWithDomains( BmSpace* self, BmDomain** domainsBuffer, uint nboDomains )
{
    uint nboDiffDom= nboDomains, dim= BmSpace_dimention(self);
    for( uint i= 1 ; i <= dim ; ++i )
    {
        BmDomain* dom= BmSpace_variable_domain(self, i);
        uint ok= (dom != NULL);
        for( uint j= 1 ; ok && j <= nboDiffDom ; ++j )
        {
            ok= ok && ( dom != array_at(domainsBuffer, j) );
        }
        if( ok )
        {
            nboDiffDom+= 1;
            array_at_set(domainsBuffer, nboDiffDom, dom);
        }
    }
    return nboDiffDom;
}

/* Code Manipulation */
BmCode* BmSpace_asNewBmCode( BmSpace* self )
{
    uint dim= BmSpace_dimention(self);
    BmCode* code= newBmCodeBasic( dim );
    for( uint i= 1 ; i <= dim ; ++i )
    {
        BmCode_at_set( code, i, BmSpace_variable_domainSize(self, i) );
    }
    return code;
}

/* Code State */
char* BmSpace_printVariable_at( BmSpace* self, uint iVar, uint iVal, char* buffer )
{
    strcat( buffer, BmSpace_variable_name( self, iVar ) );
    strcat( buffer, "(" );
    strcat( buffer, BmDomain_strAt( BmSpace_variable_domain(self, iVar), iVal ) );
    strcat( buffer, ")" );
    return buffer;
}

char* BmSpace_code_wording( BmSpace* self, BmCode* code )
{
    char* output= bm_lineBuffer();
    strcpy(output, "");
    return BmSpace_code_print(self, code, output);
}

char* BmSpace_code_print( BmSpace* self, BmCode* code, char* output )
{
    strcat(output, "[");
    uint dim= BmSpace_dimention(self);
    if( dim >= 1 )
    {
        BmSpace_printVariable_at( self, 1, BmCode_at(code, 1), output );
    }
    for( uint i = 2 ; i <= BmSpace_dimention(self) ; ++i )
    {
        strcat( output, ", " );
        BmSpace_printVariable_at( self, i, BmCode_at(code, i), output );
    }
    strcat( output, "]" );
    return output;
}

char* BmSpace_key_wording( BmSpace* self, ulong key )
{
    char* output= bm_lineBuffer();
    strcpy(output, "");
    return BmSpace_key_print(self, key, output);
}

char* BmSpace_key_print( BmSpace* self, ulong key, char* output )
{
    BmCode* code= BmSpace_key_toNewCode(self, key);
    BmSpace_code_print(self, code, output);
    deleteBmCode(code);
    return output;
}

/* Printing */
char* BmSpace_print(BmSpace* self, char* output)
{
    uint dim= BmSpace_dimention(self);
    if( dim >= 1 )
    {
        BmDomain_print( BmSpace_variable_domain(self, 1), output );
    }
    for( uint i= 2 ; i <= dim ; ++i)
    {
        strcat(output, ",\n" );
        BmDomain_print( BmSpace_variable_domain(self, i), output );
    }
    strcat(output, ".");
    return output;
}
