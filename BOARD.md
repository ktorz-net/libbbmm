# BeMAge board

## On the fire:

- Bugs on BbMm Probabilities
- Create `newDistritutionEquiprobable`, test it and add it in `BmCondition_initializeEquiprobable`
- Reactivate bbmm-models
	* `BmModel`-> BmProcess  (`<State, Action, Shift, transition, reward>`)
	* BmFunction -> BmTree selector and values[] (opt).
	* BmPolicy -> BmTree selector and distribution (probability + value) (opt)
- Réactivate Python Wrap (pyBbMm)

- `BmModel_variable_initializeProbabilities` and `BmModel_variable_addProbabilities` based on attached prebuilt-distribution.
	* become... `BmModel_variable_attachDistribution_if` based on `BmModel_variable_neBmistribution`.
- Set default node as equiprobable.
- Test pure random variable in transition definition (shift of future with no dependancy).
- It's time for solvers based on `<transition, reward>` (ValueIt. - PolicyIt.).
- Test memory leaks (Valgrin ?).
- Add tests intenssivelly (BmDistribution,....).
- Add testing functions (isValid....).
- remove BmCollection.
- BmTree initialized empty (default 0), easely resizable (bench128).
- Clean function names + on code documentations. 
- PyBbMm.
- doc + webpage
- `attach` and `deepDistroy` everywhere/ `switch` everythings - (Edit a Dev. Phylosophie)
- `Enenced-C`: `vector` (~BmCode based on void**) ; `data` a bench of vector + type ; `ratio`

## What about ?

- Probabitilities as ratio (Rational Number).
- Distribution versus Collection ?
- BmCollection based on Bench128 (1028/128 == 1028>>7; 1028%128 == 1028&127)s

## State Of The Art: 

- OMLT: https://jmlr.org/papers/v23/22-0277.html - Optimisation and Learning based on Neural Networks.
- Pyomo: https://pypi.org/project/Pyomo/ - Modeling Optiisation Problems.x
- AGRUM and pyAGRUM
- PpenMPI - https://www.open-mpi.org/ - Open Source High Performance Computing


## Going Further:

- manipulate rational-number ?
- Integrate a string-stream librairie or use a BmStr structure ?
- What about igraph toolbox ?
- Pharo: Prepare for uiPlayer: (BbMm-presenter Package... Paul)
- Windaube

see: on [dlib](https://github.com/davisking/dlib/blob/master/examples/bayes_net_ex.cpp), for an inspiration of how to generate a BN. 

