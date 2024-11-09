#include "bbmm-test.h"

#include <stdlib.h>
#include <stdio.h>

//   DEPENDANCIES:

Suite * bbmm_dpd_test_suite(void)
{
    Suite *s= suite_create("BbMm-dependencies");

    suite_add_tcase( s, test_case_system() );
    suite_add_tcase( s, test_case_bench() );
    suite_add_tcase( s, test_case_basis() );
    
    return s;
}

//   STRUCTURE MODULE:

Suite * bbmm_stt_test_suite(void)
{
    Suite *s= suite_create("BbMm-structures");

    suite_add_tcase( s, test_case_BmCode() );
    suite_add_tcase( s, test_case_BmBench() );
    suite_add_tcase( s, test_case_BmTree() );
    suite_add_tcase( s, test_case_BmVector() );
    
    return s;
}

//   FUNCTION MODULE:

Suite * bbmm_fct_test_suite(void)
{
    Suite *s= suite_create("BbMm-functions");

    suite_add_tcase( s, test_case_BmValueFct() );
    suite_add_tcase( s, test_case_BmFunction() );

    return s;
}

//   COMPONENT MODULE:

Suite * bbmm_cpn_test_suite(void)
{
    Suite *s= suite_create("BbMm-component");

    suite_add_tcase( s, test_case_BmCondition() );
    suite_add_tcase( s, test_case_BmInferer() );
    suite_add_tcase( s, test_case_BmEvaluator() );

    return s;
}

//   SOLVER MODULE:

Suite * bbmm_slv_test_suite(void)
{
    Suite *s= suite_create("BbMm-solver");

    suite_add_tcase( s, test_case_BmFunction() );
    suite_add_tcase( s, test_case_BmBasicPolicy() );

    return s;
}

//   TEST-CASES:

Suite * testcases_test_suite(void)
{
    Suite *s= suite_create("CofeeRobot");

    //suite_add_tcase( s, test_case_cofeeRobot() );

    return s;
}

int main(void)
{
    Suite* suites[4]={
        bbmm_dpd_test_suite(),
        bbmm_stt_test_suite(),
        bbmm_fct_test_suite(),
        bbmm_slv_test_suite()
    };
    
    int number_failed= 0;
    for( int i = 0 ; i < 4 ; ++i )
    {
        SRunner *sr= srunner_create( suites[i] );
        srunner_run_all(sr, CK_NORMAL);
        number_failed+= srunner_ntests_failed(sr);
        srunner_free(sr);
    }
    
    printf("Test-Runner: %i\n", number_failed);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
