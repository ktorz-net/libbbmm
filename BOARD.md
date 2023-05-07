# BeMAge board

## On the fire:

- BmSystem - Reward: `BmValue`
- Réactivate cofeeRobot exemple
- Réactivate Python Wrap (pyBbMm) with cofeeRobot
- Multi-Robot version -> Article for EUMAS.

- Respect the BmSystem_variable_addProbabilities order, (and adapt BmTree accordingly) (cf. _ToDo:_ tag in `tc-mld-BmSystem.c`)
- Factorized printing of `BmSystem_printVariable`.
- It's time for solvers based on `<transition, reward>` (ValueIt. - PolicyIt.).
	* `BmPolicy` -> BmTree selector and distribution (probability + value) (opt)

- Set default node as equiprobable.
- Test pure random variable in transition definition (shift of future with no dependancy).
- Test memory leaks (Valgrin ?).
- Add tests intenssivelly (BmDistribution,....).
- Add testing functions (isValid....).
- BmTree easely resizable (bench128).
- In code documentation (code to markdon documentation) + Dev. Phylosophie
- Complete BmTree functionnalities (auto-optimisation...).


## What about ?

- Probabitilities as ratio (Rational Number) ? Ok, but with compilation mode.
- BmCollection based on Bench128 (1028/128 == 1028>>7; 1028%128 == 1028&127)s
- Integrate a string-stream librairie or use a BmStr structure
- Use igraph toolbox
- Pharo: Prepare for uiPlayer: (BbMm-presenter Package... Paul)
- Windaube ?
- Optimazing BmBench buffer with ratio attributs (count the numbers of attachLast versus size ).
- Create `newDistritutionEquiprobable`, test it and add it in `BmCondition_initializeEquiprobable`
- `BmSystem_variable_initializeProbabilities` and `BmSystem_variable_addProbabilities` based on attached prebuilt-distribution. become... `BmSystem_variable_attachDistribution_if` based on `BmSystem_variable_neBmistribution`.


## State Of The Art: 

- OMLT: https://jmlr.org/papers/v23/22-0277.html - Optimisation and Learning based on Neural Networks.
- Pyomo: https://pypi.org/project/Pyomo/ - Modeling Optimisation Problems.
- AGRUM and pyAGRUM
- PpenMPI - https://www.open-mpi.org/ - Open Source High Performance Computing


see: on [dlib](https://github.com/davisking/dlib/blob/master/examples/bayes_net_ex.cpp), for an inspiration of how to generate a BN. 

