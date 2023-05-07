#include "bbmm-structures.h"

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S T R U C T U R E :  B A S I C   T O O L S              *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* buffers */

char wordBuffer[WORD_BUFFER_SIZE];
char lineBuffer[LINE_BUFFER_SIZE];
char textBuffer[TEXT_BUFFER_SIZE];

char* bm_wordBuffer()
{
    return wordBuffer;
}

char* bm_lineBuffer()
{
    return lineBuffer;
}

char* bm_textBuffer()
{
    return textBuffer;
}

/* double  manipulation */
char doubleBuffer[WORD_BUFFER_SIZE];

char* bm_printDouble1(double p, char* output)
{
    sprintf( doubleBuffer, "%f", p);
    int end= strlen(doubleBuffer) - SIGNIFICANT_DIGITS;
    int i;
    // Go to the '.' character:
    for( i=0 ; doubleBuffer[i] != '.' && i < end; ++i ) ;
    
    //Tronc the buffer:
    doubleBuffer[i+1+SIGNIFICANT_DIGITS]= 0; 

    strcat(output, doubleBuffer);
    return output;
}

char* bm_printDouble0(double p, char* output)
{
    if( p < 0.0000000000001 )
    {
        strcat(output, "0.0");
        return output;
    }

    sprintf( doubleBuffer, "%f", p);
    int end= strlen(doubleBuffer) - SIGNIFICANT_DIGITS;
    int i;
    // Go to the '.' character:
    for( i=0 ; doubleBuffer[i] != '.' && i < end; ++i ) ;
    // Find the first significant digit:
    for( ++i; doubleBuffer[i] == '0' && i < end; ++i ) ;
 
    //Tronc the buffer:
    doubleBuffer[i+SIGNIFICANT_DIGITS]= 0; 

    strcat(output, doubleBuffer);
    return output;
}

char* bm_printDouble(double p, char* output)
{
    if(  p >= 1.0 )
        return bm_printDouble1( p, output);
    return bm_printDouble0( p, output);
}
