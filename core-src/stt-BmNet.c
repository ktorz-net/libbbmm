#include "bbmm-stt.h"

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
    BmNet_distroy(self);
    free( self );
}

/* Protected - to use with precaution */
BmNet* BmNet_create(BmNet* self, uint size)
{
    self->edges= newArrayOfBmCode(size, 0);
    self->size= size;
    return self;
}

BmNet* BmNet_distroy(BmNet* self)
{
    deleteArrayOfBmCode( self->edges, self->size );
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
    BmCode_initializeArray( array_on(self->edges, iNode), 0, NULL);
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

