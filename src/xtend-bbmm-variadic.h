#include "bbmm.h"
#include <stdarg.h>

#ifndef BBMM_VARIADIC_H
#define BBMM_VARIADIC_H

BmCode* newBmCode_list(uint size, uint number1, ... );
BmCode* newBmCodeMerge_list( uint numberOfCodes, BmCode* code1, ... );
BmCode* BmCode_reinit_list( BmCode* self, uint newSize, uint number1, ... );
BmVector* newBmVector_list( uint size, double val1, ... );

#endif // BBMM_VARIADIC_H