
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

void deleteDeepBmBench(BmBench* self)
{
    BmBench_distroy(self);
    free(self);
}


BmBench* BmBench_create(BmBench* self, uint capacity)
{
    self->capacity= capacity;
    self->items= newEmptyArray( BmCode*, self->capacity );
    self->tags= newEmptyArray( uint, self->capacity );
    self->size= 0;
    return self;
}
BmBench* BmBench_distroy(BmBench* self)
{
    delEmptyArray( self->tags );
    delEmptyArray( self->items );
    return self;
}

BmBench* BmBench_distroyDeep(BmBench* self)
{
    for( uint i= 1; i <= self->size; ++i )
    {
        deleteBmCode( array_at( self->items, i ) );
    }
    return BmBench_distroy(self);
}

/* Initializer */

/* Modification */
void BmBench_resizeCapacity( BmBench* self, uint newCapacity )
{
    // Security:
    assert( newCapacity >= self->size );

    // Allocate new memory
    BmCode ** newItems= newEmptyArray( BmCode*, newCapacity );
    uint* newTags= newEmptyArray( uint, newCapacity );

    // Copy
    for( uint i = 1 ; i <= self->size ; ++i )
    {
        array_at_set( newItems, i, array_at( self->items, i ) );
        array_at_set( newTags, i, array_at( self->tags, i ) );
    }
    
    // Update the structure:
    delEmptyArray( self->items );
    delEmptyArray( self->tags );
    self->items= newItems;
    self->tags= newTags;
    self->capacity= newCapacity;
}

/* Accessor */
uint BmBench_size(BmBench* self)
{
    return self->size;
}

BmCode* BmBench_at_item( BmBench* self, uint i )
{
    return array_at( self->items, i );
}

uint BmBench_at_tag( BmBench* self, uint i )
{
    return array_at( self->tags, i );
}


/* Test */

/* Construction Item */
uint BmBench_attachItem( BmBench* self, BmCode* item )
{
    if( self->capacity == self->size )
        BmBench_resizeCapacity( self, self->capacity+1+(self->capacity/2) );
 
    self->size+= 1;
    array_at_set( self->items, self->size, item );
    array_at_set( self->tags, self->size, 0 );
    return self->size;
}

BmCode* BmBench_at_setTag( BmBench* self, uint i, uint tagValue )
{
    array_at_set( self->tags, i, tagValue );
    return array_at( self->items, i );
}

void BmBench_sortOnItem(BmBench* self)
{
    bool searching= true;
    while( searching )
    {
        searching= false;
        for( uint i= 2 ; i <= self->size ; ++i )
            if( BmCode_isGreaterThan( array_at(self->items, i-1), array_at(self->items, i) ) )
        {
            BmCode* tmp= array_at(self->items, i);
            uint tag= array_at(self->tags, i);
            array_at_set( self->items, i, array_at( self->items, i-1 ) );
            array_at_set( self->tags, i, array_at( self->tags, i-1 ) );
            array_at_set( self->items, i-1, tmp );
            array_at_set( self->tags, i-1, tag );
            searching= true;
        }
    }
}

void BmBench_sortOnTag(BmBench* self)
{
    bool searching= true;
    while( searching )
    {
        searching= false;
        for( uint i= 2 ; i <= self->size ; ++i )
            if( array_at(self->tags, i-1) > array_at(self->tags, i) )
        {
            BmCode* tmp= array_at(self->items, i);
            uint tag= array_at(self->tags, i);
            array_at_set( self->items, i, array_at( self->items, i-1 ) );
            array_at_set( self->tags, i, array_at( self->tags, i-1 ) );
            array_at_set( self->items, i-1, tmp );
            array_at_set( self->tags, i-1, tag );
            searching= true;
        }
    }
}

/* Printing */
char* BmBench_printItem(BmBench* self, uint iItem, char* output)
{
    BmCode_print( array_at(self->items, iItem), output);
    if( BmBench_at_tag(self, iItem) == 0 )
        return output;
    strcat(output, ":");
    char buffer[16]= "";
    sprintf( buffer, "%u", BmBench_at_tag(self, iItem) );
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
