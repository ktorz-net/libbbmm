#include "bbmm-structures.h"

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


BmCode* newBmCodeMergeList( uint numberOfCodes, BmCode* code1, ... )
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
    BmCode_destroy(instance);
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
    uint size= BmCode_dimention(model);
    self->dsc = malloc( sizeof(uint)*(size+1) );
    self->dsc[0]= size;
    for( uint i= 1 ; i <= size ; ++i )
        BmCode_at_set(self, i, BmCode_at(model, i));
    return self;
}


BmCode* BmCode_createMerge( BmCode* self, uint numberOfCodes, BmCode ** codes )
{
    // Final size:
    uint size= 0;
    for( uint iCode= 1 ; iCode <= numberOfCodes ; ++iCode )
        size+= BmCode_dimention( array_at(codes, iCode) );
    
    // Build the strucutre:
    self->dsc = malloc( sizeof(uint)*(size+1) );
    self->dsc[0]= size;

    // Fill with values:
    uint i= 1;
    for( uint iCode= 1 ; iCode <= numberOfCodes ; ++iCode )
    {
        BmCode* code= array_at( codes, iCode );
        size= BmCode_dimention( array_at(codes, iCode) );
        for( uint iVal= 1 ; iVal <= size ; ++iVal )
        {
            BmCode_at_set( self, i, BmCode_at(code, iVal ) );
            ++i;
        }
    }
    return self;
}

BmCode* BmCode_destroy(BmCode* self)
{
    free(self->dsc);
    return self;
}

/* Initializer */
BmCode* BmCode_reinit( BmCode* self, uint newSize )
{
    BmCode_destroy(self);
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
    BmCode_destroy(self);
    return BmCode_createAs(self, model);
}

BmCode* BmCode_copyNumbers(BmCode* self, BmCode* model)
{
    uint minSize= (BmCode_dimention(self) < BmCode_dimention(model))?BmCode_dimention(self):BmCode_dimention(model); 
    for( uint i= 1 ; i <= minSize ; ++i )
        BmCode_at_set( self, i, BmCode_at(model, i) );
    return self;
}

/* Getter */
uint BmCode_dimention(BmCode* self)
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
    assert( index <= BmCode_dimention(self) );
    return self->dsc[index];
}

uint BmCode_count(BmCode* self, uint value)
{
    uint count= 0;
    for( uint i=1 ; i <= BmCode_dimention(self) ; ++i )
        if( BmCode_at(self, i) == value )
            count+= 1;
    return count;
}

ulong BmCode_sum(BmCode* self)
{
    ulong r= 0;
    for( uint i=1 ; i <= BmCode_dimention(self) ; ++i )
        r+= (ulong)(BmCode_at(self, i));
    return r;
}

ulong BmCode_product(BmCode* self)
{
    ulong r= 1;
    for( uint i=1 ; i <= BmCode_dimention(self) ; ++i )
        r= r * (ulong)(BmCode_at(self, i));
    return r;
}

/* Test */
bool BmCode_isEqualTo(BmCode* self, BmCode* another)
{
    bool eq= (BmCode_dimention(self) == BmCode_dimention(another));
    for( uint i= 1 ; eq && i <= BmCode_dimention(self) ; ++i )
    {
        eq= (BmCode_at(self, i) == BmCode_at(another, i));
    }
    return eq;
}

bool BmCode_isGreaterThan(BmCode* self, BmCode* another)
{
    uint minSize= (BmCode_dimention(self) < BmCode_dimention(another) ? BmCode_dimention(self) : BmCode_dimention(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmCode_at(self, i) > BmCode_at(another, i) )
            return true;
        if( BmCode_at(self, i) < BmCode_at(another, i) )
            return false;
    }
    if ( BmCode_dimention(self) > BmCode_dimention(another) )
        return true;
    return false;
}

bool BmCode_isSmallerThan(BmCode* self, BmCode* another)
{
    uint minSize= (BmCode_dimention(self) < BmCode_dimention(another) ? BmCode_dimention(self) : BmCode_dimention(another));
    for( uint i= 1 ; i <= minSize ; ++i )
    {
        if( BmCode_at(self, i) < BmCode_at(another, i) )
            return true;
        if( BmCode_at(self, i) > BmCode_at(another, i) )
            return false;
    }
    if ( BmCode_dimention(self) < BmCode_dimention(another) )
        return true;
    return false;
}

/* Modification */
BmCode* BmCode_redimention(BmCode* self, uint newSize)
{
    // Allocate new memory
    uint *newDsc= malloc( sizeof(uint)*(newSize+1) );

    // Copy
    uint boundedSize= BmCode_dimention(self);
    if ( newSize < boundedSize )
        boundedSize= newSize;
    
    newDsc[0]= newSize;
    for( uint i = 1 ; i <= boundedSize ; ++i )
    {
        newDsc[i]= BmCode_at(self, i);
    }

    // and initialize to 0
    for( uint i = BmCode_dimention(self)+1 ; i <= newSize ; ++i )
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
    for( uint i= 1 ; i <= BmCode_dimention(self) ; ++i )
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
    for( uint i= 1 ; i <= BmCode_dimention(self) ; ++i )
        BmCode_at_set( self, i, numbers[i-1] );
    return self;
}

void BmCode_sort(BmCode* self)
{
    bool searching= true;
    while( searching )
    {
        searching= false;
        for( uint i= BmCode_dimention(self) ; i > 1 ; --i )
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
    for( uint i= 1 ; i <= BmCode_dimention(self) ; ++i )
        if( self->dsc[i] == value )
            return i;
    return 0;
}

/* code and key managment in set (ranges) */
ulong BmCode_keyOf(BmCode* ranges, BmCode* code)
{
    assert( BmCode_dimention(ranges) == BmCode_dimention(code) );
    
	ulong key= 0;
    ulong size= 1;
    
    for( uint i = 1 ; i <= BmCode_dimention(ranges) ; ++i )
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
    assert( BmCode_dimention(ranges) == BmCode_dimention(code) );

    if( key == 0 )
    {
        BmCode_setAll(code, 0 );
        return code;
    }

	ulong iKey= key-1;

    for( uint i = 1 ; i <= BmCode_dimention(ranges) ; ++i )
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
    assert( BmCode_dimention(ranges) == BmCode_dimention(code) );
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
    BmCode * code= newBmCode( BmCode_dimention(ranges) );
    BmCode_setCode_onKey( ranges, code, key );
    return code;
}

BmCode* BmCode_newBmCodeFirst(BmCode* ranges)
{
    return newBmCode_all( BmCode_dimention(ranges), 1 );
}

BmCode* BmCode_newBmCodeLast(BmCode* ranges)
{
    return newBmCodeAs( ranges );
}

BmCode* BmCode_nextCode(BmCode* ranges, BmCode* code)
{
    uint i = 1;
    uint size= BmCode_dimention(ranges);
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
    uint size= BmCode_dimention(ranges);
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
    uint size= BmCode_dimention(ranges);
    for( uint i= 1 ; i <= size ; ++i )
    {
        ok= ok && BmCode_at(code, i) > 0 && (BmCode_at(code, i) <= BmCode_at(ranges, i));
    }
    return ok;
}

/* mask */
BmCode* BmCode_newBmCodeMask(BmCode* self, BmCode* mask)
{   
    uint maskSize= BmCode_dimention(mask);
    assert( maskSize <= BmCode_dimention(self) );
    BmCode* code = newBmCode( BmCode_dimention(mask) );

    for( uint i= 1 ; i <= maskSize ; ++i )
            BmCode_at_set( code, i,
                BmCode_at( self, BmCode_at(mask, i) ) );
    
    return code;
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

    if( BmCode_dimention(self) > 0 )
    {
        sprintf( tmp, "%u", BmCode_at(self, 1) );
        strcat(buffer, tmp );
        
        for( uint i= 2 ; i <= BmCode_dimention(self) ; ++i)
        {
            sprintf( tmp, "%u", BmCode_at(self, i) );
            strcat(buffer, ", ");
            strcat(buffer, tmp );
        }
    }
    strcat(buffer, "]");
    return buffer;
}
