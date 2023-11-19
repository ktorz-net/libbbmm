// BbMm
#include "bbmm-models.h"
#include "bbmm-test.h"

#include <stdio.h>


START_TEST(test_BmCondition_init)
{
    BmCondition* instance1= newBmConditionBasic(2);
    char buffer[1024];

    ck_assert_uint_eq( BmCode_dimention( BmCondition_parents( instance1 ) ), 1 );
    ck_assert_uint_eq( instance1->outputSize, 2 );
    ck_assert_str_eq( BmCode_wording( BmCondition_parents(instance1) ), "[1]");
    ck_assert_uint_eq( BmCode_dimention( BmCondition_parents(instance1) ), 1 );

    ck_assert_str_eq( BmCode_wording(instance1->parentRanges), "[1]" );
    {
        BmCode* config= BmCode_newBmCodeOnKey( instance1->parentRanges, 1 );
        ck_assert_str_eq( BmCode_wording(config), "[1]" );
    
        ck_assert_str_eq( BmTree_wording( instance1->selector), "{}" );

        BmBench* distrib= BmCondition_at( instance1, config );
        deleteBmCode(config);

        strcpy( buffer, "" );
        ck_assert_str_eq( BmBench_printValues(distrib, buffer), "{[1]:1.00}" );
    }

    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmBench_printValues( BmCondition_atKey(instance1, 1), buffer ), "{[1]:1.00}" );

    BmCondition* instance2= newBmConditionWith(
        2,
        newBmCode_list(3, 4, 5, 6),
        newBmBenchWith( 1, newBmCode_list( 1, 1 ), 1, 1.0 )
    );

    ck_assert_uint_eq( BmCode_product( BmCondition_parents(instance2) ), 4*5*6 );
    ck_assert_uint_eq( instance2->outputSize, 2 );
    ck_assert_str_eq( BmCode_wording( BmCondition_parents(instance2) ), "[4, 5, 6]");

    uint dim= BmCode_dimention( BmCondition_parents( instance2 ) );
    for( uint i= 1; i <= dim ; ++i )
    {
        strcpy( buffer, "" );
        ck_assert_str_eq(
            BmBench_printValues( BmCondition_atKey(instance2, i), buffer ),
            "{[1]:1.00}"
        );
    }

    deleteBmCondition(instance2);
    deleteBmCondition(instance1);
}
END_TEST

START_TEST(test_BmCondition_init2)
{
    char buffer[1024];
    BmCondition* instance= newBmConditionWith(
        4,
        newBmCode_list(2, 3, 5),
        newBmBenchWith( 1, newBmCode_list( 1, 1 ), 1, 1.0 )
    );

    ck_assert_uint_eq( instance->outputSize, 4 );
    ck_assert_uint_eq( BmCode_dimention( instance->parentRanges ), 2 );
    ck_assert_uint_eq( BmCode_product( instance->parentRanges ), 3*5 );
    
    strcpy(buffer, "");
    ck_assert_str_eq( BmCondition_printIdentity( instance, buffer ), "[3, 5]->[4]" ); 

    strcpy(buffer, BmTree_wording( instance->selector ));
    ck_assert_str_eq( buffer, "{}");

    strcpy(buffer, "");
    BmCondition_printExtendSep( instance, buffer, ",\n" );
    ck_assert_str_eq( buffer, "[3, 5]->[4]: {[1, 1]: {[1]:1.00},\n\
[2, 1]: {[1]:1.00},\n\
[3, 1]: {[1]:1.00},\n\
[1, 2]: {[1]:1.00},\n\
[2, 2]: {[1]:1.00},\n\
[3, 2]: {[1]:1.00},\n\
[1, 3]: {[1]:1.00},\n\
[2, 3]: {[1]:1.00},\n\
[3, 3]: {[1]:1.00},\n\
[1, 4]: {[1]:1.00},\n\
[2, 4]: {[1]:1.00},\n\
[3, 4]: {[1]:1.00},\n\
[1, 5]: {[1]:1.00},\n\
[2, 5]: {[1]:1.00},\n\
[3, 5]: {[1]:1.00}}" );

    deleteBmCondition(instance);
}
END_TEST

START_TEST(test_BmCondition_defaultDistrib)
{
    char buffer[1014]= "";

    // Initialize BmDistribution
    BmBench* distrib= newBmBench(4);
    BmBench_attachLast( distrib, newBmCode_list(1, 1), 0, 0.3 );
    BmBench_attachLast( distrib, newBmCode_list(1, 2), 0, 0.1 );
    BmBench_attachLast( distrib, newBmCode_list(1, 3), 0, 0.3 );
    BmBench_attachLast( distrib, newBmCode_list(1, 5), 0, 0.3 );

    strcpy(buffer, "");
    BmBench_printValues( distrib, buffer );
    ck_assert_str_eq(
    buffer,
    "{[1]:0.30, [2]:0.10, [3]:0.30, [5]:0.30}"
    );

    // Initialize BmCondition
    BmCondition* instance= newBmConditionWith(
        6,
        newBmCode_list(2, 6, 2),
        distrib);
    
    strcpy(buffer, "");
    BmCode* code= newBmCodeAs( BmCondition_parents(instance) );
    BmBench_printValues( BmCondition_at(instance, code), buffer );
    ck_assert_str_eq(
    buffer,
    "{[1]:0.30, [2]:0.10, [3]:0.30, [5]:0.30}"
    );

    BmCode_at_set(code, 1, 3);
    BmCode_at_set(code, 2, 1);

    strcpy(buffer, "");
    BmBench_printValues( BmCondition_at(instance, code), buffer );
    ck_assert_str_eq(
    buffer,
    "{[1]:0.30, [2]:0.10, [3]:0.30, [5]:0.30}"
    );

    strcpy(buffer, "");
    BmCondition_print( instance, buffer );
    ck_assert_str_eq(
    buffer,
    "[6, 2]->[6]: {[0, 0]: {[1]:0.30, [2]:0.10, [3]:0.30, [5]:0.30}}"
    );

    deleteBmCode(code);
    deleteBmCondition(instance);
}

START_TEST(test_BmCondition_switch)
{
    char buffer[1024];
    BmCondition* instance= newBmConditionWith(
        4,
        newBmCode_list(2, 3, 5),
        newBmBenchWith( 1, newBmCode_list( 1, 1 ), 1, 1.0 )
    );

    ck_assert_uint_eq( instance->outputSize, 4 );
    
    strcpy(buffer, "");
    ck_assert_str_eq( BmCondition_printIdentity( instance, buffer ), "[3, 5]->[4]" ); 

    strcpy(buffer, "");
    BmTree_print( instance->selector, buffer);
    ck_assert_str_eq( buffer, "{}");

    strcpy(buffer, "");
    BmCondition_printExtendSep( instance, buffer, ",\n" );
    ck_assert_str_eq( buffer, "[3, 5]->[4]: {[1, 1]: {[1]:1.00},\n\
[2, 1]: {[1]:1.00},\n\
[3, 1]: {[1]:1.00},\n\
[1, 2]: {[1]:1.00},\n\
[2, 2]: {[1]:1.00},\n\
[3, 2]: {[1]:1.00},\n\
[1, 3]: {[1]:1.00},\n\
[2, 3]: {[1]:1.00},\n\
[3, 3]: {[1]:1.00},\n\
[1, 4]: {[1]:1.00},\n\
[2, 4]: {[1]:1.00},\n\
[3, 4]: {[1]:1.00},\n\
[1, 5]: {[1]:1.00},\n\
[2, 5]: {[1]:1.00},\n\
[3, 5]: {[1]:1.00}}" );

    BmCondition* doobleganger= newBmConditionWith(
        3, newBmCode_list( 2, 3, 2 ),
        newBmBenchWith( 1, newBmCode_list(1, 1), 1, 1.0 )
    );

    strcpy(buffer, "");
    BmCondition_printExtendSep( doobleganger, buffer, ",\n" );

    ck_assert_str_eq( buffer, "[3, 2]->[3]: {[1, 1]: {[1]:1.00},\n\
[2, 1]: {[1]:1.00},\n\
[3, 1]: {[1]:1.00},\n\
[1, 2]: {[1]:1.00},\n\
[2, 2]: {[1]:1.00},\n\
[3, 2]: {[1]:1.00}}" );

    BmCondition_switch(instance, doobleganger);

    strcpy(buffer, "");
    BmCondition_printExtendSep( doobleganger, buffer, ",\n" );
    ck_assert_str_eq( buffer, "[3, 5]->[4]: {[1, 1]: {[1]:1.00},\n\
[2, 1]: {[1]:1.00},\n\
[3, 1]: {[1]:1.00},\n\
[1, 2]: {[1]:1.00},\n\
[2, 2]: {[1]:1.00},\n\
[3, 2]: {[1]:1.00},\n\
[1, 3]: {[1]:1.00},\n\
[2, 3]: {[1]:1.00},\n\
[3, 3]: {[1]:1.00},\n\
[1, 4]: {[1]:1.00},\n\
[2, 4]: {[1]:1.00},\n\
[3, 4]: {[1]:1.00},\n\
[1, 5]: {[1]:1.00},\n\
[2, 5]: {[1]:1.00},\n\
[3, 5]: {[1]:1.00}}" );

    strcpy(buffer, "");
    BmCondition_printExtendSep( instance, buffer, ",\n" );

    ck_assert_str_eq( buffer, "[3, 2]->[3]: {[1, 1]: {[1]:1.00},\n\
[2, 1]: {[1]:1.00},\n\
[3, 1]: {[1]:1.00},\n\
[1, 2]: {[1]:1.00},\n\
[2, 2]: {[1]:1.00},\n\
[3, 2]: {[1]:1.00}}" );

    deleteBmCondition(instance);
    deleteBmCondition(doobleganger);
}
END_TEST

START_TEST(test_BmCondition_manipulate)
{
    char buffer[1024];
    BmCondition* instance= newBmConditionWith(
        2,
        newBmCode_list(3, 4, 5, 6),
        newBmBenchWith( 1, newBmCode_list( 1, 1 ), 1, 1.0 )
    );

    uint size= BmCode_product( BmCondition_parents(instance) );
    for( uint i= 1; i <= size ; ++i )
    {
        strcpy( buffer, "" );
        ck_assert_str_eq(
            BmBench_printValues( BmCondition_atKey(instance, i), buffer ),
            "{[1]:1.00}" );
    }

    // Attach a new distrib to another config:
    BmCode* config= newBmCode_list(3, 1, 2, 2);
    
    ck_assert_uint_eq(
        BmCode_dimention(instance->parentRanges),
        BmCode_dimention(config)
    );
    
    uint iDistrib= BmCondition_at_attach(
        instance, config,
        newBmBenchWith( 2, newBmCode_list(1, 2), 0, 0.6 )
    );

    BmBench_attachLast( 
        BmCondition_distribution( instance, iDistrib ),
        newBmCode_list(1, 1), 0, 0.4
    );

    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmBench_printValues( BmCondition_at(instance, config), buffer ),
        "{[2]:0.60, [1]:0.40}"
    );

    ck_assert_uint_eq( BmCode_keyOf( instance->parentRanges, config), 25 );
    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmBench_printValues( BmCondition_atKey(instance, 25), buffer ),
        "{[2]:0.60, [1]:0.40}"
    );

    strcpy( buffer, "" );
    BmCondition_print( instance, buffer );
    ck_assert_str_eq( buffer,
"[4, 5, 6]->[2]: {[0, 2, 1]: {[1]:1.00},\n\
  [0, 2, 2]: {[2]:0.60, [1]:0.40},\n\
  [0, 2, 3]: {[1]:1.00},\n\
  [0, 2, 4]: {[1]:1.00},\n\
  [0, 2, 5]: {[1]:1.00},\n\
  [0, 2, 6]: {[1]:1.00},\n\
  [1, 1, 0]: {[1]:1.00},\n\
  [1, 3, 0]: {[1]:1.00},\n\
  [1, 4, 0]: {[1]:1.00},\n\
  [1, 5, 0]: {[1]:1.00},\n\
  [2, 0, 0]: {[1]:1.00},\n\
  [3, 0, 0]: {[1]:1.00},\n\
  [4, 0, 0]: {[1]:1.00}}"
    );

    // Attach another one...:
    BmCode_reinit_list( config, 3, 1, 4, 3);
    
    BmBench* distrib= newBmBench(2);
    BmBench_attachLast( distrib, newBmCode_list(1, 1), 0, 0.8 );
    BmBench_attachLast( distrib, newBmCode_list(1, 2), 0, 0.2 );

    iDistrib= BmCondition_at_attach( instance, config, distrib );

    ck_assert_uint_eq( iDistrib, 3 );

    strcpy( buffer, "" );
    BmCondition_print( instance, buffer );
    ck_assert_str_eq( buffer,
"[4, 5, 6]->[2]: {[0, 2, 1]: {[1]:1.00},\n\
  [0, 2, 2]: {[2]:0.60, [1]:0.40},\n\
  [0, 2, 3]: {[1]:1.00},\n\
  [0, 2, 4]: {[1]:1.00},\n\
  [0, 2, 5]: {[1]:1.00},\n\
  [0, 2, 6]: {[1]:1.00},\n\
  [0, 4, 1]: {[1]:1.00},\n\
  [0, 4, 2]: {[1]:1.00},\n\
  [0, 4, 3]: {[1]:0.80, [2]:0.20},\n\
  [0, 4, 4]: {[1]:1.00},\n\
  [0, 4, 5]: {[1]:1.00},\n\
  [0, 4, 6]: {[1]:1.00},\n\
  [1, 1, 0]: {[1]:1.00},\n\
  [1, 3, 0]: {[1]:1.00},\n\
  [1, 5, 0]: {[1]:1.00},\n\
  [2, 0, 0]: {[1]:1.00},\n\
  [3, 0, 0]: {[1]:1.00},\n\
  [4, 0, 0]: {[1]:1.00}}"
    );

    // And reinit:
    distrib= newBmBench(2);
    BmBench_attachLast( distrib, newBmCode_list(1, 1), 0, 0.8 );
    BmBench_attachLast( distrib, newBmCode_list(1, 2), 0, 0.2 );

    BmCondition_reinitDistributionsWith( instance, distrib );

    size= BmCode_product( BmCondition_parents( instance ) );
    for( uint i= 1; i <= size ; ++i )
    {
        strcpy( buffer, "" );
        ck_assert_str_eq(
            BmBench_printValues( BmCondition_atKey(instance, i), buffer ),
            "{[1]:0.80, [2]:0.20}" );
    }

    deleteBmCondition(instance);
}
END_TEST

START_TEST(test_BmCondition_manipulate2)
{
    char buffer[1024];
    BmBench* distrib= newBmBench(2);
    BmBench_attachLast( distrib, newBmCode_list(1, 1), 0, 0.8 );
    BmBench_attachLast( distrib, newBmCode_list(1, 3), 0, 0.2 );

    BmCondition* instance= newBmConditionWith(
        4, newBmCode_list(2, 3, 5), distrib );

    BmCode* config= newBmCode_list(2, 2, 5 );
    distrib= newBmBenchWith( 1, newBmCode_list(1, 2), 0 , 1.0 );
    BmCondition_at_attach( instance, config, distrib );


    uint configId= BmCode_keyOf( instance->parentRanges, config );
    uint parentSize=  BmCode_product( BmCondition_parents(instance) );

    for( uint i= 1; i <= parentSize ; ++i )
    {
        if( i == configId )
        {
            strcpy( buffer, "" );
            ck_assert_str_eq(
                BmBench_printValues( BmCondition_atKey(instance, i), buffer ),
                "{[2]:1.00}"
            );
        }
        else 
        {
            strcpy( buffer, "" );
            ck_assert_str_eq(
                BmBench_printValues( BmCondition_atKey(instance, i), buffer ),
                "{[1]:0.80, [3]:0.20}"
            );
        }
    }

    BmBench* parentDistribution= newBmBench(2);

    BmBench_attachLast( parentDistribution, newBmCodeAs(config), 0, 1.0 );

    ck_assert_uint_eq( BmBench_size( parentDistribution ), 1 );
    
    BmCondition_infer( instance, parentDistribution );

    strcpy( buffer, "" );
    ck_assert_str_eq(
        BmBench_printValues( parentDistribution, buffer ),
        "{[2, 5, 2]:1.00}"
    );

    /*
    BmDistribution_initialize(parentDistribution, 2);
    BmDistribution_addConfig( parentDistribution, BmCode_at_set(config, 2, 3), 1.0 );
    BmCondition_infer(instance, parentDistribution);

    ck_assert_str_eq( BmDistribution_wording( parentDistribution ), "{[2, 3, 1]:0.80, [2, 3, 3]:0.20}" );

    BmDistribution_initialize(parentDistribution, 2);
    BmDistribution_addConfig( parentDistribution, config, 0.5 );

    BmCode_at_set(config, 2, 5);
    BmDistribution_addConfig( parentDistribution, config, 0.5 );

    BmCondition_infer(instance, parentDistribution);
    ck_assert_str_eq( BmDistribution_wording( parentDistribution ), "{[2, 3, 1]:0.40, [2, 3, 3]:0.10, [2, 5, 2]:0.50}" );

    deleteBmCode( config );
    deleteBmDistribution(parentDistribution);
    */

    deleteBmCode( config );
    deleteBmCondition(instance);
}
END_TEST

START_TEST(test_BmCondition_print)
{
    /*
    BmCondition* instance;
    {
        BmCode* parentRanges= newBmCode_list(3, 4, 5, 6);
        instance= newBmConditionBreack( 2, parentRanges );
        deleteBmCode(parentRanges);
    }

    ck_assert_str_eq( 
        BmCondition_wordingExtend( instance ),
        "[4, 5, 6]->[2]: {[1, 1, 1]: {[1]:1.00},\n\
[2, 1, 1]: {[1]:1.00},\n\
[3, 1, 1]: {[1]:1.00},\n\
[4, 1, 1]: {[1]:1.00},\n\
[1, 2, 1]: {[1]:1.00},\n\
[2, 2, 1]: {[1]:1.00},\n\
[3, 2, 1]: {[1]:1.00},\n\
[4, 2, 1]: {[1]:1.00},\n\
[1, 3, 1]: {[1]:1.00},\n\
[2, 3, 1]: {[1]:1.00},\n\
[3, 3, 1]: {[1]:1.00},\n\
[4, 3, 1]: {[1]:1.00},\n\
[1, 4, 1]: {[1]:1.00},\n\
[2, 4, 1]: {[1]:1.00},\n\
[3, 4, 1]: {[1]:1.00},\n\
[4, 4, 1]: {[1]:1.00},\n\
[1, 5, 1]: {[1]:1.00},\n\
[2, 5, 1]: {[1]:1.00},\n\
[3, 5, 1]: {[1]:1.00},\n\
[4, 5, 1]: {[1]:1.00},\n\
[1, 1, 2]: {[1]:1.00},\n\
[2, 1, 2]: {[1]:1.00},\n\
[3, 1, 2]: {[1]:1.00},\n\
[4, 1, 2]: {[1]:1.00},\n\
[1, 2, 2]: {[1]:1.00},\n\
[2, 2, 2]: {[1]:1.00},\n\
[3, 2, 2]: {[1]:1.00},\n\
[4, 2, 2]: {[1]:1.00},\n\
[1, 3, 2]: {[1]:1.00},\n\
[2, 3, 2]: {[1]:1.00},\n\
[3, 3, 2]: {[1]:1.00},\n\
[4, 3, 2]: {[1]:1.00},\n\
[1, 4, 2]: {[1]:1.00},\n\
[2, 4, 2]: {[1]:1.00},\n\
[3, 4, 2]: {[1]:1.00},\n\
[4, 4, 2]: {[1]:1.00},\n\
[1, 5, 2]: {[1]:1.00},\n\
[2, 5, 2]: {[1]:1.00},\n\
[3, 5, 2]: {[1]:1.00},\n\
[4, 5, 2]: {[1]:1.00},\n\
[1, 1, 3]: {[1]:1.00},\n\
[2, 1, 3]: {[1]:1.00},\n\
[3, 1, 3]: {[1]:1.00},\n\
[4, 1, 3]: {[1]:1.00},\n\
[1, 2, 3]: {[1]:1.00},\n\
[2, 2, 3]: {[1]:1.00},\n\
[3, 2, 3]: {[1]:1.00},\n\
[4, 2, 3]: {[1]:1.00},\n\
[1, 3, 3]: {[1]:1.00},\n\
[2, 3, 3]: {[1]:1.00},\n\
[3, 3, 3]: {[1]:1.00},\n\
[4, 3, 3]: {[1]:1.00},\n\
[1, 4, 3]: {[1]:1.00},\n\
[2, 4, 3]: {[1]:1.00},\n\
[3, 4, 3]: {[1]:1.00},\n\
[4, 4, 3]: {[1]:1.00},\n\
[1, 5, 3]: {[1]:1.00},\n\
[2, 5, 3]: {[1]:1.00},\n\
[3, 5, 3]: {[1]:1.00},\n\
[4, 5, 3]: {[1]:1.00},\n\
[1, 1, 4]: {[1]:1.00},\n\
[2, 1, 4]: {[1]:1.00},\n\
[3, 1, 4]: {[1]:1.00},\n\
[4, 1, 4]: {[1]:1.00},\n\
[1, 2, 4]: {[1]:1.00},\n\
[2, 2, 4]: {[1]:1.00},\n\
[3, 2, 4]: {[1]:1.00},\n\
[4, 2, 4]: {[1]:1.00},\n\
[1, 3, 4]: {[1]:1.00},\n\
[2, 3, 4]: {[1]:1.00},\n\
[3, 3, 4]: {[1]:1.00},\n\
[4, 3, 4]: {[1]:1.00},\n\
[1, 4, 4]: {[1]:1.00},\n\
[2, 4, 4]: {[1]:1.00},\n\
[3, 4, 4]: {[1]:1.00},\n\
[4, 4, 4]: {[1]:1.00},\n\
[1, 5, 4]: {[1]:1.00},\n\
[2, 5, 4]: {[1]:1.00},\n\
[3, 5, 4]: {[1]:1.00},\n\
[4, 5, 4]: {[1]:1.00},\n\
[1, 1, 5]: {[1]:1.00},\n\
[2, 1, 5]: {[1]:1.00},\n\
[3, 1, 5]: {[1]:1.00},\n\
[4, 1, 5]: {[1]:1.00},\n\
[1, 2, 5]: {[1]:1.00},\n\
[2, 2, 5]: {[1]:1.00},\n\
[3, 2, 5]: {[1]:1.00},\n\
[4, 2, 5]: {[1]:1.00},\n\
[1, 3, 5]: {[1]:1.00},\n\
[2, 3, 5]: {[1]:1.00},\n\
[3, 3, 5]: {[1]:1.00},\n\
[4, 3, 5]: {[1]:1.00},\n\
[1, 4, 5]: {[1]:1.00},\n\
[2, 4, 5]: {[1]:1.00},\n\
[3, 4, 5]: {[1]:1.00},\n\
[4, 4, 5]: {[1]:1.00},\n\
[1, 5, 5]: {[1]:1.00},\n\
[2, 5, 5]: {[1]:1.00},\n\
[3, 5, 5]: {[1]:1.00},\n\
[4, 5, 5]: {[1]:1.00},\n\
[1, 1, 6]: {[1]:1.00},\n\
[2, 1, 6]: {[1]:1.00},\n\
[3, 1, 6]: {[1]:1.00},\n\
[4, 1, 6]: {[1]:1.00},\n\
[1, 2, 6]: {[1]:1.00},\n\
[2, 2, 6]: {[1]:1.00},\n\
[3, 2, 6]: {[1]:1.00},\n\
[4, 2, 6]: {[1]:1.00},\n\
[1, 3, 6]: {[1]:1.00},\n\
[2, 3, 6]: {[1]:1.00},\n\
[3, 3, 6]: {[1]:1.00},\n\
[4, 3, 6]: {[1]:1.00},\n\
[1, 4, 6]: {[1]:1.00},\n\
[2, 4, 6]: {[1]:1.00},\n\
[3, 4, 6]: {[1]:1.00},\n\
[4, 4, 6]: {[1]:1.00},\n\
[1, 5, 6]: {[1]:1.00},\n\
[2, 5, 6]: {[1]:1.00},\n\
[3, 5, 6]: {[1]:1.00},\n\
[4, 5, 6]: {[1]:1.00}}" );

    char buffer[1024]="";
    BmCondition_print( instance, buffer );
    
    ck_assert_str_eq( buffer,
"[4, 5, 6]->[2]: {[0, 0, 0]: {[1]:1.00}}"
    );

    deleteBmCondition(instance);
    */
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_BmCondition(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("BmCondition");

    tcase_add_test(tc, test_BmCondition_init);
    tcase_add_test(tc, test_BmCondition_init2);
    tcase_add_test(tc, test_BmCondition_defaultDistrib);
    tcase_add_test(tc, test_BmCondition_switch);
    tcase_add_test(tc, test_BmCondition_manipulate);
    tcase_add_test(tc, test_BmCondition_manipulate2);
    tcase_add_test(tc, test_BmCondition_print);

    return tc;
}
