#ifndef WANDA_TEST
#define WANDA_TEST

#include <check.h>

#define tc_print(str) printf("<--\n%s\n-->\n", str)

// BbMm structures 
TCase * test_case_BmCode(void);
TCase * test_case_BmBench(void);
TCase * test_case_BmTree(void);
TCase * test_case_BmVector(void);

// BbMm values 
TCase * test_case_BmGauge(void);
TCase * test_case_BmEval(void);

// BbMm Probabilities
TCase * test_case_BmDistribution(void);
TCase * test_case_BmCondition(void);
TCase * test_case_BmTransition(void);

// BbMm Models
TCase * test_case_BmDomain(void);
TCase * test_case_BmSpace(void);
TCase * test_case_BmSystem(void);

// BbMm Solver


// Examples
//TCase * test_case_cofeeRobot(void);


#endif //WANDA_TEST