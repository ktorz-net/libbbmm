// BbMm
#include "bbmm-structures.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmBench_init)
{
    BmBench* collec= newBmBench(1);

    ck_assert_uint_eq( collec->capacity, 1 );
    ck_assert_uint_eq( collec->size, 0 );

    char buffer[1024];
    BmBench_attachLast(collec, newBmCode_list(1, 42), 0, 0.0 );

    ck_assert_uint_eq( collec->size, 1 );
    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[42]}" );

    BmBench_attachLast(collec, newBmCode_list(2, 69, 103), 0, 0.0 );
    
    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[42], [69, 103]}" );
    ck_assert_uint_eq( BmBench_size(collec), 2);
    ck_assert_uint_eq( collec->capacity, 4 );
    
    BmCode* c= newBmCode_list(1, 3);
    BmBench_attach(collec, c );

    ck_assert_uint_eq( collec->size, 3 );
    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[42], [69, 103], [3]}" );
    ck_assert_uint_eq( BmBench_size(collec), 3);
    ck_assert_uint_eq( collec->capacity, 6 );

    BmBench_attach(collec,  newBmCode_list(2, 69, 56) );
    ck_assert_uint_eq( collec->size, 4 );
    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[42], [69, 103], [3], [69, 56]}" );
    ck_assert_uint_eq( BmBench_size(collec), 4);
    ck_assert_uint_eq( collec->capacity, 8 );

    deleteBmBench(collec);
}
END_TEST

START_TEST(test_BmBench_sorting)
{
    BmBench* collec= newBmBench(4);

    BmBench_attach(collec, newBmCode_list(1, 42) );
    BmBench_attach(collec, newBmCode_list(2, 69, 103) );
    BmBench_attach(collec, newBmCode_list(1, 3) );
    BmBench_attach(collec,  newBmCode_list(2, 69, 56) );
    
    ck_assert_uint_eq( BmBench_size(collec), 4 );

    char buffer[1024];
    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[42], [69, 103], [3], [69, 56]}" );

    BmBench_sortOnItem(collec);

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[3], [42], [69, 56], [69, 103]}" );

    BmBench_destroy(collec);
    BmBench_create(collec, 8);

    BmBench_attach(collec, newBmCode_list(3, 5, 8, 12) );
    BmBench_attach(collec, newBmCode_list(3, 1, 2, 3) );
    BmBench_attach(collec, newBmCode_list(3, 1, 2, 1) );
    BmBench_attach(collec, newBmCode_list(3, 1, 1, 8) );

    BmBench_attach(collec, newBmCode_list(3, 4, 2, 1) );
    BmBench_attach(collec, newBmCode_list(3, 5, 2, 3) );
    BmBench_attach(collec, newBmCode_list(3, 2, 2, 2) );
    BmBench_attach(collec, newBmCode_list(3, 2, 3, 2) );

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[5, 8, 12], [1, 2, 3], [1, 2, 1], [1, 1, 8], [4, 2, 1], [5, 2, 3], [2, 2, 2], [2, 3, 2]}" );

    BmBench_sortOnItem(collec);

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[1, 1, 8], [1, 2, 1], [1, 2, 3], [2, 2, 2], [2, 3, 2], [4, 2, 1], [5, 2, 3], [5, 8, 12]}" );

    deleteBmBench(collec);
}
END_TEST

START_TEST(test_BmBench_tags)
{
    BmBench* collec= newBmBench(4);

    BmBench_attach(collec, newBmCode_list(1, 42) );
    BmBench_attach(collec, newBmCode_list(2, 69, 103) );
    BmBench_attach(collec, newBmCode_list(1, 3) );
    BmBench_attach(collec, newBmCode_list(2, 69, 56) );
    
    ck_assert_uint_eq( BmBench_size(collec), 4 );
    char buffer[1024];

    strcpy( buffer, "" );
    BmBench_printCodes(collec, buffer);
    ck_assert_str_eq( buffer, "{[42], [69, 103], [3], [69, 56]}" );

    ck_assert_uint_eq( BmBench_tagAt( collec, 1 ), 0 );

    strcpy( buffer, "" );
    BmBench_print(collec, buffer);
    ck_assert_str_eq( buffer, "{[42]:0:0.00, [69, 103]:0:0.00, [3]:0:0.00, [69, 56]:0:0.00}" );

    ck_assert_uint_eq( BmBench_tagAt( collec, 2 ), 0 );
    BmBench_at_tag( collec, 2, 1 );
    ck_assert_uint_eq( BmBench_tagAt( collec, 2 ), 1 );

    strcpy( buffer, "" );
    BmBench_print(collec, buffer);
    ck_assert_str_eq( buffer, "{[42]:0:0.00, [69, 103]:1:0.00, [3]:0:0.00, [69, 56]:0:0.00}" );

    BmBench_at_tag( collec, 1, 3 );
    BmBench_at_tag( collec, 3, 4 );
    BmBench_at_tag( collec, 4, 1 );

    strcpy( buffer, "" );
    BmBench_print(collec, buffer);
    ck_assert_str_eq( buffer, "{[42]:3:0.00, [69, 103]:1:0.00, [3]:4:0.00, [69, 56]:1:0.00}" );

    deleteBmBench(collec);
}
END_TEST

START_TEST(test_BmBench_values)
{
    BmBench* collec= newBmBench(4);

    BmBench_attach(collec, newBmCode_list(1, 42) );
    BmBench_attach(collec, newBmCode_list(2, 69, 103) );
    BmBench_attach(collec, newBmCode_list(1, 3) );
    BmBench_attach(collec, newBmCode_list(2, 69, 56) );
    
    ck_assert_uint_eq( BmBench_size(collec), 4 );
    char buffer[1024];

    strcpy( buffer, "" );
    BmBench_printCodes(collec, buffer);
    ck_assert_str_eq( buffer, "{[42], [69, 103], [3], [69, 56]}" );

    strcpy( buffer, "" );
    BmBench_print(collec, buffer);
    ck_assert_str_eq( buffer, "{[42]:0:0.00, [69, 103]:0:0.00, [3]:0:0.00, [69, 56]:0:0.00}" );

    ck_assert_uint_eq( BmBench_tagAt( collec, 2 ), 0 );
    BmBench_at_value( collec, 2, 1.1 );
    ck_assert_double_eq_tol( BmBench_valueAt( collec, 2 ), 1.1, 0.00001 );

    strcpy( buffer, "" );
    BmBench_print(collec, buffer);
    ck_assert_str_eq( buffer, "{[42]:0:0.00, [69, 103]:0:1.10, [3]:0:0.00, [69, 56]:0:0.00}" );

    BmBench_at_value( collec, 1, 3.3 );
    BmBench_at_value( collec, 3, 4.4 );
    BmBench_at_value( collec, 4, 1.1 );

    strcpy( buffer, "" );
    BmBench_print(collec, buffer);
    ck_assert_str_eq( buffer, "{[42]:0:3.30, [69, 103]:0:1.10, [3]:0:4.40, [69, 56]:0:1.10}" );

    deleteBmBench(collec);
}
END_TEST

START_TEST(test_BmBench_sortingWithTags)
{
    BmBench* collec= newBmBench(10);

    BmBench_attach(collec, newBmCode_list(3, 5, 8, 12) );
    BmBench_attach(collec, newBmCode_list(3, 1, 2, 3) );
    BmBench_attach(collec, newBmCode_list(3, 1, 2, 1) );
    BmBench_attach(collec, newBmCode_list(3, 1, 1, 8) );

    BmBench_attach(collec, newBmCode_list(3, 4, 2, 1) );
    BmBench_attach(collec, newBmCode_list(3, 5, 2, 3) );
    BmBench_attach(collec, newBmCode_list(3, 2, 2, 2) );
    BmBench_attach(collec, newBmCode_list(3, 2, 3, 2) );

    for( uint i=1 ; i <= 7 ; ++i )
        BmBench_at_tag( collec, i, i );
    
    char buffer[1024];
    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmBench_print(collec, buffer),
        "{[5, 8, 12]:1:0.00, [1, 2, 3]:2:0.00, [1, 2, 1]:3:0.00, [1, 1, 8]:4:0.00, [4, 2, 1]:5:0.00, [5, 2, 3]:6:0.00, [2, 2, 2]:7:0.00, [2, 3, 2]:0:0.00}" );

    BmBench_sortOnItem(collec);

    strcpy( buffer, "" );
    BmBench_print(collec, buffer);
    ck_assert_str_eq( buffer, "{[1, 1, 8]:4:0.00, [1, 2, 1]:3:0.00, [1, 2, 3]:2:0.00, [2, 2, 2]:7:0.00, [2, 3, 2]:0:0.00, [4, 2, 1]:5:0.00, [5, 2, 3]:6:0.00, [5, 8, 12]:1:0.00}" );

    strcpy( buffer, "" );
    BmBench_printTags(collec, buffer);
    ck_assert_str_eq( buffer, "{[1, 1, 8]:4, [1, 2, 1]:3, [1, 2, 3]:2, [2, 2, 2]:7, [2, 3, 2]:0, [4, 2, 1]:5, [5, 2, 3]:6, [5, 8, 12]:1}" );

    deleteBmBench(collec);
}
END_TEST

START_TEST(test_BmBench_attach_detach)
{
    BmBench* collec= newBmBench(10);

    BmBench_attachLast(collec, newBmCode_list(3, 5, 8, 12), 0, 0.0 );
    BmBench_attachLast(collec, newBmCode_list(3, 1, 2, 3), 0, 0.0 );

    char buffer[1024];
    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[5, 8, 12], [1, 2, 3]}" );

    BmBench_attachLast(collec, newBmCode_list(3, 1, 2, 1), 0, 0.0 );

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[5, 8, 12], [1, 2, 3], [1, 2, 1]}" );

    BmCode* bob= BmBench_detachLast(collec);
    deleteBmCode(bob);

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[5, 8, 12], [1, 2, 3]}" );

    bob= BmBench_detachLast(collec);
    deleteBmCode(bob);
    bob= BmBench_detachLast(collec);
    deleteBmCode(bob);

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{}" );

    BmBench_attachLast(collec, newBmCode_list(3, 4, 2, 1), 0, 0.0 );
    BmBench_attachLast(collec, newBmCode_list(3, 5, 2, 3), 0, 0.0 );
    BmBench_attachLast(collec, newBmCode_list(3, 2, 2, 2), 0, 0.0 );
    BmBench_attachLast(collec, newBmCode_list(3, 2, 3, 2), 0, 0.0 );

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[4, 2, 1], [5, 2, 3], [2, 2, 2], [2, 3, 2]}" );

    deleteBmBench(collec);
}
END_TEST


START_TEST(test_BmBench_attach_front)
{
    BmBench* collec= newBmBench(10);

    BmBench_attach(collec, newBmCode_list(3, 5, 8, 12) );
    BmBench_attach(collec, newBmCode_list(3, 1, 2, 3) );

    char buffer[1024];
    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[5, 8, 12], [1, 2, 3]}" );

    BmBench_attachLast(collec, newBmCode_list(3, 1, 2, 1), 0, 0.0 );

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[5, 8, 12], [1, 2, 3], [1, 2, 1]}" );

    BmCode* bob= BmBench_detachFirst(collec);
    deleteBmCode(bob);

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[1, 2, 3], [1, 2, 1]}" );

    bob= BmBench_detachFirst(collec);
    deleteBmCode(bob);
    bob= BmBench_detachFirst(collec);
    deleteBmCode(bob);

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{}" );

    BmBench_attachLast(collec, newBmCode_list(3, 4, 2, 1), 0, 0.0 );
    BmBench_attachLast(collec, newBmCode_list(3, 5, 2, 3), 0, 0.0 );
    BmBench_attachLast(collec, newBmCode_list(3, 2, 2, 2), 0, 0.0 );
    BmBench_attachFirst(collec, newBmCode_list(3, 2, 3, 2), 0, 0.0 );

    strcpy( buffer, "" );
    ck_assert_str_eq( BmBench_printCodes(collec, buffer), "{[2, 3, 2], [4, 2, 1], [5, 2, 3], [2, 2, 2]}" );

    deleteBmBench(collec);
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmBench(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("stt-BmBench");

    tcase_add_test(tc, test_BmBench_init);
    tcase_add_test(tc, test_BmBench_sorting);
    tcase_add_test(tc, test_BmBench_tags);
    tcase_add_test(tc, test_BmBench_values);
    tcase_add_test(tc, test_BmBench_sortingWithTags);
    tcase_add_test(tc, test_BmBench_attach_detach);
    tcase_add_test(tc, test_BmBench_attach_front);

    return tc;
}