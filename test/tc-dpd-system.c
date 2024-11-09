// BbMm
#include "bbmm-test.h"

#include <stdio.h>

#ifndef uint
#define uint unsigned int
#endif

#ifndef ulong
#define ulong unsigned long
#endif

START_TEST(test_sizeof)
{
    ck_assert_uint_eq( sizeof(unsigned int), 4 );
    ck_assert_uint_eq( sizeof(long unsigned int), 8 );
    ck_assert_uint_eq( sizeof(double), 8 );
    ck_assert_uint_eq( sizeof(void*), 8 );
}

START_TEST(test_uint)
{
    uint ui= 3042;
    ck_assert_uint_eq( sizeof(ui), 4 );
    ck_assert_uint_eq( ui, 0x000be2 );
    // printf( "uint size: %lu, exemple: %u, exa: %x \n", sizeof(uint), ui, ui );
    
    uint mask= 0xff000000;
    uint dec= 3*8;

    uint refs[4]= { 0, 0, 11, 226};
    for( uint i= 0 ; i < 4 ; ++i )
    {
        ck_assert_uint_eq( (ui&mask)>>dec, refs[i] );
        mask= mask >> 8;
        dec-= 8;
    }

    ck_assert_uint_eq( 11*0x100+226, 3042 );
}

START_TEST(test_array)
{
    void* dsc[16];
    ck_assert_uint_eq( sizeof(void*) * 16, 128 );
    ck_assert_uint_eq( sizeof(dsc), 128 );

    memset( dsc, 0, 128 );
    for( unsigned int i= 0 ; i < 6 ; ++i )
    {
        ck_assert_uint_eq( (ulong)dsc[i], 0 );
    }
}

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_system(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("dependencies");

    tcase_add_test(tc, test_sizeof);
    tcase_add_test(tc, test_uint);
    tcase_add_test(tc, test_array);

    return tc;
}
