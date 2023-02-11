#include "bemage-test.h"

#include <stdlib.h>
#include <stdio.h>

Suite * wanda_stt_test_suite(void)
{
    Suite *s= suite_create("BeMAge-structures");

    suite_add_tcase( s, test_case_BmCode() );
    suite_add_tcase( s, test_case_BmBench() );
    suite_add_tcase( s, test_case_BmNet() );
    suite_add_tcase( s, test_case_BmTree() );
    
    return s;
}

Suite * wanda_prb_test_suite(void)
{
    Suite *s= suite_create("BeMAge-probabilities");

    //suite_add_tcase( s, test_case_WdDistribution() );
    //suite_add_tcase( s, test_case_WdCondition() );
    //suite_add_tcase( s, test_case_WdTransition() );

    return s;
}

Suite * wanda_mdl_test_suite(void)
{
    Suite *s= suite_create("BeMAge-model");

    //suite_add_tcase( s, test_case_WdDomain() );
    //suite_add_tcase( s, test_case_WdSpace() );
    //suite_add_tcase( s, test_case_WdModel() );

    return s;
}

Suite * wanda_slv_test_suite(void)
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
        SRunner *sr= srunner_create( wanda_stt_test_suite() );
        srunner_run_all(sr, CK_NORMAL);
        number_failed+= srunner_ntests_failed(sr);
        srunner_free(sr);
    }
    {
        SRunner *sr= srunner_create( wanda_prb_test_suite() );
        srunner_run_all(sr, CK_NORMAL);
        number_failed+= srunner_ntests_failed(sr);
        srunner_free(sr);
    }
    {
        SRunner *sr= srunner_create( wanda_mdl_test_suite() );
        srunner_run_all(sr, CK_NORMAL);
        number_failed+= srunner_ntests_failed(sr);
        srunner_free(sr);
    }
    {
        SRunner *sr= srunner_create( wanda_slv_test_suite() );
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

    printf("Test-Runner: %i\n", number_failed);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}