#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   V A L U E  S :  V E C T O R 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor Destructor */
BmVector* newBmVector( digit dimention )
{
    return BmVector_create( newEmpty(BmVector), dimention );
}

BmVector* newBmVector_values( digit dimention, double* values )
{
    return BmVector_create_values( newEmpty(BmVector), dimention, values );
}

BmVector* newBmVector_all( digit dimention, double value )
{
    return BmVector_create_all( newEmpty(BmVector), dimention, value );
}

BmVector* newBmVectorAs( BmVector* model )
{
    return BmVector_createAs( newEmpty(BmVector), model );
}

BmVector* BmVector_create( BmVector* self, digit dimention )
{
    self->values= newEmptyArray(double, dimention);
    self->dimention= dimention;
    return self;
}

BmVector* BmVector_create_values( BmVector* self, digit dimention, double* values )
{
    BmVector_create(self, dimention);
    for( digit i=0 ; i < dimention ; ++i )
        array_at_set( self->values, i+1, values[i] );
    return self;
}

BmVector* BmVector_create_all( BmVector* self, digit dimention, double value )
{
    BmVector_create(self, dimention);
    for( digit i=0 ; i < dimention ; ++i )
        array_at_set( self->values, i+1, value );
    return self;
}

BmVector* BmVector_createAs( BmVector* self, BmVector* model )
{
    digit dimention= BmVector_dimention(model);
    BmVector_create(self, dimention);
    for( digit i=1 ; i <= dimention ; ++i )
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
BmVector* BmVector_reinit( BmVector* self, digit newDimention )
{
    BmVector_destroy( self );
    BmVector_create( self, newDimention );
    return self;
}

BmVector* BmVector_copy( BmVector* self, BmVector* model )
{
    BmVector_destroy( self );
    BmVector_create_values( self, BmVector_dimention(model), model->values );
    return self;
}

/* Accessor */
digit BmVector_dimention(BmVector* self)
{
    return self->dimention;
}

double BmVector_value(BmVector* self, digit i)
{
    return array_at( self->values, i );
}

/* Construction */
BmVector* BmVector_redimention(BmVector* self, digit dimention)
{
    // Allocate new memory
    double *newValues= newEmptyArray(double, dimention);

    // Copy
    digit boundeddimention= BmVector_dimention(self);
    if ( dimention < boundeddimention )
        boundeddimention= dimention;
    
    for( digit i = 1 ; i <= boundeddimention ; ++i )
    {
        array_at_set( newValues, i, BmVector_value(self, i) );
    }

    // and initialize to 0
    for( digit i = boundeddimention+1 ; i <= dimention ; ++i )
    {
        array_at_set( newValues, i, 0 );
    }
    
    // Update the structure:
    deleteEmptyArray( self->values );
    self->values= newValues;
    self->dimention= dimention;

    return self;
}

double BmVector_at_set(BmVector* self, digit i, double value)
{
    array_at_set( self->values, i, value );
    return array_at( self->values, i );
}

BmVector* BmVector_setValues( BmVector* self, double* values )
{
    digit dimention= BmVector_dimention( self );
    for( digit i= 0 ; i < dimention ; ++i )
        array_at_set( self->values, i+1, values[i] );
    return self;
}

/* Operation */
double BmVector_sum( BmVector* self )
{
    double result= 0;
    for( digit i= 1 ; i <= self->dimention ; ++i )
    {
        result+= BmVector_value( self, i );
    }
    return result;
}

double BmVector_product( BmVector* self )
{
    double result= 1;
    for( digit i= 1 ; i <= self->dimention ; ++i )
        result*= BmVector_value( self, i );
    return result;
}

/* Test */
bool BmVector_isEqualTo( BmVector* self, BmVector* another )
{
    bool eq= (BmVector_dimention(self) == BmVector_dimention(another));
    for( digit i= 1 ; eq && i <= BmVector_dimention(self) ; ++i )
    {
        eq= (BmVector_value(self, i) == BmVector_value(another, i));
    }
    return eq;
}

bool BmVector_isGreaterThan( BmVector* self, BmVector* another )
{
    digit mindimention= (BmVector_dimention(self) < BmVector_dimention(another) ? BmVector_dimention(self) : BmVector_dimention(another));
    for( digit i= 1 ; i <= mindimention ; ++i )
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
    digit mindimention= (BmVector_dimention(self) < BmVector_dimention(another) ? BmVector_dimention(self) : BmVector_dimention(another));
    for( digit i= 1 ; i <= mindimention ; ++i )
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
        
        for( digit i= 2 ; i <= BmVector_dimention(self) ; ++i)
        {
            sprintf( tmp, format, BmVector_value(self, i) );
            strcat(buffer, ", ");
            strcat(buffer, tmp );
        }
    }
    strcat(buffer, "]");
    return buffer;
}
