#include "bbmm-values.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   V A L U E  S :  V E C T O R 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor Destructor */
BmVector* newBmVectorBasic( uint size )
{
    return BmVector_createBasic( newEmpty(BmVector), size );
}
BmVector* newBmVector_values( uint size, double* values )
{
    return BmVector_create_values( newEmpty(BmVector), size, values );
}

BmVector* newBmVector_list( uint size, double val1, ... )
{
    double values[size];

    // Build words array from args
    va_list ap;
    values[0]= val1;
    va_start(ap, val1); 
    for ( uint i = 1 ; i < size ; ++i )
    {
        values[i]= va_arg(ap, double);
    }
    va_end(ap);

    // Create the instance
    return BmVector_create_values( newEmpty(BmVector), size, values );
}

BmVector* BmVector_createBasic( BmVector* self, uint size )
{
    self->values= newEmptyArray(double, size);
    self->size= size;
    return self;
}

BmVector* BmVector_create_values( BmVector* self, uint size, double* values )
{
    BmVector_createBasic(self, size);
    for( uint i=0 ; i < size ; ++i )
        array_at_set( self->values, i+1, values[i] );
    return self;
}

BmVector* BmVector_distroy( BmVector* self )
{
    deleteEmptyArray( self->values );
    return self;
}

void deleteBmVector( BmVector* self )
{
    free( BmVector_distroy(self) );
}

/* Initialize */
BmVector* BmVector_resize(BmVector* self, uint size)
{
    // Allocate new memory
    double *newValues= newEmptyArray(double, size);

    // Copy
    uint boundedSize= BmVector_size(self);
    if ( size < boundedSize )
        boundedSize= size;
    
    for( uint i = 1 ; i <= boundedSize ; ++i )
    {
        array_at_set( newValues, i, BmVector_at(self, i) );
    }

    // and initialize to 0
    for( uint i = boundedSize+1 ; i <= size ; ++i )
    {
        array_at_set( newValues, i, 0 );
    }
    
    // Update the structure:
    deleteEmptyArray( self->values );
    self->values= newValues;
    self->size= size;

    return self;
}

/* Accessor */
uint BmVector_size(BmVector* self)
{
    return self->size;
}
double BmVector_at(BmVector* self, uint i)
{
    return array_at( self->values, i );
}

/* Construction */
double BmVector_at_set(BmVector* self, uint i, double value)
{
    array_at_set( self->values, i, value );
    return array_at( self->values, i );
}

/* Printing */
char* BmVector_print( BmVector* self, char* buffer)
{
    char tmp[64];
    strcat(buffer, "[");

    if( BmVector_size(self) > 0 )
    {
        sprintf( tmp, "%f", BmVector_at(self, 1) );
        strcat(buffer, tmp );
        
        for( uint i= 2 ; i <= BmVector_size(self) ; ++i)
        {
            sprintf( tmp, "%f", BmVector_at(self, i) );
            strcat(buffer, ", ");
            strcat(buffer, tmp );
        }
    }
    strcat(buffer, "]");
    return buffer;
}
