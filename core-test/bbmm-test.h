#ifndef WANDA_TEST
#define WANDA_TEST

#include <check.h>

#define tc_print(str) printf("<--\n%s\n-->\n", str)

// Wanda-stt
TCase * test_case_BmCode(void);
TCase * test_case_BmBench(void);
TCase * test_case_BmNet(void);
TCase * test_case_BmTree(void);
//TCase * test_case_WdValueFct(void); // TODO

// Wanda-prb
TCase * test_case_BmDistribution(void);
TCase * test_case_BmCondition(void);
TCase * test_case_BmTransition(void);

// Wanda-mdl
TCase * test_case_BmDomain(void);
//TCase * test_case_WdSpace(void);
//TCase * test_case_WdModel(void);

// Wanda-slv

// Examples
//TCase * test_case_cofeeRobot(void);


#endif //WANDA_TEST