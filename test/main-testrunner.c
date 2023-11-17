#include "bbmm-test.h"

#include <stdlib.h>
#include <stdio.h>

Suite * bbmm_stt_test_suite(void)
{
    Suite *s= suite_create("BbMm-structures");

    suite_add_tcase( s, test_case_BmCode() );
    suite_add_tcase( s, test_case_BmBench() );
    suite_add_tcase( s, test_case_BmTree() );
    suite_add_tcase( s, test_case_BmVector() );
    
    return s;
}

Suite * bbmm_mdl_test_suite(void)
{
    Suite *s= suite_create("BbMm-model");

    suite_add_tcase( s, test_case_BmCondition() );
    //suite_add_tcase( s, test_case_BmTransition() );
    //suite_add_tcase( s, test_case_BmReward() );

    return s;
}

Suite * bbmm_slv_test_suite(void)
{
    Suite *s= suite_create("Wanda-solver");

    return s;
}

Suite * cofeeRobot_test_suite(void)
{
    Suite *s= suite_create("CofeeRobot");

    //suite_add_tcase( s, test_case_cofeeRobot() );

    return s;
}

int main(void)
{
    int number_failed= 0;
    {
        SRunner *sr= srunner_create( bbmm_stt_test_suite() );
        srunner_run_all(sr, CK_NORMAL);
        number_failed+= srunner_ntests_failed(sr);
        srunner_free(sr);
    }
    {
        SRunner *sr= srunner_create( bbmm_mdl_test_suite() );
        srunner_run_all(sr, CK_NORMAL);
        number_failed+= srunner_ntests_failed(sr);
        srunner_free(sr);
    }
/*
    {
        SRunner *sr= srunner_create( bbmm_slv_test_suite() );
        srunner_run_all(sr, CK_NORMAL);
        number_failed+= srunner_ntests_failed(sr);
        srunner_free(sr);
    }
    {
        SRunner *sr= srunner_create( cofeeRobot_test_suite() );
        srunner_run_all(sr, CK_NORMAL);
        number_failed+= srunner_ntests_failed(sr);
        srunner_free(sr);
    }
*/
    printf("Test-Runner: %i\n", number_failed);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
