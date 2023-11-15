// Wanda
#include "wanda-mdl.h"
#include "wanda-test.h"

#include <stdio.h>

/* test cofee Robot.
 * An example from : Craig Boutilier, Richard Dearden, Moisés Goldszmidt - Stochastic dynamic programming with factored representations - 2000
 */

enum VariableNames {
    var_location=1,
    var_wet,
    var_umbrella,
    var_raining,
    var_robotCofee,
    var_ownerCofee,
    var_action,
    var_locationT1,
    var_wetT1,
    var_umbrellaT1,
    var_rainingT1,
    var_robotCofeeT1,
    var_ownerCofeeT1,
    var_size= var_ownerCofeeT1
};

enum Boolean {
    True=1,
    False,
    Boolean_size= False
};

enum LocationNames {
    location_office=1,
    location_cafe,
    location_size= location_cafe
};

enum ActionNames {
    action_move=0,
    action_buyCofee,
    action_giveCofee,
    action_getUmbrella,
    action_size 
};

WdTransition* CofeeRobot_newTransition()
{
    WdCode* states= newWdCodeBasic(6, 2);
    WdCode* actions= newWdCode(1, action_size);
    
    WdTransition * model= newWdTransition(states, actions );
    return model;
}

/*
void CofeeRobot_initializeProbabilities(WdDynamic * model)
{
    WdDynamic * d= WdDynamic_node(model, 5 );
    deleteWdDynamic(d);
}
*/

START_TEST(test_cofeeRobot_init)
{
    WdTransition* model= CofeeRobot_newTransition();
    
    ck_assert_uint_eq( WdTransition_overallDimention(model), var_size );
    ck_assert_str_eq( WdNet_wording(model->network), "1[], 2[], 3[], 4[], 5[], 6[], 7[], 8[], 9[], 10[], 11[], 12[], 13[]" );
    
    // Robot Location :
/*
    WdDynamic_node_depends2(model, var_locationT1, var_location, var_action);
    ck_assert_str_eq( WdNet_wording(model->network), "0[], 1[], 2[], 3[], 4[], 5[], 6[], 7[0, 6], 8[], 9[], 10[], 11[], 12[]" );

    WdVariable_str( &(model->nodes[var_locationT1]), buffer);
    ck_assert_uint_ge( blen, strlen(buffer) );
    ck_assert_str_eq( buffer, "X {[0, 0]: {[0]: 1.000000} ; [1, 0]: {[0]: 1.000000} ; [0, 1]: {[0]: 1.000000} ; [1, 1]: {[0]: 1.000000} ; [0, 2]: {[0]: 1.000000} ; [1, 2]: {[0]: 1.000000} ; [0, 3]: {[0]: 1.000000} ; [1, 3]: {[0]: 1.000000}}" );
    
    WdVariable *locationT1Dep= model->nodes+var_locationT1;
    for( uint a= 0 ; a < (uint)action_size ; ++a  )
    {
        for( uint l= 0 ; l < (uint)location_size ; ++l  )
        {
            uint state[2]= {l, a};
            WdDistribution* d= WdVariable_initializeDistributionOf(locationT1Dep, state);
            if( a == action_move )
            {
                if( l == location_office )
                {
                    WdDistribution_add1( d, (uint)location_cafe, 1.0 );
                }
                else
                {
                    WdDistribution_add1( d, (uint)location_office, 1.0 );
                }
            }
            else
            {
                WdDistribution_add1( d, l, 1.0 );
            }
        }
    }

    WdVariable_str( &(model->nodes[var_locationT1]), buffer);
    ck_assert_str_eq( "X {[0, 0]: {[1]: 1.000000} ; [1, 0]: {[0]: 1.000000} ; [0, 1]: {[0]: 1.000000} ; [1, 1]: {[1]: 1.000000} ; [0, 2]: {[0]: 1.000000} ; [1, 2]: {[1]: 1.000000} ; [0, 3]: {[0]: 1.000000} ; [1, 3]: {[1]: 1.000000}}", buffer );
*/
    deleteWdTransition(model);
}
END_TEST

/*
 *       Test case scenario
 ***********************************************************************************/

TCase * test_case_cofeeRobot(void)
{
    /* WdDomain test case */
    TCase *tc= tcase_create("CofeeRobot");

    tcase_add_test(tc, test_cofeeRobot_init);

    return tc;
}
