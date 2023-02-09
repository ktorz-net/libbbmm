#include "bemage-stt.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B e M A g e   S T R U C T U R E :  C O D E                            *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor Destructor */
//BmCode* newBasicBmCode()
//{
//    return newBmCode(1);
//}
//
//BmCode* newBmCode(uint size)
//{
//    return BmCode_createBasic( newEmpty(BmCode), size, 0 );
//}

BmCode* newBmCodeBasic(uint size, uint defaultValue)
{
    return BmCode_createBasic( newEmpty(BmCode), size, defaultValue );
}

BmCode* newBmCodeArray(uint size, uint* numbers)
{
    return BmCode_createArray( newEmpty(BmCode), size, numbers );
}

BmCode* newBmCodeAs(BmCode* model)
{
    return BmCode_createAs( newEmpty(BmCode), model );
}

BmCode* newBmCode(uint size, ...)
{
    uint numbers[size];
    // Build words array from args
    va_list ap;
    va_start(ap, size); 
    for ( uint i = 0 ; i < size ; ++i )
    {
        numbers[i]= va_arg(ap, uint);
    }
    va_end(ap);
    // Create the instance
    return BmCode_createArray( newEmpty(BmCode), size, numbers );
}

void deleteBmCode(BmCode* instance)
{
    BmCode_distroy(instance);
    delEmpty(instance);
}

/* Arrayed */
BmCode* newArrayOfBmCode(uint arraySize, uint codeSize)
{
    BmCode* array= newEmptyArray(BmCode, arraySize);
    for( uint i= 0 ; i < arraySize ; ++i )
        BmCode_createBasic( array+i, codeSize, 0 );
    return array;
};

void deleteArrayOfBmCode(BmCode* array, uint arraySize)
{
    for( uint i= 0 ; i < arraySize ; ++i )
        BmCode_distroy( array+i );
    delEmptyArray(array);
}

/* Protected - to use with precaution */
BmCode* BmCode_createBasic(BmCode* self, uint size, uint defaultValue)
{
    self->dsc = malloc( sizeof(uint)*(size+1) );
    self->dsc[0]= size;
    for( uint i= 1 ; i <= size ; ++i )
        BmCode_at_set(self, i, defaultValue);
    return self;
}

BmCode* BmCode_createArray(BmCode* self, uint size, uint* numbers)
{
    self->dsc = malloc( sizeof(uint)*(size+1) );
    self->dsc[0]= size;
    for( uint i= 1 ; i <= size ; ++i )
        BmCode_at_set(self, i, numbers[i-1]);
    return self;
}

BmCode* BmCode_createAs(BmCode* self, BmCode* model)
{
    uint size= BmCode_size(model);
    self->dsc = malloc( sizeof(uint)*(size+1) );
    self->dsc[0]= size;
    for( uint i= 1 ; i <= size ; ++i )
        BmCode_at_set(self, i, BmCode_at(model, i));
    return self;
}

BmCode* BmCode_distroy(BmCode* self)
{
    free(self->dsc);
    return self;
}

/* Initializer */
BmCode* WdCore_initializeBasic(BmCode* self, uint newSize)
{
    BmCode_distroy(self);
    return BmCode_createBasic( self, newSize, 0 );
}

BmCode* WdCore_initializeHomogene(BmCode* self, uint newSize, uint defaultRange)
{
    BmCode_distroy(self);
    return BmCode_createBasic( self, newSize, defaultRange );
}

BmCode* BmCode_initializeArray(BmCode* self, uint newSize, uint* numbers )
{
    BmCode_distroy(self);
    return BmCode_createArray( self, newSize, numbers );
    return self;
}

BmCode* BmCode_initialize(BmCode* self, uint newSize, ...)
{
    uint numbers[newSize];
    // Build words array from args
    va_list ap;
    va_start(ap, newSize); 
    for ( uint i = 0 ; i < newSize ; ++i )
    {
        numbers[i]= va_arg(ap, uint);
    }
    va_end(ap);
    // initialize the instance
    return BmCode_initializeArray(self, newSize, numbers);
}

BmCode* BmCode_initialize1(BmCode* self, uint v1)
{
    uint numbers[1]= {v1};
    BmCode_initializeArray(self, 1, numbers);
    return self;
}

BmCode* BmCode_initialize2(BmCode* self, uint v1, uint v2)
{
    uint numbers[2]= {v1, v2};
    BmCode_initializeArray(self, 2, numbers);
    return self;
}


BmCode* BmCode_initialize3(BmCode* self, uint v1, uint v2, uint v3)
{
    uint numbers[3]= {v1, v2, v3};
    BmCode_initializeArray(self, 3, numbers);
    return self;
}

BmCode* BmCode_initialize4(BmCode* self, uint v1, uint v2, uint v3, uint v4)
{
    uint numbers[4]= {v1, v2, v3, v4};
    BmCode_initializeArray(self, 4, numbers);
    return self;
}

BmCode* BmCode_copy(BmCode* self, BmCode* model)
{
    BmCode_distroy(self);
    return BmCode_createAs(self, model);
}

BmCode* BmCode_copyNumbers(BmCode* self, BmCode* model)
{
    uint minSize= (BmCode_size(self) < BmCode_size(model))?BmCode_size(self):BmCode_size(model); 
    for( uint i= 1 ; i <= minSize ; ++i )
        BmCode_at_set( self, i, BmCode_at(model, i) );
    return self;
}

/* Getter */
uint BmCode_size(BmCode* self)
{
    return self->dsc[0];
}

uint* BmCode_numbers(BmCode* self)
{
    return self->dsc+1;
}

uint BmCode_at(BmCode* self, uint index)
{
    assert( index > 0 );
    assert( index <= BmCode_size(self) );
    return self->dsc[index];
}

uint BmCode_count(BmCode* self, uint value)
{
    uint count= 0;
    for( uint i=1 ; i <= BmCode_size(self) ; ++i )
        if( BmCode_at(self, i) == value )
            count+= 1;
    return count;
}

ulong BmCode_sum(BmCode* self)
{
    ulong r= 0;
    for( uint i=1 ; i <= BmCode_size(self) ; ++i )
        r+= (ulong)(BmCode_at(self, i));
    return r;
}

ulong BmCode_product(BmCode* self)
{
    ulong r= 1;
    for( uint i=1 ; i <= BmCode_size(self) ; ++i )
        r= r * (ulong)(BmCode_at(self, i));
    return r;
}

/* Test */
bool BmCode_isEqualTo(BmCode* self, BmCode* another)
{
    bool eq= (BmCode_size(self) == BmCode_size(another));
    for( uint i= 1 ; eq && i <= BmCode_size(self) ; ++i )
    {
        eq= (BmCode_at(self, i) == BmCode_at(another, i));
    }
    return eq;
}

bool BmCode_isGreaterThan(BmCode* self, BmCode* another)
{
    uint minSize= (BmCode_size(self) < BmCode_size(another) ? BmCode_size(self) : BmCode_size(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmCode_at(self, i) > BmCode_at(another, i) )
            return true;
        if( BmCode_at(self, i) < BmCode_at(another, i) )
            return false;
    }
    if ( BmCode_size(self) > BmCode_size(another) )
        return true;
    return false;
}

bool BmCode_isLighterThan(BmCode* self, BmCode* another)
{
    uint minSize= (BmCode_size(self) < BmCode_size(another) ? BmCode_size(self) : BmCode_size(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmCode_at(self, i) < BmCode_at(another, i) )
            return true;
        if( BmCode_at(self, i) > BmCode_at(another, i) )
            return false;
    }
    if ( BmCode_size(self) < BmCode_size(another) )
        return true;
    return false;
}

/* Modification */
BmCode* BmCode_resize(BmCode* self, uint newSize)
{
    // Allocate new memory
    uint *newDsc= malloc( sizeof(uint)*(newSize+1) );

    // Copy
    uint boundedSize= BmCode_size(self);
    if ( newSize < boundedSize )
        boundedSize= newSize;
    
    newDsc[0]= newSize;
    for( uint i = 1 ; i <= boundedSize ; ++i )
    {
        newDsc[i]= BmCode_at(self, i);
    }

    // and initialize to 0
    for( uint i = BmCode_size(self)+1 ; i <= newSize ; ++i )
    {
        newDsc[i]= 0;
    }
    
    // Update the structure:
    free(self->dsc);
    self->dsc= newDsc;

    return self;
}

BmCode* BmCode_setAll(BmCode* self, uint value )
{
    for( uint i= 1 ; i <= BmCode_size(self) ; ++i )
    {
        BmCode_at_set(self, i, value);
    }
    return self;
}

BmCode* BmCode_at_set(BmCode* self, uint index, uint value )
{
    assert( index > 0 );
    self->dsc[index]= value;
    return self;
}

BmCode* BmCode_at_increment(BmCode* self, uint index, uint value )
{
    self->dsc[index]+= value;
    return self;
}

BmCode* BmCode_at_decrement(BmCode* self, uint index, uint value )
{
    self->dsc[index]-= value;
    return self;
}

BmCode* BmCode_setNumbers(BmCode* self, uint* numbers )
{
    for( uint i= 1 ; i <= BmCode_size(self) ; ++i )
        BmCode_at_set( self, i, numbers[i-1] );
    return self;
}

void BmCode_sort(BmCode* self)
{
    bool searching= true;
    while( searching )
    {
        searching= false;
        for( uint i= BmCode_size(self) ; i > 1 ; --i )
            if( self->dsc[i] < self->dsc[i-1] )
        {
            uint buf= self->dsc[i];
            self->dsc[i]= self->dsc[i-1];
            self->dsc[i-1]= buf;
            searching= true;
        }
    }
}

void BmCode_switch( BmCode* self, BmCode* anotherCode )
{
    uint* bufDsc= self->dsc;
    self->dsc= anotherCode->dsc;
    anotherCode->dsc= bufDsc;
}

uint BmCode_search( BmCode* self, uint value )
{
    for( uint i= 1 ; i <= BmCode_size(self) ; ++i )
        if( self->dsc[i] == value )
            return i;
    return 0;
}

/* code and key managment in set (ranges) */
ulong BmCode_keyOf(BmCode* ranges, BmCode* code)
{
    assert( BmCode_size(ranges) == BmCode_size(code) );
    
	ulong key= 0;
    ulong size= 1;
    
    for( uint i = 1 ; i <= BmCode_size(ranges) ; ++i )
    {
        if( BmCode_at(code, i) == 0 )
            return (ulong)0;
		key += (BmCode_at(code, i)-1) * size;
		size = size * BmCode_at(ranges, i);
	}
	return key+1;
}

BmCode* BmCode_setCode_onKey(BmCode* ranges, BmCode* code, ulong key)
{
    assert( BmCode_size(ranges) == BmCode_size(code) );

    if( key == 0 )
    {
        BmCode_setAll(code, 0 );
        return code;
    }

	ulong iKey= key-1;

    for( uint i = 1 ; i <= BmCode_size(ranges) ; ++i )
    {
        uint s= BmCode_at(ranges, i);
        BmCode_at_set(code, i, (iKey%s)+1);
        iKey-= (BmCode_at(code, i)-1);
        iKey= (iKey/s);
    }
    return code;
}

BmCode* BmCode_setCodeFirst(BmCode* ranges, BmCode* code)
{
    assert( BmCode_size(ranges) == BmCode_size(code) );
    BmCode_setAll( code, 1 );
    return code;
}

BmCode* BmCode_setCodeLast(BmCode* ranges, BmCode* code)
{
    BmCode_copy(code, ranges);
    return code;
}

BmCode* BmCode_newBmCodeOnKey(BmCode* ranges, ulong key)
{
    BmCode * code= newBmCodeBasic( BmCode_size(ranges), 0 );
    BmCode_setCode_onKey( ranges, code, key );
    return code;
}

BmCode* BmCode_newBmCodeFirst(BmCode* ranges)
{
    return newBmCodeBasic( BmCode_size(ranges), 1 );
}

BmCode* BmCode_newBmCodeLast(BmCode* ranges)
{
    return newBmCodeAs( ranges );
}

BmCode* BmCode_nextCode(BmCode* ranges, BmCode* code)
{
    uint i = 1;
    uint size= BmCode_size(ranges);
    while( i <= size )
    {
        BmCode_at_increment(code, i, 1);
        if( BmCode_at(code, i) <= BmCode_at(ranges, i) )
            break;
        BmCode_at_set(code, i, 1);
        ++i;
	}
    
    if( i > size )
    {
        BmCode_setAll(code, 0 );
    }

	return code;
}

BmCode* BmCode_previousCode(BmCode* ranges, BmCode* code)
{
    uint i = 1;
    uint size= BmCode_size(ranges);
    while( i <= size && BmCode_at(code, i) == 1 )
    {
        BmCode_at_set( code, i, BmCode_at(ranges, i) );
        ++i;
    }

    if( i > size )
    {
        BmCode_setAll(code, 0 );
        return code;
    }

    BmCode_at_decrement(code, i, 1);
	return code;
}

bool BmCode_isIncluding(BmCode* ranges, BmCode* code)
{
    bool ok= true;
    uint size= BmCode_size(ranges);
    for( uint i= 1 ; i <= size ; ++i )
    {
        ok= ok && BmCode_at(code, i) > 0 && (BmCode_at(code, i) <= BmCode_at(ranges, i));
    }
    return ok;
}

/* Printing */
char* BmCode_wording(BmCode* self)
{
    char* buffer= bm_wordBuffer();
    strcpy( buffer, "");
    return BmCode_print(self, buffer);
}

char* BmCode_print(BmCode* self, char* buffer)
{
    char tmp[64];
    strcat(buffer, "[");

    if( BmCode_size(self) > 0 )
    {
        sprintf( tmp, "%u", BmCode_at(self, 1) );
        strcat(buffer, tmp );
        
        for( uint i= 2 ; i <= BmCode_size(self) ; ++i)
        {
            sprintf( tmp, "%u", BmCode_at(self, i) );
            strcat(buffer, ", ");
            strcat(buffer, tmp );
        }
    }
    strcat(buffer, "]");
    return buffer;
}
