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
    uint size= BmVectorDimention(model);
    BmVector_create(self, size);
    for( uint i=1 ; i <= size ; ++i )
        array_at_set( self->values, i, BmVector_value(model, i) );
    return self;
}

BmVector* BmVectordestroy( BmVector* self )
{
    deleteEmptyArray( self->values );
    return self;
}

void deleteBmVector( BmVector* self )
{
    free( BmVectordestroy(self) );
}

/* Re-Initialize */
BmVector* BmVector_reinit( BmVector* self, uint newSize )
{
    BmVectordestroy( self );
    BmVector_create( self, newSize );
    return self;
}

BmVector* BmVector_copy( BmVector* self, BmVector* model )
{
    BmVectordestroy( self );
    BmVector_create_values( self, BmVectorDimention(model), model->values );
    return self;
}

/* Accessor */
uint BmVectorDimention(BmVector* self)
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
    uint boundedSize= BmVectorDimention(self);
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
    uint size= BmVectorDimention( self );
    for( uint i= 0 ; i < size ; ++i )
        array_at_set( self->values, i+1, values[i] );
    return self;
}

/* Operation */
double BmVectorSum( BmVector* self )
{
    double result= 0;
    for( uint i= 1 ; i <= self->size ; ++i )
    {
        result+= BmVector_value( self, i );
    }
    return result;
}

double BmVectorProduct( BmVector* self )
{
    double result= 1;
    for( uint i= 1 ; i <= self->size ; ++i )
        result*= BmVector_value( self, i );
    return result;
}

/* Test */
bool BmVector_isEqualTo( BmVector* self, BmVector* another )
{
    bool eq= (BmVectorDimention(self) == BmVectorDimention(another));
    for( uint i= 1 ; eq && i <= BmVectorDimention(self) ; ++i )
    {
        eq= (BmVector_value(self, i) == BmVector_value(another, i));
    }
    return eq;
}

bool BmVector_isGreaterThan( BmVector* self, BmVector* another )
{
    uint minSize= (BmVectorDimention(self) < BmVectorDimention(another) ? BmVectorDimention(self) : BmVectorDimention(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmVector_value(self, i) > BmVector_value(another, i) )
            return true;
        if( BmVector_value(self, i) < BmVector_value(another, i) )
            return false;
    }
    if ( BmVectorDimention(self) > BmVectorDimention(another) )
        return true;
    return false;
}


bool BmVector_isSmallerThan( BmVector* self, BmVector* another )
{
    uint minSize= (BmVectorDimention(self) < BmVectorDimention(another) ? BmVectorDimention(self) : BmVectorDimention(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmVector_value(self, i) < BmVector_value(another, i) )
            return true;
        if( BmVector_value(self, i) > BmVector_value(another, i) )
            return false;
    }
    if ( BmVectorDimention(self) < BmVectorDimention(another) )
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

    if( BmVectorDimention(self) > 0 )
    {
        sprintf( tmp, format, BmVector_value(self, 1) );
        strcat(buffer, tmp );
        
        for( uint i= 2 ; i <= BmVectorDimention(self) ; ++i)
        {
            sprintf( tmp, format, BmVector_value(self, i) );
            strcat(buffer, ", ");
            strcat(buffer, tmp );
        }
    }
    strcat(buffer, "]");
    return buffer;
}
