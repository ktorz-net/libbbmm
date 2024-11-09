#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  C O D E                            *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor Destructor */
BmCode* newBmCode(digit size)
{
    return BmCode_create( newEmpty(BmCode), size );
}

BmCode* newBmCode_numbers( digit size, digit* numbers )
{
    return BmCode_create_numbers( newEmpty(BmCode), size, numbers );
}

BmCode* newBmCode_all(digit size, digit defaultValue)
{
    return BmCode_create_all( newEmpty(BmCode), size, defaultValue );
}

BmCode* newBmCodeAs( BmCode* model )
{
    return BmCode_createAs( newEmpty(BmCode), model );
}

void deleteBmCode(BmCode* instance)
{
    BmCode_destroy(instance);
    delEmpty(instance);
}

/* Protected - to use with precaution */
BmCode* BmCode_create( BmCode* self, digit size )
{
    self->dsc = malloc( sizeof(digit)*(size+1) );
    self->dsc[0]= size;
    return self;
}

BmCode* BmCode_create_numbers( BmCode* self, digit size, digit* numbers )
{
    BmCode_create(self, size);
    for( digit i= 1 ; i <= size ; ++i )
        BmCode_at_set( self, i, numbers[i-1] );
    return self;
}

BmCode* BmCode_create_all( BmCode* self, digit size, digit defaultValue )
{
    BmCode_create(self, size);
    BmCode_setAll(self, defaultValue);
    return self;
}

BmCode* BmCode_createAs(BmCode* self, BmCode* model)
{
    digit size= BmCode_dimention(model);
    self->dsc = malloc( sizeof(digit)*(size+1) );
    self->dsc[0]= size;
    for( digit i= 1 ; i <= size ; ++i )
        BmCode_at_set(self, i, BmCode_digit(model, i));
    return self;
}


BmCode* BmCode_createMerge( BmCode* self, digit numberOfCodes, BmCode ** codes )
{
    // Final size:
    digit size= 0;
    for( digit iCode= 1 ; iCode <= numberOfCodes ; ++iCode )
        size+= BmCode_dimention( array_at(codes, iCode) );
    
    // Build the strucutre:
    self->dsc = malloc( sizeof(digit)*(size+1) );
    self->dsc[0]= size;

    // Fill with values:
    digit i= 1;
    for( digit iCode= 1 ; iCode <= numberOfCodes ; ++iCode )
    {
        BmCode* code= array_at( codes, iCode );
        size= BmCode_dimention( array_at(codes, iCode) );
        for( digit iVal= 1 ; iVal <= size ; ++iVal )
        {
            BmCode_at_set( self, i, BmCode_digit(code, iVal ) );
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
BmCode* BmCode_reinit( BmCode* self, digit newSize )
{
    BmCode_destroy(self);
    return BmCode_create( self, newSize );
}

BmCode* BmCode_copy(BmCode* self, BmCode* model)
{
    BmCode_destroy(self);
    return BmCode_createAs(self, model);
}

BmCode* BmCode_copyNumbers(BmCode* self, BmCode* model)
{
    digit minSize= (BmCode_dimention(self) < BmCode_dimention(model))?BmCode_dimention(self):BmCode_dimention(model); 
    for( digit i= 1 ; i <= minSize ; ++i )
        BmCode_at_set( self, i, BmCode_digit(model, i) );
    return self;
}

/* Getter */
digit BmCode_dimention(BmCode* self)
{
    return self->dsc[0];
}

digit BmCode_digit(BmCode* self, digit index)
{
    assert( index > 0 );
    assert( index <= BmCode_dimention(self) );
    return self->dsc[index];
}

digit BmCode_count(BmCode* self, digit value)
{
    digit count= 0;
    for( digit i=1 ; i <= BmCode_dimention(self) ; ++i )
        if( BmCode_digit(self, i) == value )
            count+= 1;
    return count;
}

hash BmCode_sum(BmCode* self)
{
    hash r= 0;
    for( digit i=1 ; i <= BmCode_dimention(self) ; ++i )
        r+= (hash)(BmCode_digit(self, i));
    return r;
}

hash BmCode_product(BmCode* self)
{
    hash r= 1;
    for( digit i=1 ; i <= BmCode_dimention(self) ; ++i )
        r= r * (hash)(BmCode_digit(self, i));
    return r;
}

/* Test */
bool BmCode_isEqualTo(BmCode* self, BmCode* another)
{
    bool eq= (BmCode_dimention(self) == BmCode_dimention(another));
    for( digit i= 1 ; eq && i <= BmCode_dimention(self) ; ++i )
    {
        eq= (BmCode_digit(self, i) == BmCode_digit(another, i));
    }
    return eq;
}

bool BmCode_isGreaterThan(BmCode* self, BmCode* another)
{
    digit minSize= (BmCode_dimention(self) < BmCode_dimention(another) ? BmCode_dimention(self) : BmCode_dimention(another));
    for( digit i= 1 ; i <= minSize ; ++i )
    {
        if( BmCode_digit(self, i) > BmCode_digit(another, i) )
            return true;
        if( BmCode_digit(self, i) < BmCode_digit(another, i) )
            return false;
    }
    if ( BmCode_dimention(self) > BmCode_dimention(another) )
        return true;
    return false;
}

bool BmCode_isSmallerThan(BmCode* self, BmCode* another)
{
    digit minSize= (BmCode_dimention(self) < BmCode_dimention(another) ? BmCode_dimention(self) : BmCode_dimention(another));
    for( digit i= 1 ; i <= minSize ; ++i )
    {
        if( BmCode_digit(self, i) < BmCode_digit(another, i) )
            return true;
        if( BmCode_digit(self, i) > BmCode_digit(another, i) )
            return false;
    }
    if ( BmCode_dimention(self) < BmCode_dimention(another) )
        return true;
    return false;
}

/* Modification */
BmCode* BmCode_redimention(BmCode* self, digit newSize)
{
    // Allocate new memory
    digit *newDsc= malloc( sizeof(digit)*(newSize+1) );

    // Copy
    digit boundedSize= BmCode_dimention(self);
    if ( newSize < boundedSize )
        boundedSize= newSize;
    
    newDsc[0]= newSize;
    for( digit i = 1 ; i <= boundedSize ; ++i )
    {
        newDsc[i]= BmCode_digit(self, i);
    }

    // and initialize to 0
    for( digit i = BmCode_dimention(self)+1 ; i <= newSize ; ++i )
    {
        newDsc[i]= 0;
    }
    
    // Update the structure:
    free(self->dsc);
    self->dsc= newDsc;

    return self;
}

BmCode* BmCode_setAll(BmCode* self, digit value )
{
    for( digit i= 1 ; i <= BmCode_dimention(self) ; ++i )
    {
        BmCode_at_set(self, i, value);
    }
    return self;
}

BmCode* BmCode_at_set(BmCode* self, digit index, digit value )
{
    assert( index > 0 );
    self->dsc[index]= value;
    return self;
}

BmCode* BmCode_at_increment(BmCode* self, digit index, digit value )
{
    self->dsc[index]+= value;
    return self;
}

BmCode* BmCode_at_decrement(BmCode* self, digit index, digit value )
{
    self->dsc[index]-= value;
    return self;
}

BmCode* BmCode_setNumbers(BmCode* self, digit* numbers )
{
    for( digit i= 1 ; i <= BmCode_dimention(self) ; ++i )
        BmCode_at_set( self, i, numbers[i-1] );
    return self;
}

void BmCode_sort(BmCode* self)
{
    bool searching= true;
    while( searching )
    {
        searching= false;
        for( digit i= BmCode_dimention(self) ; i > 1 ; --i )
            if( self->dsc[i] < self->dsc[i-1] )
        {
            digit buf= self->dsc[i];
            self->dsc[i]= self->dsc[i-1];
            self->dsc[i-1]= buf;
            searching= true;
        }
    }
}

void BmCode_switch( BmCode* self, BmCode* anotherCode )
{
    digit* bufDsc= self->dsc;
    self->dsc= anotherCode->dsc;
    anotherCode->dsc= bufDsc;
}

digit BmCode_search( BmCode* self, digit value )
{
    for( digit i= 1 ; i <= BmCode_dimention(self) ; ++i )
        if( self->dsc[i] == value )
            return i;
    return 0;
}

/* code and key managment in set (ranges) */
hash BmCode_keyOf(BmCode* ranges, BmCode* code)
{
    assert( BmCode_dimention(ranges) == BmCode_dimention(code) );
    
	hash key= 0;
    hash size= 1;
    
    for( digit i = 1 ; i <= BmCode_dimention(ranges) ; ++i )
    {
        if( BmCode_digit(code, i) == 0 )
            return (hash)0;
		key += (BmCode_digit(code, i)-1) * size;
		size = size * BmCode_digit(ranges, i);
	}
	return key+1;
}

BmCode* BmCode_setCode_onKey(BmCode* ranges, BmCode* code, hash key)
{
    assert( BmCode_dimention(ranges) == BmCode_dimention(code) );

    if( key == 0 )
    {
        BmCode_setAll(code, 0 );
        return code;
    }

	hash iKey= key-1;

    for( digit i = 1 ; i <= BmCode_dimention(ranges) ; ++i )
    {
        digit s= BmCode_digit(ranges, i);
        BmCode_at_set(code, i, (iKey%s)+1);
        iKey-= (BmCode_digit(code, i)-1);
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

BmCode* BmCode_newBmCodeOnKey(BmCode* ranges, hash key)
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
    digit i = 1;
    digit size= BmCode_dimention(ranges);
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
    digit i = 1;
    digit size= BmCode_dimention(ranges);
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
    digit size= BmCode_dimention(ranges);
    for( digit i= 1 ; i <= size ; ++i )
    {
        ok= ok && BmCode_digit(code, i) > 0 && (BmCode_digit(code, i) <= BmCode_digit(ranges, i));
    }
    return ok;
}

/* mask */
BmCode* BmCode_newBmCodeMask(BmCode* self, BmCode* mask)
{   
    digit maskSize= BmCode_dimention(mask);
    assert( maskSize <= BmCode_dimention(self) );
    BmCode* code = newBmCode( BmCode_dimention(mask) );

    for( digit i= 1 ; i <= maskSize ; ++i )
            BmCode_at_set( code, i,
                BmCode_digit( self, BmCode_digit(mask, i) ) );
    
    return code;
}

/* Printing */
char* BmCode_print(BmCode* self, char* buffer)
{
    char tmp[64];
    strcat(buffer, "[");

    if( BmCode_dimention(self) > 0 )
    {
        sprintf( tmp, "%u", BmCode_digit(self, 1) );
        strcat(buffer, tmp );
        
        for( digit i= 2 ; i <= BmCode_dimention(self) ; ++i)
        {
            sprintf( tmp, "%u", BmCode_digit(self, i) );
            strcat(buffer, ", ");
            strcat(buffer, tmp );
        }
    }
    strcat(buffer, "]");
    return buffer;
}
