# version trace...

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
       - BmTransition   : Define a Dynamic Bayesian Network composed of state, action and tramsitional nodes 
* BbMm Evaluation - A library in KISS philosophy of *Bayesian-based Markov-models* values manipulation.
       - BmVector       : a fixed size collection of values.
       - BmGauge        : a distribution of values over configuration (codes)
       - BmReward       : a composed value function over multiple Gauges.
* BbMm Solver - A library in KISS philosophy for *Bayesian-based Markov-models* solvers.
       - WdValueIteration or WdSolver     : ...
