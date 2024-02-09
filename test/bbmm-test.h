#ifndef BBMM_TEST
#define BBMM_TEST

#include <check.h>

#define tc_print(str) printf("<--\n%s\n-->\n", str)

// BbMm structures 
TCase * test_case_BmCode(void);
TCase * test_case_BmBench(void);
TCase * test_case_BmTree(void);
TCase * test_case_BmVector(void);

// BbMm Functions
TCase * test_case_BmFunction(void);
TCase * test_case_BmCondition(void);
TCase * test_case_BmInferer(void);
TCase * test_case_BmEvaluator(void);

// BbMm Models
TCase * test_case_BmChain(void);
TCase * test_case_BmProcess(void);

// BbMm Solver
TCase * test_case_BmBasicPolicy(void);

// Examples
//TCase * test_case_cofeeRobot(void);


#endif //BBMM_TEST