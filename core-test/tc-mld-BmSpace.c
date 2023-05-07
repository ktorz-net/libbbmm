// Wanda
#include "bbmm-models.h"
#include "bbmm-test.h"

#include <stdlib.h>
#include <stdio.h>

START_TEST(test_BmSpace_initEmpty)
{
    BmSpace* space= newBmSpaceBasic();

    ck_assert_uint_eq( BmSpace_dimention(space), 0);
    ck_assert_uint_eq( (uint)(BmSpace_size(space)), 1 );

    deleteBmSpace(space);
}
END_TEST

START_TEST(test_BmSpace_init)
{
    char* variableNames[3]= {"X1", "X2", "X3"};
    BmDomain * dom12= newBmDomainRange("Range", 3, 9, 3);
    BmDomain * dom3= newBmDomainWords("Bool", 2, "True", "False");
    BmDomain * domains[3]= { dom12, dom12, dom3 };
    
    BmSpace* space= newBmSpace(3, variableNames, domains );

    ck_assert_uint_eq( BmSpace_dimention(space), 3);

    ck_assert_uint_eq( BmSpace_variable_domainSize( space, 1 ), 3 );
    ck_assert_uint_eq( BmSpace_variable_domainSize( space, 2 ), 3 );
    ck_assert_uint_eq( BmSpace_variable_domainSize( space, 3 ), 2 );
    
    ck_assert_uint_eq( (uint)(BmSpace_size(space)), (3*3*2) );

    BmDomain * domainsBuffer[ BmSpace_dimention(space) ];
    uint nboDomains= BmSpace_feedWithDomains( space, domainsBuffer, 0 );
    ck_assert_uint_eq( nboDomains, 2);

    deleteDeepBmSpace(space);
}
END_TEST

START_TEST(test_BmSpace_initCascade)
{
    char* variableNames[3]= {"X1", "X2", "X3"};
    BmDomain * dom12= newBmDomainRange("Range", 3, 9, 3);
    BmDomain * dom3= newBmDomainWords("Bool", 2, "True", "False");
    BmDomain * domains[3]= { dom12, dom12, dom3 };
    
    BmSpace* space= newBmSpace(3, variableNames, domains );

    BmSpace_distroyDeep( space );
    BmSpace_createEmpty( space, 3 );

    ck_assert( !BmSpace_isValid(space) );

    BmSpace_attachVariable(space, 1, "X", newBmDomainWords("Bool", 2, "True", "False") );
    BmSpace_attachVariable(space, 2, "Y", newBmDomainRange("Range", 3, 9, 3) );
    BmSpace_attachVariable(space, 3, "Z", BmSpace_variable_domain(space, 1) );
    
    ck_assert( BmSpace_isValid(space) );

    ck_assert_uint_eq( BmSpace_dimention(space), 3);
    ck_assert_uint_eq( (uint)(BmSpace_size(space)), (2*3*2) );

    ck_assert_str_eq( BmDomain_wording( BmSpace_variable_domain( space, 1 ) ), "Bool[True, False]" );
    ck_assert_str_eq( BmDomain_wording( BmSpace_variable_domain( space, 2 ) ), "Range[3, 6, 9]" );
    ck_assert_str_eq( BmDomain_wording( BmSpace_variable_domain( space, 3 ) ), "Bool[True, False]" );

    BmDomain * domainsBuffer[ BmSpace_dimention(space) ];
    uint nboDomains= BmSpace_feedWithDomains( space, domainsBuffer, 0 );
    ck_assert_uint_eq( nboDomains, 2);

    ck_assert_str_eq( BmDomain_wording( array_at(domainsBuffer, 1) ), "Bool[True, False]" );
    ck_assert_str_eq( BmDomain_wording( array_at(domainsBuffer, 2) ), "Range[3, 6, 9]" );

    deleteBmSpace(space);    
}
END_TEST

START_TEST(test_BmSpace_nullSpace)
{
    BmSpace* space= newBmSpace( 0, NULL, NULL );
    ck_assert_uint_eq( BmSpace_dimention(space), 0);

    BmDomain* differentDomains[ BmSpace_dimention(space) ];
    uint nbDiffDom= BmSpace_feedWithDomains( space, differentDomains, 0 );

    ck_assert_uint_eq( nbDiffDom, 0 );

    deleteDeepBmSpace(space);
}
END_TEST

START_TEST(test_BmSpace_code)
{
    BmDomain* dom1= newBmDomainRange( "Range", 3, 9, 3);
    BmDomain* dom2= newBmDomainWords( "Bool", 2, "True", "False" );
    char* atoe[5]= {"A", "B", "C", "D", "E"};
    BmDomain* dom3= newBmDomainWordsArray( "Letters", 5, atoe );

    char* varNames[5]= {"X1", "X2", "X3", "X4", "X5"};
    BmDomain * spaceDom[5]= { dom1, dom2, dom3, dom3, dom1 };
    
    BmSpace* space= newBmSpace( 5, varNames, spaceDom );
    
    BmCode* codeSpace= BmSpace_asNewBmCode(space);
    
    ck_assert_uint_eq( BmCode_at(codeSpace, 1), 3 );
    ck_assert_uint_eq( BmCode_at(codeSpace, 2), 2 );
    ck_assert_uint_eq( BmCode_at(codeSpace, 3), 5 );
    ck_assert_uint_eq( BmCode_at(codeSpace, 4), 5 );
    ck_assert_uint_eq( BmCode_at(codeSpace, 5), 3 );
    ck_assert_uint_eq( BmCode_keyOf(codeSpace, codeSpace), BmSpace_size(space) );

    BmCode* code= BmCode_newBmCodeFirst(codeSpace);
    ck_assert_uint_eq( BmCode_at(code, 1), 1 );
    ck_assert_uint_eq( BmCode_at(code, 2), 1 );
    ck_assert_uint_eq( BmCode_at(code, 3), 1 );
    ck_assert_uint_eq( BmCode_at(code, 4), 1 );
    ck_assert_uint_eq( BmCode_at(code, 5), 1 );
    ck_assert_uint_eq( BmCode_keyOf(codeSpace, code), 1 );

    deleteBmCode(code);

    code= BmCode_newBmCodeOnKey(codeSpace, 2);

    ck_assert_uint_eq( BmCode_at(code, 1), 2 );
    ck_assert_uint_eq( BmCode_at(code, 2), 1 );
    ck_assert_uint_eq( BmCode_at(code, 3), 1 );
    ck_assert_uint_eq( BmCode_at(code, 4), 1 );
    ck_assert_uint_eq( BmCode_at(code, 5), 1 );
    ck_assert_uint_eq( BmCode_keyOf(codeSpace, code), 2 );

    deleteBmCode(code);

    code= BmCode_newBmCodeOnKey(codeSpace, 164);

    ck_assert_uint_eq( BmCode_at(code, 1), 2 );
    ck_assert_uint_eq( BmCode_at(code, 2), 1 );
    ck_assert_uint_eq( BmCode_at(code, 3), 3 );
    ck_assert_uint_eq( BmCode_at(code, 4), 1 );
    ck_assert_uint_eq( BmCode_at(code, 5), 2 );
    ck_assert_uint_eq( BmCode_keyOf(codeSpace, code), 164 );

    deleteBmCode(code);

    deleteBmCode(codeSpace);
    deleteBmSpace(space);
}
END_TEST

START_TEST(test_BmSpace_state)
{
    BmDomain* dom1= newBmDomainRange( "Range", 3, 9, 3);
    BmDomain* dom2= newBmDomainWords( "Bool", 2, "True", "False" );
    char* atoe[5]= {"A", "B", "C", "D", "E"};
    BmDomain* dom3= newBmDomainWordsArray( "Letters", 5, atoe );

    char* varNames[5]= {"X1", "X2", "X3", "X4", "X5"};
    BmDomain * spaceDom[5]= { dom1, dom2, dom3, dom3, dom1 };
    
    BmSpace* space= newBmSpace( 5, varNames, spaceDom );

    uint numbers[5]= {1, 1, 4, 5, 2};
    BmCode* code= newBmCode_numbers(5, numbers);

    ck_assert_str_eq( BmDomain_strAt( BmSpace_variable_domain( space, 1), BmCode_at(code, 1) ), "3" );
    ck_assert_str_eq( BmDomain_strAt( BmSpace_variable_domain( space, 2), BmCode_at(code, 2) ), "True" );
    ck_assert_str_eq( BmDomain_strAt( BmSpace_variable_domain( space, 3), BmCode_at(code, 3) ), "D" );
    ck_assert_str_eq( BmDomain_strAt( BmSpace_variable_domain( space, 4), BmCode_at(code, 4) ), "E" );
    ck_assert_str_eq( BmDomain_strAt( BmSpace_variable_domain( space, 5), BmCode_at(code, 5) ), "6" );

    ck_assert_str_eq( BmSpace_code_wording( space, code ), "[X1(3), X2(True), X3(D), X4(E), X5(6)]" );
    ck_assert_str_eq( BmSpace_key_wording( space, 289 ), "[X1(3), X2(True), X3(D), X4(E), X5(6)]" );
    ck_assert_str_eq( BmSpace_key_wording( space, 127 ), "[X1(3), X2(True), X3(B), X4(E), X5(3)]" );
    
    deleteBmCode(code);
    deleteBmSpace(space);
}
END_TEST

START_TEST(test_BmSpace_print)
{
    BmDomain* dom1= newBmDomainRange( "Range", 3, 9, 3);
    BmDomain* dom2= newBmDomainWords( "Bool", 2, "True", "False" );
    char* atoe[5]= {"A", "B", "C", "D", "E"};
    BmDomain* dom3= newBmDomainWordsArray( "Letters", 5, atoe );

    char* varNames[5]= {"X1", "X2", "X3", "X4", "X5"};
    BmDomain * spaceDom[5]= { dom1, dom2, dom3, dom3, dom1 };
    
    BmSpace* space= newBmSpace( 5, varNames, spaceDom );

    char buffer[2048]= "";

    BmSpace_print( space, buffer );
    ck_assert_str_eq(
        buffer,
        "Range[3, 6, 9],\n\
Bool[True, False],\n\
Letters[A, B, C, D, E],\n\
Letters[A, B, C, D, E],\n\
Range[3, 6, 9]."
    );

    deleteBmSpace(space);
}
END_TEST

/*
START_TEST(test_BmSpace_duplicate)
{
    BmDomain* dom1= newBmDomainRange( "Range", 3, 9, 3);
    BmDomain* dom2= newBmDomainWords( "Bool", 2, "True", "False" );
    char* atoe[5]= {"A", "B", "C", "D", "E"};
    BmDomain* dom3= newBmDomainWordsArray( "Letters", 5, atoe );
    BmDomain * spaceDom[5]= { dom1, dom2, dom3, dom3, dom1 };
    BmSpace* space= newBmSpace( 5, spaceDom );
    deleteBmDomain(dom1);
    deleteBmDomain(dom2);
    deleteBmDomain(dom3);

    BmSpace* duplicata= newBmSpaceAs( space );

    ck_assert_uint_eq( BmSpace_dimention(space),  BmSpace_dimention(duplicata) );
    ck_assert_uint_eq( BmSpace_size(space),  BmSpace_size(duplicata) );
    ck_assert_uint_eq( space->nbOfDomains,  space->nbOfDomains );

    char buffer1[2048]= "";
    char buffer2[2048]= "";

    ck_assert_str_eq(
         BmSpace_print( space, buffer1 ),
         BmSpace_print( duplicata, buffer2 )
    );
   
    deleteBmSpace(duplicata);
    deleteBmSpace(space);
}
END_TEST
*/

/*
 *       Test case scenario
 ***********************************************************************************/
TCase * test_case_BmSpace(void)
{
    /* BmSpace test case */
    TCase *tc= tcase_create("BmSpace");

    tcase_add_test(tc, test_BmSpace_initEmpty);
    tcase_add_test(tc, test_BmSpace_init);
    tcase_add_test(tc, test_BmSpace_initCascade);
    tcase_add_test(tc, test_BmSpace_nullSpace);

    tcase_add_test(tc, test_BmSpace_code);
    tcase_add_test(tc, test_BmSpace_state);
    tcase_add_test(tc, test_BmSpace_print);
    //tcase_add_test(tc, test_BmSpace_duplicate);
    
    return tc;
}
