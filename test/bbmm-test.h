#ifndef WANDA_TEST
#define WANDA_TEST

#include <check.h>

#define tc_print(str) printf("<--\n%s\n-->\n", str)

// BbMm structures 
TCase * test_case_BmCode(void);
TCase * test_case_BmBench(void);
TCase * test_case_BmTree(void);
TCase * test_case_BmVector(void);

// BbMm Models
TCase * test_case_BmCondition(void);
TCase * test_case_BmTransition(void);
TCase * test_case_BmEvaluator(void);

// BbMm Solver

// Examples
//TCase * test_case_cofeeRobot(void);


#endif //WANDA_TEST