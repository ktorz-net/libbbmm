#ifndef BBMM_MODEL_H
#define BBMM_MODEL_H

#include "bbmm-structures.h"

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   M O D E L  :  C O N D I T I O N                             *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a Bayesian Node (conditional probabilities over variable affectations)
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint outputSize;
  BmCode* parentRanges;
  BmTree* selector;
  uint distribSize, distribCapacity;
  BmBench **distributions;
} BmCondition;

/* Constructor */
BmCondition* newBmConditionBasic(uint outputSize);
BmCondition* newBmConditionWith(uint domainSize, BmCode* newParentRanges, BmBench* newDefaultDistrib);

BmCondition* BmCondition_createBasic(BmCondition* self, uint outputSize);
BmCondition* BmCondition_createWith(BmCondition* self, uint domainSize, BmCode* newParentRanges, BmBench* newDefaultDistrib);

/* Destructor */
BmCondition* BmCondition_destroy(BmCondition* self);
void deleteBmCondition(BmCondition* instance);

/* re-initializer */
uint BmCondition_reinitWith( BmCondition* self, uint outputSize, BmCode* newParents, BmBench* newDistrib );
uint BmCondition_reinitDistributionsWith( BmCondition* self, BmBench* newDistrib );

/* Accessor */
uint BmCondition_output( BmCondition* self );
BmCode* BmCondition_parents( BmCondition* self );
BmBench* BmCondition_at( BmCondition* self, BmCode* configuration );
BmBench* BmCondition_atKey( BmCondition* self, uint configKey );
BmBench* BmCondition_distribution( BmCondition* self, uint iDistrib );

/* Construction */
uint BmCondition_at_attach( BmCondition* self, BmCode* configuration, BmBench* distribution );

/* Instance tools */
void BmCondition_switch(BmCondition* self, BmCondition* doppelganger);

/* Inferring */
BmBench* BmCondition_infer( BmCondition* self, BmBench* distribOverConfigurations );
BmBench* BmCondition_newDistributionByInfering( BmCondition* self, BmBench* distribOverConfigurations );
BmBench* BmCondition_newDistributionByInfering_mask( BmCondition* self, BmBench* longDistrib, BmCode* parentMask );

/* Printing */
char* BmCondition_print(BmCondition* self, char* output);
char* BmCondition_printSep(BmCondition* self, char* output, char* separator);

char* BmCondition_printExtend(BmCondition* self, char* output); // print `self` at the end of `output`
char* BmCondition_printExtendSep(BmCondition* self, char* output, char* separator);

char* BmCondition_printIdentity( BmCondition* self, char* output ); // print `self` at the end of on `output`


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   M O D E L  :  I N F E R E R                                 *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a Bayesian Network composed of state, action and tramsitional nodes
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint inputDimention, outputDimention, overallDimention;
  BmBench* network;
  BmCondition* nodes;
  BmBench* distribution;
} BmInferer;

/* Constructor*/
BmInferer* newBmInferer( BmCode* variableSpace, uint inputDimention, uint outputDimention );
BmInferer* newBmInfererStateAction( BmCode* stateSpace, BmCode* actionSpace );
BmInferer* newBmInfererStateActionShift( BmCode* stateSpace, BmCode* actionSpace, BmCode* shiftSpace );

BmInferer* BmInferer_create( BmInferer* self, BmCode* varDomains, uint inputDimention, uint outputDimention );

/* Destructor */
BmInferer* BmInferer_destroy(BmInferer* self);
void deleteBmInferer(BmInferer* self);

/* Accessor */
BmBench* BmInferer_distribution( BmInferer* self );

uint BmInferer_inputDimention( BmInferer* self );
uint BmInferer_outputDimention( BmInferer* self );
uint BmInferer_shiftDimention( BmInferer* self );
uint BmInferer_overallDimention( BmInferer* self );

BmCondition* BmInferer_node( BmInferer* self, uint iNode );
uint BmInferer_node_size( BmInferer* self, uint iVar );
BmCode* BmInferer_node_parents( BmInferer* self, uint iVar );

/* Construction */
BmCondition* BmInferer_node_reinitIndependant( BmInferer* self, uint index );
BmCondition* BmInferer_node_reinitWith( BmInferer* self, uint index, BmCode* newDependenceMask, BmBench* newDefaultDistrib );

/* Process */
BmBench* BmInferer_process( BmInferer* self, BmBench* inputDistribution );        // Return distribution over output varibales
BmBench* BmInferer_process_newOverallDistribution( BmInferer* self, BmBench* inputDistribution ); // Return distribution over all variables
BmBench* BmInferer_processState_Action( BmInferer* self, BmCode* state, BmCode* action ); // Return distribution over statePrime (output)

/* Printing */
char* BmInferer_print(BmInferer* self, char* output); // print `self` at the end of `output`
char* BmInferer_printStateActionSignature(BmInferer* self, char* output); // print `self` at the end of `output`
char* BmInferer_printDependency(BmInferer* self, char* output); // print `self` at the end of `output`



/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   M O D E L  :  E V A L U A T O R                             *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * Define a multi-critera Value function
 * (input -> value vector \times weight -> value)
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  BmCode* space;
  uint criteriaSize;
  BmTree ** criteria;
  BmCode ** masks;
  BmVector* weights;
} BmEvaluator ;

/* Constructor*/
BmEvaluator* newBmEvaluatorBasic( uint spaceDimention, uint numberOfCriteria );
BmEvaluator* newBmEvaluatorWith( BmCode* newSpace, uint numberOfCriteria );

BmEvaluator* BmEvaluator_createWith( BmEvaluator* self, BmCode* newSpace, uint numberOfCriteria );

/* Destructor */
void deleteBmEvaluator( BmEvaluator* self );
BmEvaluator* BmEvaluator_destroy( BmEvaluator* self);

/* Accessor */
BmCode* BmEvaluator_space( BmEvaluator* self );
uint BmEvaluator_numberOfCriteria( BmEvaluator* self );
BmTree* BmEvaluator_crit( BmEvaluator* self, uint iCritirion );
BmVector* BmEvaluator_weights( BmEvaluator* self );
double BmEvaluator_crit_weight( BmEvaluator* self, uint iCritirion );

/* Process */
double BmEvaluator_process( BmEvaluator* self, BmCode* input );
double BmEvaluator_processState_action(BmEvaluator* self, BmCode* state, BmCode* action);
double BmEvaluator_processState_action_state(BmEvaluator* self, BmCode* state, BmCode* action, BmCode* statePrime);

double BmEvaluator_crit_process( BmEvaluator* self, uint iCriterion, BmCode* input );

/* Construction */
BmEvaluator* BmEvaluator_reinitCriterion( BmEvaluator* self, uint numberOfCriterion );
BmTree* BmEvaluator_crit_reinitWith( BmEvaluator* self, uint index, BmCode* newDependenceMask, uint numberOfOptions, double defaultValue );
void BmEvaluator_crit_setWeight( BmEvaluator* self, uint iCritirion, double weight );

/* Infering */


/* Printing */




#endif // BBMM_MODEL_H