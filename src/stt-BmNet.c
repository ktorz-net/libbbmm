#include "bbmm-structures.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m  S T R U C T U R E :  N E T W O R K
 *
 * Define a Simple Network 
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor Destructor */
BmNet* newBmNet(uint size)
{
    return BmNet_create( newEmpty(BmNet), size );
}

void deleteBmNet(BmNet * self)
{
    BmNet_destroy(self);
    free( self );
}

/* Protected - to use with precaution */
BmNet* BmNet_create(BmNet* self, uint size)
{
    self->edges= newEmptyArray( BmCode, size );
    for( uint i = 1 ; i <= size ; ++i )
    {
        BmCode_create( array_on( self->edges, i ), 0 );
    }
    self->size= size;
    return self;
}

BmNet* BmNet_destroy(BmNet* self)
{
    for( uint i = 1 ; i <= self->size; ++i )
    {
        BmCode_destroy( array_on( self->edges, i ) );
    }
    deleteEmptyArray(self->edges);
    return self;
}

/* Accessor */
BmCode* BmNet_at(BmNet* self, uint i)
{
    return array_on(self->edges, i);
}

/* Test */

/* Modification */
void BmNet_clearAt(BmNet* self, uint iNode)
{
    BmCode* c= array_on(self->edges, iNode);
    BmCode_initializeBasic( c, 0 );
}

void BmNet_at_connect(BmNet* self, uint from, BmCode* targets)
{
    BmCode_copy( array_on(self->edges, from), targets );
    BmCode_sort( array_on(self->edges, from) );
}

/* Printing */
char* BmNet_wording(BmNet* self)
{
    char* output= bm_textBuffer();
    strcpy(output, "");
    return BmNet_print(self, output);
}

char* BmNet_print(BmNet* self, char* output)
{
    char buffer[64];

    if( self->size > 0 )
    {
        sprintf(buffer, "%u", 1 );
        strcat(output, buffer);
        BmCode_print( BmNet_at(self, 1), output );
    }
    for( uint i= 2 ; i <= self->size ; ++i)
    {
        sprintf(buffer, ", %u", i );
        strcat(output, buffer);
        BmCode_print( BmNet_at(self, i), output );
    }

    return output;
}

