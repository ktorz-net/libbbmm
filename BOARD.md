# Bayesian-based Marokov-model board

## On the fire:

- Manage explicit version number
	- git ???
	- [cmake](https://cmake.org/cmake/help/v3.18/guide/tutorial/#adding-a-version-number-and-configured-header-file)
- Updated version of BmTree and BmBench.
	* Removing of the notion of tag (plus D1 version for attach)
	* BmTree BmTree_at return a code (BmTree integrate a bench in place of tag and value).
	* Make BmTree from BmBench + separator and vise versa.
- Generate a frontend (bbmm.h) from bbmm-xxx.h and licence.
- It's time for solvers based on `<transition, reward>`
	*  `BmSolver` with process function variable and `policyIteration`, `valueIteration`.
- Réactivate cofeeRobot exemple and generate cofeeFleet.

- In some way, Transition and Stoschastic policy are sames: Bayesian Network...
	* BmTranstion => BmInferencor( input (state) / premises, shift, output / consequences (distribution) );

- move bbmm repo to libbbmm and create a BbMM-space project, then -> PAAMS / EUMAS

## Next:

- Réactivate Python Wrap (pyBbMm).
- Réactivate Cpp Wrap (cppBbMm.h).

- libbbmm + wraps -> journal or huge conf

- In code documentation (code to markdon documentation) + Dev. Phylosophie

### ABM and simulation:

- BmAgent
	* Process: action= agent.decide(perception)
- BmSystem
	* Process: output/perception= system.process(input/action)

### Learning Algorythms:

- Complete BmTree functionnalities (auto-optimisation...)
- Learn DecisionTrees, RandomForest and GradianBoost.
- Apply to learn policy.
- Learn Transition ?

### Testing:

- Test pure random variable in transition definition (shift of future with no dependancy).
- Test memory leaks (Valgrin ?).
- Add testing functions (isValid....).
- Add tests intenssivelly...

## What about ?

- Probabitilities as ratio (Rational Number) ? Ok, but with compilation mode.
	- lookat [gmplib](https://gmplib.org/)
- BmCollection based on Bench128 (1028/128 == 1028>>7; 1028%128 == 1028&127)s
- Integrate a string-stream librairie or use a BmStr structure
- Use igraph toolbox
- Pharo: Prepare for uiPlayer: (BbMm-presenter Package... Paul)
- Windaube ?
- Optimazing BmBench buffer with ratio attributs (count the numbers of attachLast versus size ).
- Create `newDistritutionEquiprobable`, test it and add it in `BmCondition_initializeEquiprobable`
- `BmSystem_variable_initializeProbabilities` and `BmSystem_variable_addProbabilities` based on attached prebuilt-distribution. become... `BmSystem_variable_attachDistribution_if` based on `BmSystem_variable_neBmistribution`.
- Moving BbMm Models as a `cpp-src` moduls ?
- Bridge BbMm and [igraph](https://github.com/igraph/igraph) ?

see: on [dlib](https://github.com/davisking/dlib/blob/master/examples/bayes_net_ex.cpp), for an inspiration of how to generate a BN. 

## State Of The Art: 

- OMLT: https://jmlr.org/papers/v23/22-0277.html - Optimisation and Learning based on Neural Networks.
- Pyomo: https://pypi.org/project/Pyomo/ - Modeling Optimisation Problems.
- AGRUM and pyAGRUM
- PpenMPI - https://www.open-mpi.org/ - Open Source High Performance Computing

