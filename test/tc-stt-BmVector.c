// BbMm
#include "bbmm.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmVector_init01)
{
    BmVector* vector= newBmVector(3);
    
    ck_assert_uint_eq( BmVectorDimention(vector), 3);

    BmVector_at_set(vector, 1, 1.1);
    BmVector_at_set(vector, 2, 2.1);
    BmVector_at_set(vector, 3, 3.1);

    ck_assert_double_eq_tol( BmVector_at(vector, 1), 1.1, 0.00001 );
    ck_assert_double_eq_tol( BmVector_at(vector, 2), 2.1, 0.00001 );
    ck_assert_double_eq_tol( BmVector_at(vector, 3), 3.1, 0.00001 );

    deleteBmVector(vector);
}
END_TEST

START_TEST(test_BmVector_init02)
{
    BmVector* vector= newBmVector_list(3, 1.1,  2.1,  3.1 );
    
    ck_assert_uint_eq( BmVectorDimention(vector), 3);
    ck_assert_double_eq_tol( BmVector_at(vector, 1), 1.1, 0.00001 );
    ck_assert_double_eq_tol( BmVector_at(vector, 2), 2.1, 0.00001 );
    ck_assert_double_eq_tol( BmVector_at(vector, 3), 3.1, 0.00001 );

    deleteBmVector(vector);
}
END_TEST

START_TEST(test_BmVector_operation)
{
    BmVector* vector= newBmVector_list(3, 1.1,  2.1,  3.1 );
    
    ck_assert_double_eq_tol( BmVector_sum(vector), 1.1+2.1+3.1, 0.00001 );
    ck_assert_double_eq_tol( BmVector_product(vector), 1.1*2.1*3.1, 0.00001 );

    deleteBmVector(vector);
}
END_TEST

START_TEST(test_BmVector_print)
{
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmVector(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmVector");

    tcase_add_test(tc, test_BmVector_init01);
    tcase_add_test(tc, test_BmVector_init02);
    tcase_add_test(tc, test_BmVector_operation);
    tcase_add_test(tc, test_BmVector_print);
    
    return tc;
}