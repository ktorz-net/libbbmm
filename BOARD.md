# BeMAge board


## On the fire:

- `BmModel_variable_initializeProbabilities` and `BmModel_variable_addProbabilities` based on attached prebuilt-distribution.
	* become... `BmModel_variable_attachDistribution_if` based on `BmModel_variable_neBmistribution`.
- Change `BmCollection` into `BmData`, use `BmData` with `BmTree` into BmFunction.
- Create BmModel `<State, Action, Shift, transition, reward>`.
	* BmValuesFct -> BmDescriptor Based on BmTree (similar to BmCollection functionnality).
- Set default node as equiprobable.
- Test pure random variable in transition definition (shift of future with no dependancy).
- It's time for solvers based on `<transition, reward>` (ValueIt. - PolicyIt.).
- Test memory leaks (Valgrin ?).
- Add tests intenssivelly (BmDistribution,....).
- Add testing functions (isValid....).
- remove BmCollection.
- BmTree initialized empty (default 0), easely resizable (bench128).
- Clean function names + on code documentations. 
- PyBeMAge.
- doc + webpage

## What about ?

- Distribution versus Collection ?
- BmCollection based on Bench128 (1028/128 == 1028>>7; 1028%128 == 1028&127)


## State Of The Art: 

- OMLT: https://jmlr.org/papers/v23/22-0277.html - Optimisation and Learning based on Neural Networks.
- Pyomo: https://pypi.org/project/Pyomo/ - Modeling Optiisation Problems.x
- AGRUM and pyAGRUM
- PpenMPI - https://www.open-mpi.org/ - Open Source High Performance Computing


## Going Further:

- manipulate rational-number ?
- Integrate a string-stream librairie or use a BmStr structure ?
- What about igraph toolbox ?
- Pharo: Prepare for uiPlayer: (BeMAge-presenter Package... Paul)
- Windaube

see: on [dlib](https://github.com/davisking/dlib/blob/master/examples/bayes_net_ex.cpp), for an inspiration of how to generate a BN. 

