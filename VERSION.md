# version trace...

## v0.21

* Unique header `bbmm.h`.
* BbMm Functions
       - BmCondition     : Define a Bayesian Node (conditional probabilities over variable affectations)
       - BmInferencor    : Define a Bayesian Network (potentially Dynamic) composed of Input, Output and Shift node. 
       - BmEvaluator     : a composed value function over multiple Gauges
* BbMm Models
       - BmChain         : Markov Chain $<S, T>$
       - BmProcess       : Define a classical Markov Decision Process $<S, A, t, r>$
* BbMm Solvers
       - BmBasicPolicy   : Basic algorythm to solve BmProcess, generate basicly defined policy.
       - BmBasicQLearner : ...
       - BmStockasticPolicy :
       - ... 
* Test cases: 421, CofeeRobot, Labyrinth test case...

## v0.20

Restructure: 

* BbMm Structures - data structures.
       - BmCode         : a fixed size list of digit (unsigned integers)
       - BmBench        : a dynamic-size collection of BmCode with tag and value (i -> code + tag and value )
       - BmTree         : a tree based BmCode (BmCode -> tag and value )
       - BmVector       : a fixed size list of values (doubles)
* BbMm Model - Bayesian-based Markov-models
       - BmCondition    : Define a Bayesian Node (conditional probabilities over variable affectations)
       - BmInferencor   : Define a Bayesian Network (potentially Dynamic) composed of Input, Output and Shift node. 
       - BmEvaluator    : a composed value function over multiple Gauges
* BbMm Solver - Solvers..
       - BmValueIt      : ...
       - BmPolicyIt     : ...

## v0.13

* BbMm Structures - A library in KISS philosophy for *Bayesian-based Markov-models* data structures.
       - BmCode         : a fixed size list of digit (unsigned integers)
       - BmBench        : a dynamic-size collection of BmCode
       - BmTree         : a decision tree based BmCode (BmCode -> uint)
       - BmNet          : a adjacence list network based on unsigned integers
* BbMm Model - A library in KISS philosophy for *Bayesian-based Markov-models* creation and manipulation.
       - BmDomain       : Definie e finit set of possible values (words, integer-interval, cluster-means, .... )
       - BmSpace        : a vector of variable in a collection of domains
       - BmModel        : `<S, A, t, r>`
* BbMm Probability - A library in KISS philosophy of *Bayesian-based Markov-models* probability manipulation.
       - BmDistribution : Define a distribution of probabilities over configuration (codes)
       - BmCondition    : Define a Bayesian Node (conditional probabilities over variable affectations)
       - BmInferer   : Define a Dynamic Bayesian Network composed of state, action and tramsitional nodes 
* BbMm Evaluation - A library in KISS philosophy of *Bayesian-based Markov-models* values manipulation.
       - BmVector       : a fixed size collection of values.
       - BmGauge        : a distribution of values over configuration (codes)
       - BmReward       : a composed value function over multiple Gauges.
* BbMm Solver - A library in KISS philosophy for *Bayesian-based Markov-models* solvers.
       - WdValueIteration or WdSolver     : ...
