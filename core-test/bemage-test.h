#ifndef WANDA_TEST
#define WANDA_TEST

#include <check.h>

#define wt_print(str) printf("<--\n%s\n-->\n", str)

// Wanda-stt
TCase * test_case_BmCode(void);
//TCase * test_case_WdCollection(void);
//TCase * test_case_WdNet(void);
//TCase * test_case_WdTree(void);
//TCase * test_case_WdValueFct(void); // TODO

// Wanda-prb
//TCase * test_case_WdDistribution(void);
//TCase * test_case_WdCondition(void);
//TCase * test_case_WdTransition(void);

// Wanda-mdl
//TCase * test_case_WdDomain(void);
//TCase * test_case_WdSpace(void);
//TCase * test_case_WdModel(void);

// Wanda-slv

// Examples
//TCase * test_case_cofeeRobot(void);


#endif //WANDA_TEST