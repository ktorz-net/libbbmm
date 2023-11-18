// BbMm
#include "bbmm-structures.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmCode_init)
{
    BmCode* code= newBmCode_all(3, 0);
    
    ck_assert_uint_eq( BmCode_dimention(code), 3);
    ck_assert_uint_eq( BmCode_at(code, 1), 0 );
    ck_assert_uint_eq( BmCode_at(code, 2), 0 );
    ck_assert_uint_eq( BmCode_at(code, 3), 0 );

    deleteBmCode(code);
}
END_TEST

START_TEST(test_BmCode_init2)
{
    uint numbers[11]= {2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2};
    BmCode* code= newBmCode_numbers(11, numbers);

    ck_assert_uint_eq( BmCode_dimention(code), 11);
    ck_assert_uint_eq( BmCode_at(code, 1),  2 );
    ck_assert_uint_eq( BmCode_at(code, 2),  3 );
    ck_assert_uint_eq( BmCode_at(code, 3),  4 );
    ck_assert_uint_eq( BmCode_at(code, 4),  5 );
    ck_assert_uint_eq( BmCode_at(code, 5),  6 );
    ck_assert_uint_eq( BmCode_at(code, 6),  7 );
    ck_assert_uint_eq( BmCode_at(code, 7),  8 );
    ck_assert_uint_eq( BmCode_at(code, 8),  9 );
    ck_assert_uint_eq( BmCode_at(code, 9),  0 );
    ck_assert_uint_eq( BmCode_at(code, 10),  1 );
    ck_assert_uint_eq( BmCode_at(code, 11),  2 );

    ck_assert_str_eq(
        BmCode_wording(code),
        "[2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2]"
    );
    
    uint numbersYes[11]= {2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2};
    BmCode* codeYes= newBmCode_numbers(11, numbersYes);

    ck_assert( BmCode_dimention(code) == BmCode_dimention(codeYes) );
    ck_assert( BmCode_isEqualTo(code, codeYes) );

    uint numbers1[11]= {2, 3, 4, 5, 3, 7, 8, 9, 0, 1, 2};
    BmCode* codeNo1= newBmCode_numbers(11, numbers1);;
    ck_assert( !BmCode_isEqualTo(code, codeNo1) );

    uint numbers2[10]= {2, 3, 4, 5, 6, 7, 8, 9, 0, 1};
    BmCode* codeNo2= newBmCode_numbers(10, numbers2);
    ck_assert( !BmCode_isEqualTo(code, codeNo2) );

    deleteBmCode(code);
}
END_TEST

START_TEST(test_BmCode_keys)
{
    uint numbers[8]= {4, 4, 4, 4, 6, 6, 6, 6};
    BmCode* domain= newBmCode_numbers(8, numbers);
    BmCode* code= newBmCode_all(8, 0);
    
    ck_assert_str_eq(
        BmCode_wording(code),
        "[0, 0, 0, 0, 0, 0, 0, 0]"
    );

    ck_assert_uint_eq( (uint)BmCode_product(domain), 4*4*4*4*6*6*6*6 );
    ck_assert_uint_eq( (uint)BmCode_keyOf(domain, code), 0 );

    BmCode_copy( code, domain );
    ck_assert_uint_eq( (uint)BmCode_keyOf(domain, code), (4*4*4*4*6*6*6*6) );

    // neutral code
    BmCode_setCode_onKey(domain, code, 0);
    ck_assert_str_eq(
        BmCode_wording(code),
        "[0, 0, 0, 0, 0, 0, 0, 0]"
    );

    ck_assert(1);
    BmCode_setCode_onKey(domain, code, 1);
    ck_assert_str_eq(
        BmCode_wording(code),
        "[1, 1, 1, 1, 1, 1, 1, 1]"
    );

    BmCode_setCode_onKey(domain, code, 118942);
    ck_assert_str_eq(
        BmCode_wording(code),
        "[2, 4, 2, 3, 3, 6, 1, 3]"
    );
    
    uint numbers1[8]= {2, 4, 2, 3, 3, 6, 1, 3};
    BmCode_reinit(code, 8);
    BmCode_setNumbers(code, numbers1);
    ck_assert_uint_eq( (uint)BmCode_keyOf(domain, code), 118942 );
    
    BmCode_reinit_list(code, 8, 3, 4, 4, 1, 2, 5, 6, 4);
    ck_assert_uint_eq( (uint)BmCode_keyOf(domain, code), 218431 );

    deleteBmCode(code);
    deleteBmCode(domain);
}
END_TEST


START_TEST(test_BmCode_iterate)
{
    BmCode* domain= BmCode_reinit_list( newBmCode(0), 3, 2, 4, 3 );
    BmCode* code= newBmCode_all(3, 0);
    
    ck_assert_str_eq(
        BmCode_wording(code),
        "[0, 0, 0]"
    );
    ck_assert( !BmCode_isIncluding(domain, code) );

    BmCode_setCodeFirst(domain, code); // set the code as a key value in given ranges
    ck_assert_str_eq(
        BmCode_wording(code),
        "[1, 1, 1]"
    );
    ck_assert( BmCode_isIncluding(domain, code) );

    BmCode_previousCode(domain, code);
    ck_assert( !BmCode_isIncluding(domain, code) );

    BmCode_setCodeLast(domain, code); // set the code as a key value in given ranges
    ck_assert_str_eq(
        BmCode_wording(code),
        "[2, 4, 3]"
    );
    ck_assert( BmCode_isIncluding(domain, code) );
    
    BmCode_nextCode(domain, code);
    ck_assert( !BmCode_isIncluding(domain, code) );
    
    BmCode_reinit_list(code, 3, 1, 2, 3);
    ck_assert_str_eq(
        BmCode_wording(code),
        "[1, 2, 3]"
    );

    BmCode_nextCode(domain, code);
    ck_assert_str_eq(
        BmCode_wording(code),
        "[2, 2, 3]"
    );
    
    BmCode_nextCode(domain, code);
    BmCode_nextCode(domain, code);
    BmCode_nextCode(domain, code);
    ck_assert_str_eq(
        BmCode_wording(code),
        "[1, 4, 3]"
    );

    BmCode_previousCode(domain, code);
    ck_assert_str_eq(
        BmCode_wording(code),
        "[2, 3, 3]"
    );

    BmCode_previousCode(domain, code);
    BmCode_previousCode(domain, code);
    ck_assert_str_eq(
        BmCode_wording(code),
        "[2, 2, 3]"
    );

    deleteBmCode(code);
    deleteBmCode(domain);
}
END_TEST

START_TEST(test_BmCode_test)
{
    BmCode* c1= newBmCode_list(3, 2, 2 ,3);
    BmCode* c1Bis= newBmCode_list(3, 2, 2 ,3);
    BmCode* c2= newBmCode_list(3, 1, 2 ,4);

    ck_assert( BmCode_isEqualTo( c1, c1Bis ) );
    ck_assert( !BmCode_isEqualTo( c1, c2 ) );

    ck_assert_str_eq(
        BmCode_wording(c1),
        "[2, 2, 3]"
    );
    ck_assert_str_eq(
        BmCode_wording(c2),
        "[1, 2, 4]"
    );
    
    ck_assert_uint_eq( BmCode_search(c1, 1), 0 );
    ck_assert_uint_eq( BmCode_search(c1, 2), 1 );
    ck_assert_uint_eq( BmCode_search(c1, 3), 3 );

    ck_assert_uint_eq( BmCode_search(c2, 1), 1 );
    ck_assert_uint_eq( BmCode_search(c2, 2), 2 );
    ck_assert_uint_eq( BmCode_search(c2, 3), 0 );

    ck_assert( BmCode_isGreaterThan( c1, c2 ) );
    ck_assert( BmCode_isLighterThan( c2, c1 ) );
    ck_assert( !BmCode_isLighterThan( c1Bis, c1 ) );

    deleteBmCode(c1);
    deleteBmCode(c2);
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmCode(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("stt-BmCode");

    tcase_add_test(tc, test_BmCode_init);
    tcase_add_test(tc, test_BmCode_init2);
    tcase_add_test(tc, test_BmCode_keys);
    tcase_add_test(tc, test_BmCode_iterate);
    tcase_add_test(tc, test_BmCode_test);

    return tc;
}
