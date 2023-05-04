
#include "bbmm-stt.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
 *   B b M m   S T R U C T U R E :  B E N C H
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */


/* Constructor Destructor */
BmBench* newBmBench(uint capacity)
{
    return BmBench_create( newEmpty(BmBench), capacity );
}

void deleteBmBench(BmBench* self)
{
    BmBench_distroy(self);
    free(self);
}

BmBench* BmBench_create(BmBench* self, uint capacity)
{
    self->capacity= capacity;
    self->items= newEmptyArray( BmCode*, self->capacity );
    self->tags= newEmptyArray( uint, self->capacity );
    self->start= 0;
    self->size= 0;
    return self;
}

BmBench* BmBench_distroy(BmBench* self)
{
    while( self->size > 0 )
    {
        self->start+= 1;
        deleteBmCode( array_at( self->items, self->start ) );
        self->size-= 1;
    }
    deleteEmptyArray( self->tags );
    deleteEmptyArray( self->items );
    return self;
}

/* Initializer */

/* Modification */
void BmBench_resizeCapacity( BmBench* self, uint newCapacity )
{
    // Security:
    assert( newCapacity >= self->size );

    // Allocate new memory
    uint start= (newCapacity-self->size)/2;
    BmCode ** newItems= newEmptyArray( BmCode*, newCapacity );
    uint* newTags= newEmptyArray( uint, newCapacity );

    // Copy
    for( uint i = 1 ; i <= self->size ; ++i )
    {
        array_at_set( newItems, start+i, array_at( self->items, self->start+i ) );
        array_at_set( newTags, start+i, array_at( self->tags, self->start+i ) );
    }
    
    // Update the structure:
    deleteEmptyArray( self->items );
    deleteEmptyArray( self->tags );
    self->items= newItems;
    self->tags= newTags;
    self->capacity= newCapacity;
    self->start= start;
}

/* Accessor */
uint BmBench_size(BmBench* self)
{
    return self->size;
}

BmCode* BmBench_at( BmBench* self, uint i )
{
    return array_at( self->items, self->start+i );
}

uint BmBench_tagAt( BmBench* self, uint i )
{
    return array_at( self->tags, self->start+i );
}

/* Test */

/* Construction Item */
uint BmBench_attachLast( BmBench* self, BmCode* item )
{
    uint id= self->start+self->size+1;
    if( id >= self->capacity )
    {
        BmBench_resizeCapacity( self, (self->size*2)+2);
        id= self->start+self->size+1;
    }

    array_at_set( self->items, id, item );
    array_at_set( self->tags, id, 0 );
    self->size+= 1;
    return (id-self->start);
}

BmCode* BmBench_detachLast( BmBench* self )
{
    assert( self->size > 0 );
    uint id= self->start+self->size;
    self->size-= 1;
    return array_at( self->items, id );
}

BmCode* BmBench_at_tag( BmBench* self, uint i, uint tagValue )
{
    array_at_set( self->tags, self->start+i, tagValue );
    return array_at( self->items, self->start+i );
}

void BmBench_sortOnItem(BmBench* self)
{
    bool searching= true;
    while( searching )
    {
        searching= false;
        for( uint iItem= 2 ; iItem <= self->size ; ++iItem )
        {
            uint id2= self->start+iItem;
            uint id1= id2-1;
            if( BmCode_isGreaterThan( array_at(self->items, id1), array_at(self->items, id2) ) )
            {
                BmCode* tmp= array_at(self->items, id2);
                uint tag= array_at(self->tags, id2);
                array_at_set( self->items, id2, array_at( self->items, id1 ) );
                array_at_set( self->tags, id2, array_at( self->tags, id1 ) );
                array_at_set( self->items, id1, tmp );
                array_at_set( self->tags, id1, tag );
                searching= true;
            }
        }
    }
}

void BmBench_sortOnTag(BmBench* self)
{
    bool searching= true;
    while( searching )
    {
        searching= false;
        for( uint iItem= 2 ; iItem <= self->size ; ++iItem )
        {
            uint id2= self->start+iItem;
            uint id1= id2-1;
            if( array_at(self->tags, id1) > array_at(self->tags, id2) )
            {
                BmCode* tmp= array_at(self->items, id2);
                uint tag= array_at(self->tags, id2);
                array_at_set( self->items, id2, array_at( self->items, id1 ) );
                array_at_set( self->tags, id2, array_at( self->tags, id1 ) );
                array_at_set( self->items, id1, tmp );
                array_at_set( self->tags, id1, tag );
                searching= true;
            }
        }
    }
}

/* Printing */
char* BmBench_printItem(BmBench* self, uint iItem, char* output)
{
    BmCode_print( BmBench_at(self, iItem), output);
    if( BmBench_tagAt(self, iItem) == 0 )
        return output;
    strcat(output, ":");
    char buffer[16]= "";
    sprintf( buffer, "%u", BmBench_tagAt(self, iItem) );
    strcat( output, buffer );
    return output;
}

char* BmBench_print(BmBench* self, char* output)
{
    strcat(output, "{");

    if( self->size > 0 )
    {
        BmBench_printItem( self, 1, output );
    }

    for( uint i= 2 ; i <= self->size ; ++i)
    {
        strcat(output, ", ");
        BmBench_printItem( self, i, output );
    }

    strcat(output, "}");

    return output;
}
