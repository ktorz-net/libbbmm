#include "bbmm.h"

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

BmBench* newBmBench_codeDim_vectorDim( uint capacity, uint codeDim, uint vectorDim )
{
    return BmBench_create_codeDim_vectorDim( newEmpty(BmBench), capacity, codeDim, vectorDim );
}

BmBench* DEPRECIATED_newBmBenchWith( uint capacity, BmCode* newFirstItems, double value )
{
    BmBench* self= BmBench_create( newEmpty(BmBench), capacity);
    DEPRECIATED_BmBench_attachLast( self, newFirstItems, value );
    return self;
}

BmBench* newBmBenchAs( BmBench* model )
{
    return BmBench_createAs( newEmpty(BmBench), model );
}

void deleteBmBench(BmBench* self)
{
    BmBenchdestroy(self);
    free(self);
}

BmBench* BmBench_create(BmBench* self, uint capacity)
{
    return BmBench_create_codeDim_vectorDim( self, capacity, 1, 1 );
}

BmBench* BmBench_create_codeDim_vectorDim( BmBench* self, uint capacity, uint codeDim, uint vectorDim )
{
    self->capacity= capacity;
    self->codes= newEmptyArray( BmCode*, self->capacity );
    self->vects= newEmptyArray( BmVector*, self->capacity );
    self->start= 0;
    self->size= 0;
    self->codeDim= codeDim;
    self->vectDim= vectorDim;
    return self;
}

BmBench* BmBench_createAs( BmBench* self, BmBench* model )
{
    BmBench_create(self, BmBench_size(model) );
    for( uint i= 1 ; i <= BmBench_size(model) ; ++i )
    {
        BmBench_attachLast( self,
            newBmCodeAs( BmBench_codeAt( model, i ) ),
            newBmVectorAs( BmBench_vectorAt( model, i ) )
        );
    }
    return self;
}

BmBench* BmBenchdestroy(BmBench* self)
{
    while( self->size > 0 )
    {
        self->start+= 1;
        deleteBmCode( array_at( self->codes, self->start ) );
        deleteBmVector( array_at( self->vects, self->start ) );
        self->size-= 1;
    }
    deleteEmptyArray( self->vects );
    deleteEmptyArray( self->codes );
    return self;
}

/* Re-Initializer */
BmBench* BmBench_reinit( BmBench* self, uint capacity )
{
    BmBenchdestroy( self );
    BmBench_create( self, capacity );
    return self;
}

/* Modification */
void BmBench_resizeCapacity( BmBench* self, uint newCapacity )
{
    // Security:
    assert( newCapacity >= self->size );

    // Allocate new memory
    uint start= (newCapacity-self->size)/2;
    BmCode   ** newCodes= newEmptyArray( BmCode*,   newCapacity );
    BmVector ** newVects= newEmptyArray( BmVector*, newCapacity );

    // Copy
    for( uint i = 1 ; i <= self->size ; ++i )
    {
        array_at_set( newCodes, start+i, array_at( self->codes, self->start+i ) );
        array_at_set( newVects, start+i, array_at( self->vects, self->start+i ) );
    }
    
    // Update the structure:
    deleteEmptyArray( self->codes );
    deleteEmptyArray( self->vects );
    self->codes= newCodes;
    self->vects= newVects;
    self->capacity= newCapacity;
    self->start= start;
}

/* Accessor */
uint BmBench_size(BmBench* self)
{
    return self->size;
}

uint BmBench_capacity(BmBench* self)
{
    return self->capacity;
}

BmCode* BmBench_codeAt( BmBench* self, uint i )
{
    return array_at( self->codes, self->start+i );
}

BmVector* BmBench_vectorAt( BmBench* self, uint i )
{
    return array_at( self->vects, self->start+i );
}

uint BmBench_at_digit( BmBench* self, uint i, uint j )
{
    return BmCode_digit( BmBench_codeAt( self, i ), j );
}

double BmBench_at_value( BmBench* self, uint i, uint j )
{
    return BmVector_value( BmBench_vectorAt( self, i ), j );
}

double DEPRECIATED_BmBench_valueAt( BmBench* self, uint i )
{
    return BmVector_value( array_at( self->vects, self->start+i ), 1 );
}

uint BmBenchCodeDimention( BmBench* self)
{
    return self->codeDim;
}

uint BmBenchVectorDimention( BmBench* self)
{
    return self->vectDim;
}

/* Test */

/* Construction Item */
uint BmBench_attachCode( BmBench* self, BmCode* newItem )
{
    return DEPRECIATED_BmBench_attachLast( self, newItem, 0.0 );
}

uint BmBench_attachLast( BmBench* self, BmCode* newCode, BmVector* newVector )
{
    uint id= self->start+self->size+1;
    if( id > self->capacity )
    {
        BmBench_resizeCapacity( self, (self->size*2)+2);
        id= self->start+self->size+1;
    }

    array_at_set( self->codes, id, newCode );
    array_at_set( self->vects, id, newVector );

    self->size+= 1;
    return (id-self->start);    
}

uint DEPRECIATED_BmBench_attachLast( BmBench* self, BmCode* newItem, double value )
{
    return BmBench_attachLast( self, newItem, newBmVector_list(1, value) );
}

BmCode* BmBench_detachLast( BmBench* self )
{
    assert( self->size > 0 );
    uint id= self->start+self->size;
    self->size-= 1;
    return array_at( self->codes, id );
}

uint BmBench_attachFirst( BmBench* self, BmCode* newCode, BmVector* newVector )
{
    if( self->start == 0 )
    {
        BmBench_resizeCapacity( self, (self->size*2)+2);
    }
    array_at_set( self->codes, self->start, newCode );
    array_at_set( self->vects, self->start, newVector );

    self->start= self->start-1;
    self->size+= 1;
    return self->start+1;
}

BmCode* BmBench_detachFirst( BmBench* self )
{
    assert( self->size > 0 );
    uint id= self->start+1;
    self->size-= 1;
    self->start+=1;
    return array_at( self->codes, id );
}

BmCode* DEPRECIATED_BmBench_at_setValue( BmBench* self, uint i, double value )
{
    BmVector_at_set( 
        array_at( self->vects, self->start+i ),
        1, value
    );
    return array_at( self->codes, self->start+i );
}

void BmBench_switch( BmBench* self, BmBench* doppleganger)
{
    // Local:
    uint capacity    = self->capacity;
    uint start       = self->start;
    uint size        = self->size;
    uint codeDim     = self->codeDim;
    uint vectDim     = self->vectDim;
    BmCode** codes   = self->codes;
    BmVector** vects = self->vects;
    
    // Self:
    self->capacity = doppleganger->capacity;
    self->start    = doppleganger->start;
    self->size     = doppleganger->size;
    self->codeDim  = doppleganger->codeDim;
    self->vectDim  = doppleganger->vectDim;
    self->codes    = doppleganger->codes;
    self->vects    = doppleganger->vects;
    
    // Dopple:
    doppleganger->capacity = capacity;
    doppleganger->start    = start;
    doppleganger->size     = size;
    doppleganger->codeDim  = codeDim;
    doppleganger->vectDim  = vectDim;
    doppleganger->codes    = codes;
    doppleganger->vects    = vects;
}

void BmBench_add_reducted( BmBench *self, BmBench *another, BmCode* mask )
{
    uint dim= BmCodeDimention( mask );
    BmCode* state= newBmCode_all( dim, 0 );
    for( uint iCode = 1 ; iCode <= BmBench_size(another) ; ++iCode  )
    {
        BmCode* model= BmBench_codeAt( another, iCode );
        for( uint i= 1 ; i <= dim ; ++i )
        {
            BmCode_at_set( state, i, BmCode_digit(model, BmCode_digit( mask, i) ) );
        }
        DEPRECIATED_BmBench_attachLast( self, newBmCodeAs(state), DEPRECIATED_BmBench_valueAt( another, iCode)
        );
    }
    deleteBmCode( state );
}

/* Operators */
uint BmBench_sort( BmBench* self, fctptr_BmBench_compare compare )
{
    uint counter= 0;
    bool searching= true;
    while( searching )
    {
        searching= false;
        for( uint i2= 2 ; i2 <= self->size ; ++i2 )
        {
             if( (*compare)( self, i2-1, i2 ) )
            {
                counter+= BmBench_switchAt_at( self, i2-1, i2 );
                searching= true;
            }
        }
    }
    return counter;
}

uint BmBench_switchAt_at( BmBench* self, uint i1, uint i2 )
{
    uint id2= self->start+i1;
    uint id1= self->start+i2;

    // Local:
    BmCode*   codes= array_at(self->codes, id2);
    BmVector* vects= array_at(self->vects, id2);

    // id2:
    array_at_set( self->codes, id2, array_at( self->codes, id1 ) );
    array_at_set( self->vects, id2, array_at( self->vects, id1 ) );

    // id1:
    array_at_set( self->codes, id1, codes );
    array_at_set( self->vects, id1, vects );
    
    return 1;
}

/* Comparison */
bool BmBench_isCodeGreater(BmBench* self, uint i1, uint i2)
{
    return BmCode_isGreaterThan( BmBench_codeAt(self, i1), BmBench_codeAt(self, i2) );
}

bool BmBench_isCodeSmaller(BmBench* self, uint i1, uint i2)
{
    return BmCode_isSmallerThan( BmBench_codeAt(self, i1), BmBench_codeAt(self, i2) );
}

bool BmBench_isVectorGreater(BmBench* self, uint i1, uint i2)
{
    return DEPRECIATED_BmBench_valueAt(self, i1) > DEPRECIATED_BmBench_valueAt(self, i2);
}

bool BmBench_isVectorSmaller(BmBench* self, uint i1, uint i2)
{
    return DEPRECIATED_BmBench_valueAt(self, i1) < DEPRECIATED_BmBench_valueAt(self, i2);
}


/* Printing */
char* _BmBench_printItem(BmBench* self, uint iItem, char* output)
{
    char buffer[16]= "";
    BmCode_print( BmBench_codeAt(self, iItem), output);
    strcat(output, ":");
    sprintf( buffer, "%.2f", DEPRECIATED_BmBench_valueAt(self, iItem) );
    strcat( output, buffer );
    return output;
}

char* _BmBench_printNode(BmBench* self, uint iItem, char* output)
{
    char buffer[16]= "";
    sprintf( buffer, "%u", iItem );
    strcat( output, buffer );
    BmCode_print( BmBench_codeAt(self, iItem), output);
    return output;
}

char* BmBench_print(BmBench* self, char* output)
{
    strcat(output, "{");

    if( self->size > 0 )
    {
        _BmBench_printItem( self, 1, output );
    }

    for( uint i= 2 ; i <= self->size ; ++i)
    {
        strcat(output, ", ");
        _BmBench_printItem( self, i, output );
    }

    strcat(output, "}");

    return output;
}

char* BmBench_printNetwork(BmBench* self, char* output)
{
    if( self->size > 0 )
    {
        _BmBench_printNode( self, 1, output );
    }

    for( uint i= 2 ; i <= self->size ; ++i)
    {
        strcat(output, ", ");
        _BmBench_printNode( self, i, output );
    }
    return output;
}

char* BmBench_printCodes(BmBench* self, char* output)
{
    strcat(output, "{");

    if( self->size > 0 )
    {
        BmCode_print( BmBench_codeAt(self, 1), output);
    }

    for( uint i= 2 ; i <= self->size ; ++i)
    {
        strcat(output, ", ");
        BmCode_print( BmBench_codeAt(self, i), output);
    }

    strcat(output, "}");

    return output;
}
