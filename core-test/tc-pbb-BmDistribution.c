// BbMm
#include "bbmm-probabilities.h"
#include "bbmm-test.h"

#include <stdio.h>

START_TEST(test_BmDistribution_init)
{
    BmDistribution * distrib= newBmDistribution(3);

    ck_assert_uint_eq( BmDistribution_dimention(distrib), 3 );
    ck_assert_uint_eq( BmDistribution_size(distrib), 0 );

    BmCode * config= newBmCode_list(3, 1, 1, 4 );

    BmDistribution_addConfig( distrib, config, 0.4 );
    ck_assert_uint_eq( BmDistribution_size(distrib), 1 );

    deleteBmCode(config);
    deleteBmDistribution(distrib);
}
END_TEST


START_TEST(test_BmDistribution_init2)
{
    BmDistribution * instance= newBmDistribution(4);

    ck_assert_uint_eq( instance->capacity, 1 );
    ck_assert_uint_eq( instance->size, 0 );
    ck_assert_uint_eq( instance->dimention, 4 );

    deleteBmDistribution(instance);
}
END_TEST

START_TEST(test_BmDistribution_manipulation)
{
    BmDistribution * instance= newBmDistribution(4);
    BmDistribution_resizeCapacity(instance, 8);

    BmCode* state1= newBmCode_list(4, 1, 2, 1, 9);
    BmCode* state2= newBmCode_list(4, 2, 1, 4, 4);
    BmCode* state3= newBmCode_list(4, 2, 1, 4, 8);

    BmDistribution_addConfig(instance, state1, 0.2);
    BmDistribution_addConfig(instance, state2, 0.7);

    ck_assert_uint_eq( instance->size, 2 );
    ck_assert_uint_eq( instance->capacity, 8 );

    for( uint i= 1 ; i <= 4 ; ++i )
        ck_assert_uint_eq( BmCode_at( instance->configurations[1], i), BmCode_at(state2, i) );

    BmDistribution_addConfig(instance, state3, 0.1);

    ck_assert_uint_eq( instance->size, 3 );
    ck_assert_uint_gt( instance->capacity, 2 );

    for( uint i= 1 ; i <= 4 ; ++i )
        ck_assert_uint_eq( BmCode_at(instance->configurations[2], i), BmCode_at(state3, i) );

    BmDistribution_clear(instance);
    ck_assert_uint_eq( instance->size, 0 );

    deleteBmCode( state1 );
    deleteBmCode( state2 );
    deleteBmCode( state3 );
    deleteBmDistribution(instance);
}
END_TEST

START_TEST(test_BmDistribution_copy)
{
    BmDistribution * model= newBmDistribution(4);


    BmCode* state[3]= { newBmCode_list(4, 1, 2, 1, 9), newBmCode_list(4, 2, 1, 4, 4), newBmCode_list(4, 2, 1, 4, 8) };
    double proba[3]= {0.2, 0.7, 0.1};

    BmDistribution_addConfig(model, state[0], proba[0]);
    BmDistribution_addConfig(model, state[1], proba[1]);
    BmDistribution_addConfig(model, state[2], proba[2]);

    BmDistribution * instance= newBmDistribution(3);

    ck_assert_str_eq(BmDistribution_wording(instance), "{}");

    BmDistribution_initializeAs(instance, model);
    deleteBmDistribution(model);

    ck_assert_uint_eq( instance->capacity, 3 );
    ck_assert_uint_eq( instance->dimention, 4 );
    ck_assert_uint_eq( instance->size, 3 );

    for( uint st= 0 ; st < 3 ; ++st )
    {
        for( uint i= 1 ; i <= 4 ; ++i )
            ck_assert_uint_eq( BmCode_at(instance->configurations[st], i), BmCode_at(state[st], i) );
    }

    for( uint i=0 ; i < 3 ; ++i )
        deleteBmCode( state[i] );
    deleteBmDistribution(instance);
}
END_TEST

START_TEST(test_BmDistribution_print)
{
    BmDistribution * instance= newBmDistribution(4);

    BmCode* state1= newBmCode_list(4, 1, 2, 1, 9);
    BmCode* state2= newBmCode_list(4, 2, 1, 4, 4);
    BmCode* state3= newBmCode_list(4, 2, 1, 4, 8);

    char buffer[256];

    ck_assert_str_eq(BmDistribution_wording(instance), "{}");

    BmDistribution_addConfig(instance, state1, 0.2);
    BmDistribution_addConfig(instance, state2, 0.7);
    BmDistribution_addConfig(instance, state3, 0.1);

    strcpy(buffer, "");
    BmDistribution_printPosibility(instance, 2, buffer);

    ck_assert_str_eq(buffer, "[2, 1, 4, 8]: 0.10");
    
    ck_assert_str_eq(BmDistribution_wording(instance), "{[1, 2, 1, 9]: 0.20 ; [2, 1, 4, 4]: 0.70 ; [2, 1, 4, 8]: 0.10}");

    deleteBmCode( state1 );
    deleteBmCode( state2 );
    deleteBmCode( state3 );
    deleteBmDistribution(instance);
}
END_TEST

START_TEST(test_BmDistribution_process)
{
    BmDistribution * instance= newBmDistribution(4);
    BmDistribution_resizeCapacity(instance, 8);

    BmCode* states[8]=
    {
        newBmCode_list(4, 1, 2, 1, 3),
        newBmCode_list(4, 2, 1, 4, 4),
        newBmCode_list(4, 3, 1, 4, 2),
        newBmCode_list(4, 2, 3, 4, 2),
        newBmCode_list(4, 1, 2, 1, 3),
        newBmCode_list(4, 4, 1, 4, 4),
        newBmCode_list(4, 2, 1, 3, 2),
        newBmCode_list(4, 2, 1, 4, 2),
    };
    double probas[8]= {0.2, 0.1, 0.05, 0.15, 0.3, 0.02, 0.05, 0.13};

    ck_assert_uint_eq( instance->size, 0);
    ck_assert_uint_eq( instance->dimention, 4);
    ck_assert_str_eq(BmDistribution_wording(instance), "{}");

    for( int i= 0 ; i < 8 ; ++i )
        BmDistribution_addConfig(instance, states[i], probas[i] );

    ck_assert_uint_eq( instance->size, 8);
    ck_assert_str_eq(
        BmDistribution_wording(instance),
        "{[1, 2, 1, 3]: 0.20 ; [2, 1, 4, 4]: 0.10 ; [3, 1, 4, 2]: 0.050 ; [2, 3, 4, 2]: 0.15 ; [1, 2, 1, 3]: 0.30 ; [4, 1, 4, 4]: 0.020 ; [2, 1, 3, 2]: 0.050 ; [2, 1, 4, 2]: 0.13}"
    );
    
    ck_assert_uint_eq( BmDistribution_size(instance), 8 );
    ck_assert( BmDistribution_probabilityAt( instance, 2 ) == 0.05 );
    
    BmCode* c= BmDistribution_configAt( instance, 2 );
    ck_assert_str_eq( BmCode_wording(c), "[3, 1, 4, 2]" );

    BmDistribution_sort( instance );
    ck_assert_str_eq(
        BmDistribution_wording(instance),
        "{[1, 2, 1, 3]: 0.20 ; [1, 2, 1, 3]: 0.30 ; [2, 1, 3, 2]: 0.050 ; [2, 1, 4, 2]: 0.13 ; [2, 1, 4, 4]: 0.10 ; [2, 3, 4, 2]: 0.15 ; [3, 1, 4, 2]: 0.050 ; [4, 1, 4, 4]: 0.020}"
    );  
    BmDistribution_mergeDoubles( instance );

    ck_assert_uint_eq( instance->size, 7);
    ck_assert_uint_eq( instance->dimention, 4);

    ck_assert_str_eq(
        BmDistribution_wording(instance),
        "{[1, 2, 1, 3]: 0.50 ; [2, 1, 3, 2]: 0.050 ; [2, 1, 4, 2]: 0.13 ; [2, 1, 4, 4]: 0.10 ; [2, 3, 4, 2]: 0.15 ; [3, 1, 4, 2]: 0.050 ; [4, 1, 4, 4]: 0.020}"
    );

    uint mask[2]= {3, 4};
    BmDistribution_reduceDimentionTo( instance, 2, mask);

    ck_assert_uint_eq( instance->size, 4);
    ck_assert_uint_eq( instance->dimention, 2);
    ck_assert_str_eq(
        BmDistribution_wording(instance),
        "{[1, 3]: 0.50 ; [3, 2]: 0.050 ; [4, 2]: 0.33 ; [4, 4]: 0.12}"
    );

    for( uint i=0 ; i < 8 ; ++i )
        deleteBmCode( states[i] );
    deleteBmDistribution(instance);
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmDistribution(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("prb-BmDistribution");

    tcase_add_test(tc, test_BmDistribution_init);
    tcase_add_test(tc, test_BmDistribution_init2);
    tcase_add_test(tc, test_BmDistribution_manipulation);
    tcase_add_test(tc, test_BmDistribution_copy);
    tcase_add_test(tc, test_BmDistribution_print);
    tcase_add_test(tc, test_BmDistribution_process);

    return tc;
}
