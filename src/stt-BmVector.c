#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   V A L U E  S :  V E C T O R 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor Destructor */
BmVector* newBmVector( uint size )
{
    return BmVector_create( newEmpty(BmVector), size );
}

BmVector* newBmVector_values( uint size, double* values )
{
    return BmVector_create_values( newEmpty(BmVector), size, values );
}

BmVector* newBmVector_all( uint size, double value )
{
    return BmVector_create_all( newEmpty(BmVector), size, value );
}

BmVector* newBmVectorAs( BmVector* model )
{
    return BmVector_createAs( newEmpty(BmVector), model );
}

BmVector* BmVector_create( BmVector* self, uint size )
{
    self->values= newEmptyArray(double, size);
    self->size= size;
    return self;
}

BmVector* BmVector_create_values( BmVector* self, uint size, double* values )
{
    BmVector_create(self, size);
    for( uint i=0 ; i < size ; ++i )
        array_at_set( self->values, i+1, values[i] );
    return self;
}

BmVector* BmVector_create_all( BmVector* self, uint size, double value )
{
    BmVector_create(self, size);
    for( uint i=0 ; i < size ; ++i )
        array_at_set( self->values, i+1, value );
    return self;
}

BmVector* BmVector_createAs( BmVector* self, BmVector* model )
{
    uint size= BmVector_dimention(model);
    BmVector_create(self, size);
    for( uint i=1 ; i <= size ; ++i )
        array_at_set( self->values, i, BmVector_value(model, i) );
    return self;
}

BmVector* BmVector_destroy( BmVector* self )
{
    deleteEmptyArray( self->values );
    return self;
}

void deleteBmVector( BmVector* self )
{
    free( BmVector_destroy(self) );
}

/* Re-Initialize */
BmVector* BmVector_reinit( BmVector* self, uint newSize )
{
    BmVector_destroy( self );
    BmVector_create( self, newSize );
    return self;
}

BmVector* BmVector_copy( BmVector* self, BmVector* model )
{
    BmVector_destroy( self );
    BmVector_create_values( self, BmVector_dimention(model), model->values );
    return self;
}

/* Accessor */
uint BmVector_dimention(BmVector* self)
{
    return self->size;
}

double BmVector_value(BmVector* self, uint i)
{
    return array_at( self->values, i );
}

/* Construction */
BmVector* BmVector_redimention(BmVector* self, uint size)
{
    // Allocate new memory
    double *newValues= newEmptyArray(double, size);

    // Copy
    uint boundedSize= BmVector_dimention(self);
    if ( size < boundedSize )
        boundedSize= size;
    
    for( uint i = 1 ; i <= boundedSize ; ++i )
    {
        array_at_set( newValues, i, BmVector_value(self, i) );
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

double BmVector_at_set(BmVector* self, uint i, double value)
{
    array_at_set( self->values, i, value );
    return array_at( self->values, i );
}

BmVector* BmVector_setValues( BmVector* self, double* values )
{
    uint size= BmVector_dimention( self );
    for( uint i= 0 ; i < size ; ++i )
        array_at_set( self->values, i+1, values[i] );
    return self;
}

/* Operation */
double BmVector_sum( BmVector* self )
{
    double result= 0;
    for( uint i= 1 ; i <= self->size ; ++i )
    {
        result+= BmVector_value( self, i );
    }
    return result;
}

double BmVector_product( BmVector* self )
{
    double result= 1;
    for( uint i= 1 ; i <= self->size ; ++i )
        result*= BmVector_value( self, i );
    return result;
}

/* Test */
bool BmVector_isEqualTo( BmVector* self, BmVector* another )
{
    bool eq= (BmVector_dimention(self) == BmVector_dimention(another));
    for( uint i= 1 ; eq && i <= BmVector_dimention(self) ; ++i )
    {
        eq= (BmVector_value(self, i) == BmVector_value(another, i));
    }
    return eq;
}

bool BmVector_isGreaterThan( BmVector* self, BmVector* another )
{
    uint minSize= (BmVector_dimention(self) < BmVector_dimention(another) ? BmVector_dimention(self) : BmVector_dimention(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmVector_value(self, i) > BmVector_value(another, i) )
            return true;
        if( BmVector_value(self, i) < BmVector_value(another, i) )
            return false;
    }
    if ( BmVector_dimention(self) > BmVector_dimention(another) )
        return true;
    return false;
}


bool BmVector_isSmallerThan( BmVector* self, BmVector* another )
{
    uint minSize= (BmVector_dimention(self) < BmVector_dimention(another) ? BmVector_dimention(self) : BmVector_dimention(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmVector_value(self, i) < BmVector_value(another, i) )
            return true;
        if( BmVector_value(self, i) > BmVector_value(another, i) )
            return false;
    }
    if ( BmVector_dimention(self) < BmVector_dimention(another) )
        return true;
    return false;
}


/* Printing */
char* BmVector_print( BmVector* self, char* buffer)
{
    return BmVector_format_print( self, "%.2f", buffer );
}

char* BmVector_format_print( BmVector* self, char* format, char* buffer)
{
    char tmp[64];
    strcat(buffer, "[");

    if( BmVector_dimention(self) > 0 )
    {
        sprintf( tmp, format, BmVector_value(self, 1) );
        strcat(buffer, tmp );
        
        for( uint i= 2 ; i <= BmVector_dimention(self) ; ++i)
        {
            sprintf( tmp, format, BmVector_value(self, i) );
            strcat(buffer, ", ");
            strcat(buffer, tmp );
        }
    }
    strcat(buffer, "]");
    return buffer;
}
