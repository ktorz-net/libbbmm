// BbMm
#include "bbmm-mdl.h"
#include "bbmm-test.h"

#include <stdlib.h>
#include <stdio.h>

START_TEST(test_BmDomainBasic_init)
{
/*
    BmDomain * dom= newBmDomainBasic("Dom", 6);
    
    ck_assert_str_eq( dom->name, "Dom" );
    ck_assert_str_eq( BmDomain_name(dom), "Dom" );

    ck_assert( BmDomain_isType(dom, BmDomainType_BASIC) );

    ck_assert_uint_eq( dom->size, 6 );
    ck_assert_str_eq( BmDomain_strAt(dom, 1), "1" );
    ck_assert_str_eq( BmDomain_strAt(dom, 2), "2" );
    ck_assert_str_eq( BmDomain_strAt(dom, 3), "3" );
    ck_assert_str_eq( BmDomain_strAt(dom, 4), "4" );
    ck_assert_str_eq( BmDomain_strAt(dom, 5), "5" );
    ck_assert_str_eq( BmDomain_strAt(dom, 6), "6" );

    ck_assert_str_eq( dom->name, "Dom" );
    BmDomain * dom2= newBmDomainAs(dom);
 
    ck_assert_ptr_ne( dom->dsc, dom2->dsc );

    deleteBmDomain(dom);
    deleteBmDomain(dom2);
*/
}
END_TEST

START_TEST(test_BmDomainRange_init)
{
/*
    BmDomain * dom1= newBmDomainRange("Dom", 1, 3, 1);
    
    ck_assert_uint_eq( dom1->size, 3 );
    ck_assert_int_eq( BmDomainRange_from( dom1 ), 1 );
    ck_assert_int_eq( BmDomainRange_to( dom1 ), 3 );
    ck_assert_int_eq( BmDomainRange_by( dom1 ), 1 );

    ck_assert_str_eq( BmDomain_strAt(dom1, 1), "1" );
    ck_assert_str_eq( BmDomain_strAt(dom1, 2), "2" );
    ck_assert_str_eq( BmDomain_strAt(dom1, 3), "3" );
    
    ck_assert( BmDomain_isType(dom1, BmDomainType_RANGE) );
    ck_assert( !BmDomain_isType(dom1, BmDomainType_UNDEF) );

    BmDomain * dom2= newBmDomainRange("Dom", 4, 128, 2);
    
    ck_assert_uint_eq( dom2->size, 63 );
    ck_assert_int_eq( BmDomainRange_from( dom2 ), 4 );
    ck_assert_int_eq( BmDomainRange_to( dom2 ), 128 );
    ck_assert_int_eq( BmDomainRange_by( dom2 ), 2 );

    ck_assert_str_eq( BmDomain_strAt(dom2, 1), "4" );
    ck_assert_str_eq( BmDomain_strAt(dom2, 2), "6" );
    ck_assert_str_eq( BmDomain_strAt(dom2, 3), "8" );
    ck_assert_str_eq( BmDomain_strAt(dom2, 42), "86" );
    ck_assert_str_eq( BmDomain_strAt(dom2, 62), "126" );
    ck_assert_str_eq( BmDomain_strAt(dom2, 63), "128" );
        
    BmDomain * dom3= newBmDomainRange("Dom", 3, -12, -3);

    ck_assert_uint_eq( dom3->size, 6 );
    ck_assert_int_eq( BmDomainRange_from( dom3 ), 3 );
    ck_assert_int_eq( BmDomainRange_to( dom3 ), -12 );
    ck_assert_int_eq( BmDomainRange_by( dom3 ), -3 );

    ck_assert_str_eq( BmDomain_strAt(dom3, 1), "3" );
    ck_assert_str_eq( BmDomain_strAt(dom3, 2), "0" );
    ck_assert_str_eq( BmDomain_strAt(dom3, 3), "-3" );
    ck_assert_str_eq( BmDomain_strAt(dom3, 4), "-6" );
    ck_assert_str_eq( BmDomain_strAt(dom3, 5), "-9" );
    ck_assert_str_eq( BmDomain_strAt(dom3, 6), "-12" );

    deleteBmDomain(dom1);
    deleteBmDomain(dom2);
    deleteBmDomain(dom3);
*/
}
END_TEST

START_TEST(test_BmDomainWords_init)
{
/*
    char* words[4]= { "Abricot", "Bleuet", "Cerise", "Datte" };
    BmDomain * dom1= newBmDomainWordsArray("Dom", 4, words);
    
    ck_assert_uint_eq( dom1->size, 4 );
    ck_assert_str_eq( BmDomain_strAt(dom1, 1), "Abricot" );
    ck_assert_str_eq( BmDomain_strAt(dom1, 2), "Bleuet" );
    ck_assert_str_eq( BmDomain_strAt(dom1, 3), "Cerise" );
    ck_assert_str_eq( BmDomain_strAt(dom1, 4), "Datte" );
    
    ck_assert( BmDomain_isType(dom1, BmDomainType_WORDS) );

    BmDomain * dom2= newBmDomainWords("Dom", 4, "Abricot", "Bleuet", "Cerise", "Datte");
    
    ck_assert_uint_eq( dom2->size, 4 );
    ck_assert_str_eq( BmDomain_strAt(dom2, 1), "Abricot" );
    ck_assert_str_eq( BmDomain_strAt(dom2, 2), "Bleuet" );
    ck_assert_str_eq( BmDomain_strAt(dom2, 3), "Cerise" );
    ck_assert_str_eq( BmDomain_strAt(dom2, 4), "Datte" );
    
    ck_assert( BmDomain_isType(dom2, BmDomainType_WORDS) );

    deleteBmDomain(dom1);
    deleteBmDomain(dom2);
*/
}
END_TEST

START_TEST(test_BmDomain_print)
{
/*
    BmDomain * domRange= newBmDomainRange("Range", 3, -12, -3);

    ck_assert_str_eq( BmDomain_wording(domRange), "Range[3, 0, -3, -6, -9, -12]" );

    char* words[4]= { "Abricot", "Bleuet", "Cerise", "Datte" };
    BmDomain * domWords= newBmDomainWordsArray("Fruit", 4, words);
    
    ck_assert_str_eq( BmDomain_wording(domWords), "Fruit[Abricot, Bleuet, Cerise, Datte]" );
 
    BmDomain * dom2= newBmDomainAs( domWords );
    ck_assert_str_eq( BmDomain_wording(dom2), "Fruit[Abricot, Bleuet, Cerise, Datte]" ); 
    ck_assert_ptr_ne( domWords->dsc, dom2->dsc );
    
    deleteBmDomain(domWords);
    deleteBmDomain(domRange);
    deleteBmDomain(dom2);
*/
}
END_TEST

START_TEST(test_BmDomainMeans_init)
{
/*
    double means[6]= { -3.7, 0.5, 1.6, 2.0, 4.6, 12.37 };
    BmDomain * dom= newBmDomainMeans("Dom", 6, means);
    
    ck_assert( BmDomain_isType(dom, BmDomainType_MEANS) );

    ck_assert_uint_eq( dom->size, 6 );
    ck_assert_str_eq( BmDomain_strAt(dom, 1), "-3.7" );
    ck_assert_str_eq( BmDomain_strAt(dom, 2), "0.5" );
    ck_assert_str_eq( BmDomain_strAt(dom, 3), "1.6" );
    ck_assert_str_eq( BmDomain_strAt(dom, 4), "2" );
    ck_assert_str_eq( BmDomain_strAt(dom, 5), "4.6" );
    ck_assert_str_eq( BmDomain_strAt(dom, 6), "12.37" );
    
    ck_assert_str_eq( BmDomain_wording(dom), "Dom[-3.7, 0.5, 1.6, 2, 4.6, 12.37]" );

    BmDomain * dom2= newBmDomainAs(dom);
    ck_assert_str_eq( BmDomain_wording(dom2), "Dom[-3.7, 0.5, 1.6, 2, 4.6, 12.37]" ); 
    ck_assert_ptr_ne( dom->dsc, dom2->dsc );
    
    deleteBmDomain(dom);
    deleteBmDomain(dom2);
*/
}
END_TEST


START_TEST(test_BmDomain_values)
{
/*
    double means[4]= { -3.7, 0.5, 2.0, 12.37 };
    BmDomain * dom= newBmDomainMeans("Dom", 4, means);

    ck_assert_uint_eq( dom->size, 4 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 1), -3.7 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 2), 0.5 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 3), 2.0 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 4), 12.37 );

    deleteBmDomain(dom);

    dom= newBmDomainRange("Dom", -4, 10, 3);
    
    ck_assert_double_eq( BmDomain_valueAt(dom, 1), -4.0 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 2), -1.0 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 3), 2.0 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 4), 5.0 );

    deleteBmDomain(dom);

    char* words[4]= { "Abricot", "Bleuet", "Cerise", "Datte" };
    dom= newBmDomainWordsArray("Dom", 4, words);

    ck_assert_double_eq( BmDomain_valueAt(dom, 1), 1.0 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 2), 2.0 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 3), 3.0 );
    ck_assert_double_eq( BmDomain_valueAt(dom, 4), 4.0 );

    deleteBmDomain(dom);
*/
}
END_TEST

START_TEST(test_BmDomainRange_double)
{
/*
    BmDomain * dom= newBmDomainRange( "Dom", 2.1, 3.4, 0.4 );

    ck_assert_uint_eq( dom->size, 4 );
    ck_assert_float_eq( (float)BmDomain_valueAt(dom, 1), 2.1f );
    ck_assert_float_eq( (float)BmDomain_valueAt(dom, 2), 2.5f );
    ck_assert_float_eq( (float)BmDomain_valueAt(dom, 3), 2.9f );
    ck_assert_float_eq( (float)BmDomain_valueAt(dom, 4), 3.3f );

    ck_assert_str_eq( BmDomain_wording(dom), "Dom[2.1, 2.5, 2.9, 3.3]" );

    BmDomain * dom2= newBmDomainAs( dom );
    ck_assert_str_eq( BmDomain_wording(dom2), "Dom[2.1, 2.5, 2.9, 3.3]" ); 
    ck_assert_ptr_ne( dom->dsc, dom2->dsc );


    deleteBmDomain(dom);
*/
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmDomain(void)
{
    /* BmDomain test case */
    TCase *tc= tcase_create("BmDomain");

    tcase_add_test(tc, test_BmDomainBasic_init);
    tcase_add_test(tc, test_BmDomainRange_init);
    tcase_add_test(tc, test_BmDomainWords_init);
    tcase_add_test(tc, test_BmDomain_print);
    tcase_add_test(tc, test_BmDomainMeans_init);
    tcase_add_test(tc, test_BmDomain_values);
    tcase_add_test(tc, test_BmDomainRange_double);

    return tc;
}
