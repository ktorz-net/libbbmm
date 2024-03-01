#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  C O D E                            *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor Destructor */
BmCode* newBmCode(uint size)
{
    return BmCode_create( newEmpty(BmCode), size );
}

BmCode* newBmCode_numbers( uint size, uint* numbers )
{
    return BmCode_create_numbers( newEmpty(BmCode), size, numbers );
}

BmCode* newBmCode_all(uint size, uint defaultValue)
{
    return BmCode_create_all( newEmpty(BmCode), size, defaultValue );
}

BmCode* newBmCodeAs( BmCode* model )
{
    return BmCode_createAs( newEmpty(BmCode), model );
}

BmCode* newBmCode_list(uint size, uint number1, ... )
{
    uint numbers[size];
    // Build words array from args
    va_list ap;
    numbers[0]= number1;
    va_start(ap, number1); 
    for ( uint i = 1 ; i < size ; ++i )
    {
        numbers[i]= va_arg(ap, uint);
    }
    va_end(ap);
    // Create the instance
    return BmCode_create_numbers( newEmpty(BmCode), size, numbers );
}


BmCode* newBmCodeMerge_list( uint numberOfCodes, BmCode* code1, ... )
{
    BmCode* codes[numberOfCodes];

    // Build words array from args
    va_list ap;
    codes[0]= code1;
    va_start(ap, code1); 
    for ( uint i = 1 ; i < numberOfCodes ; ++i )
    {
        codes[i]= va_arg(ap, BmCode*);
    }
    va_end(ap);

    // Create the instance
    return BmCode_createMerge(
        newEmpty(BmCode), numberOfCodes, codes );
}


void deleteBmCode(BmCode* instance)
{
    BmCodedestroy(instance);
    delEmpty(instance);
}

/* Protected - to use with precaution */
BmCode* BmCode_create( BmCode* self, uint size )
{
    self->dsc = malloc( sizeof(uint)*(size+1) );
    self->dsc[0]= size;
    return self;
}

BmCode* BmCode_create_numbers( BmCode* self, uint size, uint* numbers )
{
    BmCode_create(self, size);
    for( uint i= 1 ; i <= size ; ++i )
        BmCode_at_set( self, i, numbers[i-1] );
    return self;
}

BmCode* BmCode_create_all( BmCode* self, uint size, uint defaultValue )
{
    BmCode_create(self, size);
    BmCode_setAll(self, defaultValue);
    return self;
}

BmCode* BmCode_createAs(BmCode* self, BmCode* model)
{
    uint size= BmCodeDimention(model);
    self->dsc = malloc( sizeof(uint)*(size+1) );
    self->dsc[0]= size;
    for( uint i= 1 ; i <= size ; ++i )
        BmCode_at_set(self, i, BmCode_digit(model, i));
    return self;
}


BmCode* BmCode_createMerge( BmCode* self, uint numberOfCodes, BmCode ** codes )
{
    // Final size:
    uint size= 0;
    for( uint iCode= 1 ; iCode <= numberOfCodes ; ++iCode )
        size+= BmCodeDimention( array_at(codes, iCode) );
    
    // Build the strucutre:
    self->dsc = malloc( sizeof(uint)*(size+1) );
    self->dsc[0]= size;

    // Fill with values:
    uint i= 1;
    for( uint iCode= 1 ; iCode <= numberOfCodes ; ++iCode )
    {
        BmCode* code= array_at( codes, iCode );
        size= BmCodeDimention( array_at(codes, iCode) );
        for( uint iVal= 1 ; iVal <= size ; ++iVal )
        {
            BmCode_at_set( self, i, BmCode_digit(code, iVal ) );
            ++i;
        }
    }
    return self;
}

BmCode* BmCodedestroy(BmCode* self)
{
    free(self->dsc);
    return self;
}

/* Initializer */
BmCode* BmCode_reinit( BmCode* self, uint newSize )
{
    BmCodedestroy(self);
    return BmCode_create( self, newSize );
}

BmCode* BmCode_reinit_list( BmCode* self, uint newSize, uint number1, ... )
{
    uint numbers[newSize];
    // Build words array from args
    // Build words array from args
    va_list ap;
    numbers[0]= number1;
    va_start(ap, number1); 
    for ( uint i = 1 ; i < newSize ; ++i )
    {
        numbers[i]= va_arg(ap, uint);
    }
    va_end(ap);
    // initialize the instance
    BmCode_reinit(self, newSize );
    
    return BmCode_create_numbers(self, newSize, numbers);
}

BmCode* BmCode_copy(BmCode* self, BmCode* model)
{
    BmCodedestroy(self);
    return BmCode_createAs(self, model);
}

BmCode* BmCode_copyNumbers(BmCode* self, BmCode* model)
{
    uint minSize= (BmCodeDimention(self) < BmCodeDimention(model))?BmCodeDimention(self):BmCodeDimention(model); 
    for( uint i= 1 ; i <= minSize ; ++i )
        BmCode_at_set( self, i, BmCode_digit(model, i) );
    return self;
}

/* Getter */
uint BmCodeDimention(BmCode* self)
{
    return self->dsc[0];
}

uint BmCode_digit(BmCode* self, uint index)
{
    assert( index > 0 );
    assert( index <= BmCodeDimention(self) );
    return self->dsc[index];
}

uint BmCode_count(BmCode* self, uint value)
{
    uint count= 0;
    for( uint i=1 ; i <= BmCodeDimention(self) ; ++i )
        if( BmCode_digit(self, i) == value )
            count+= 1;
    return count;
}

ulong BmCodeSum(BmCode* self)
{
    ulong r= 0;
    for( uint i=1 ; i <= BmCodeDimention(self) ; ++i )
        r+= (ulong)(BmCode_digit(self, i));
    return r;
}

ulong BmCodeProduct(BmCode* self)
{
    ulong r= 1;
    for( uint i=1 ; i <= BmCodeDimention(self) ; ++i )
        r= r * (ulong)(BmCode_digit(self, i));
    return r;
}

/* Test */
bool BmCode_isEqualTo(BmCode* self, BmCode* another)
{
    bool eq= (BmCodeDimention(self) == BmCodeDimention(another));
    for( uint i= 1 ; eq && i <= BmCodeDimention(self) ; ++i )
    {
        eq= (BmCode_digit(self, i) == BmCode_digit(another, i));
    }
    return eq;
}

bool BmCode_isGreaterThan(BmCode* self, BmCode* another)
{
    uint minSize= (BmCodeDimention(self) < BmCodeDimention(another) ? BmCodeDimention(self) : BmCodeDimention(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmCode_digit(self, i) > BmCode_digit(another, i) )
            return true;
        if( BmCode_digit(self, i) < BmCode_digit(another, i) )
            return false;
    }
    if ( BmCodeDimention(self) > BmCodeDimention(another) )
        return true;
    return false;
}

bool BmCode_isSmallerThan(BmCode* self, BmCode* another)
{
    uint minSize= (BmCodeDimention(self) < BmCodeDimention(another) ? BmCodeDimention(self) : BmCodeDimention(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmCode_digit(self, i) < BmCode_digit(another, i) )
            return true;
        if( BmCode_digit(self, i) > BmCode_digit(another, i) )
            return false;
    }
    if ( BmCodeDimention(self) < BmCodeDimention(another) )
        return true;
    return false;
}

/* Modification */
BmCode* BmCode_redimention(BmCode* self, uint newSize)
{
    // Allocate new memory
    uint *newDsc= malloc( sizeof(uint)*(newSize+1) );

    // Copy
    uint boundedSize= BmCodeDimention(self);
    if ( newSize < boundedSize )
        boundedSize= newSize;
    
    newDsc[0]= newSize;
    for( uint i = 1 ; i <= boundedSize ; ++i )
    {
        newDsc[i]= BmCode_digit(self, i);
    }

    // and initialize to 0
    for( uint i = BmCodeDimention(self)+1 ; i <= newSize ; ++i )
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
    for( uint i= 1 ; i <= BmCodeDimention(self) ; ++i )
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
    for( uint i= 1 ; i <= BmCodeDimention(self) ; ++i )
        BmCode_at_set( self, i, numbers[i-1] );
    return self;
}

void BmCodeSort(BmCode* self)
{
    bool searching= true;
    while( searching )
    {
        searching= false;
        for( uint i= BmCodeDimention(self) ; i > 1 ; --i )
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
    for( uint i= 1 ; i <= BmCodeDimention(self) ; ++i )
        if( self->dsc[i] == value )
            return i;
    return 0;
}

/* code and key managment in set (ranges) */
ulong BmCode_keyOf(BmCode* ranges, BmCode* code)
{
    assert( BmCodeDimention(ranges) == BmCodeDimention(code) );
    
	ulong key= 0;
    ulong size= 1;
    
    for( uint i = 1 ; i <= BmCodeDimention(ranges) ; ++i )
    {
        if( BmCode_digit(code, i) == 0 )
            return (ulong)0;
		key += (BmCode_digit(code, i)-1) * size;
		size = size * BmCode_digit(ranges, i);
	}
	return key+1;
}

BmCode* BmCode_setCode_onKey(BmCode* ranges, BmCode* code, ulong key)
{
    assert( BmCodeDimention(ranges) == BmCodeDimention(code) );

    if( key == 0 )
    {
        BmCode_setAll(code, 0 );
        return code;
    }

	ulong iKey= key-1;

    for( uint i = 1 ; i <= BmCodeDimention(ranges) ; ++i )
    {
        uint s= BmCode_digit(ranges, i);
        BmCode_at_set(code, i, (iKey%s)+1);
        iKey-= (BmCode_digit(code, i)-1);
        iKey= (iKey/s);
    }
    return code;
}

BmCode* BmCode_setCodeFirst(BmCode* ranges, BmCode* code)
{
    assert( BmCodeDimention(ranges) == BmCodeDimention(code) );
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
    BmCode * code= newBmCode( BmCodeDimention(ranges) );
    BmCode_setCode_onKey( ranges, code, key );
    return code;
}

BmCode* BmCodeNewBmCodeFirst(BmCode* ranges)
{
    return newBmCode_all( BmCodeDimention(ranges), 1 );
}

BmCode* BmCodeNewBmCodeLast(BmCode* ranges)
{
    return newBmCodeAs( ranges );
}

BmCode* BmCode_nextCode(BmCode* ranges, BmCode* code)
{
    uint i = 1;
    uint size= BmCodeDimention(ranges);
    while( i <= size )
    {
        BmCode_at_increment(code, i, 1);
        if( BmCode_digit(code, i) <= BmCode_digit(ranges, i) )
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
    uint size= BmCodeDimention(ranges);
    while( i <= size && BmCode_digit(code, i) == 1 )
    {
        BmCode_at_set( code, i, BmCode_digit(ranges, i) );
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
    uint size= BmCodeDimention(ranges);
    for( uint i= 1 ; i <= size ; ++i )
    {
        ok= ok && BmCode_digit(code, i) > 0 && (BmCode_digit(code, i) <= BmCode_digit(ranges, i));
    }
    return ok;
}

/* mask */
BmCode* BmCode_newBmCodeMask(BmCode* self, BmCode* mask)
{   
    uint maskSize= BmCodeDimention(mask);
    assert( maskSize <= BmCodeDimention(self) );
    BmCode* code = newBmCode( BmCodeDimention(mask) );

    for( uint i= 1 ; i <= maskSize ; ++i )
            BmCode_at_set( code, i,
                BmCode_digit( self, BmCode_digit(mask, i) ) );
    
    return code;
}

/* Printing */
char* BmCode_print(BmCode* self, char* buffer)
{
    char tmp[64];
    strcat(buffer, "[");

    if( BmCodeDimention(self) > 0 )
    {
        sprintf( tmp, "%u", BmCode_digit(self, 1) );
        strcat(buffer, tmp );
        
        for( uint i= 2 ; i <= BmCodeDimention(self) ; ++i)
        {
            sprintf( tmp, "%u", BmCode_digit(self, i) );
            strcat(buffer, ", ");
            strcat(buffer, tmp );
        }
    }
    strcat(buffer, "]");
    return buffer;
}
