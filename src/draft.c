#include <stdio.h>
#include <string.h> 

int main()
{
    unsigned short us= 3042;
    printf( "uint size: %lu, exemple: %u, exa: %x \n", sizeof(unsigned short), us, us );

    unsigned int ui= 3042;
    printf( "uint size: %lu, exemple: %u, exa: %x \n", sizeof(unsigned int), ui, ui );

    unsigned int mask= 0xff000000;
    unsigned int dec= 3*8;
    for( unsigned int i= 0 ; i < 4 ; ++i )
    {
        printf( "\tbyte-%u: %u - %x (mask: %x)\n", 4-i, (ui&mask)>>dec, (ui&mask)>>dec, mask );
        mask= mask >> 8;
        dec-= 8;
    }

    unsigned int ui_max= (unsigned int)0-(unsigned int)1;
    printf( "\tmax: %u, exa: %x, +1: %u\n", ui_max, ui_max, ui_max+(unsigned int)(1) );

    long unsigned int lui= (long unsigned int)(4294967296)*(long unsigned int)(4294967296)-(long unsigned int)(1);
    printf( "long uint size: %lu, exemple %lu, exa %lx \n", sizeof(long unsigned int), lui, lui );
    printf( "void* size: %lu\n", sizeof(void*));

    void* dsc[6];
    printf( "void* size: %lu, x6: %lu\n", sizeof(void*), sizeof(dsc) );
    memset( dsc, 0, 48 );
    for( unsigned int i= 0 ; i < 6 ; ++i )
    {
        printf( "\tv-%u: %lu\n", i, (long unsigned int)(dsc[i]) );
    }

    return 0;
}
