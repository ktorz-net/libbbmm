#include "bbmm.h"
#include "xtend-bbmm-variadic.h"
#include <stdarg.h>

BmCode* newBmCode_list(uint size, uint number1, ... )
{
    digit numbers[size];
    // Build words array from args
    va_list ap;
    numbers[0]= number1;
    va_start(ap, number1); 
    for ( uint i = 1 ; i < size ; ++i )
    {
        numbers[i]= (digit)(va_arg(ap, uint));
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
    for ( digit i = 1 ; i < numberOfCodes ; ++i )
    {
        codes[i]= va_arg(ap, BmCode*);
    }
    va_end(ap);

    // Create the instance
    return BmCode_createMerge(
        newEmpty(BmCode), numberOfCodes, codes );
}

BmCode* BmCode_reinit_list( BmCode* self, uint newSize, uint number1, ... )
{
    digit numbers[newSize];
    // Build words array from args
    // Build words array from args
    va_list ap;
    numbers[0]= number1;
    va_start(ap, number1); 
    for ( digit i = 1 ; i < newSize ; ++i )
    {
        numbers[i]= (digit)(va_arg(ap, uint));
    }
    va_end(ap);
    // initialize the instance
    BmCode_reinit(self, newSize );
    
    return BmCode_create_numbers(self, newSize, numbers);
}

BmVector* newBmVector_list( uint size, double val1, ... )
{
    double values[size];

    // Build words array from args
    va_list ap;
    values[0]= val1;
    va_start(ap, val1); 
    for ( digit i = 1 ; i < size ; ++i )
    {
        values[i]= va_arg(ap, double);
    }
    va_end(ap);

    // Create the instance
    return BmVector_create_values( newEmpty(BmVector), size, values );
}
