// BbMm
#include "bbmm-structures.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmNet_init)
{
    BmNet* net= newBmNet(4);

    ck_assert_uint_eq( net->size, 4 );
    ck_assert_str_eq( BmNet_wording(net), "1[], 2[], 3[], 4[]" );

    BmCode* dep= newBmCode_list(2, 1, 2);
    BmNet_at_connect(net, 1, dep);
    BmNet_at_connect(net, 2, BmCode_initialize_list(dep, 3, 2, 3, 4));
    BmNet_at_connect(net, 3, BmCode_initialize_list(dep, 1, 2));
    BmNet_at_connect(net, 4, BmCode_initialize_list(dep, 1, 3));
    
    ck_assert_str_eq( BmNet_wording(net), "1[1, 2], 2[2, 3, 4], 3[2], 4[3]" );

    deleteBmNet(net);
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmNet(void)
{
    TCase *tc= tcase_create("stt-BmNet");

    tcase_add_test(tc, test_BmNet_init);

    return tc;
}
