libBbMm - a librayry for Bayesian-based Markov-models
======================================================================

![](./resources/logo-BbMm-128.png)

BbMm, aims to become a multi-language library to create deliberative agent based on Decision Making under Uncertainty.
The implemented approach for that relies on Markov Decision Process with transition based on Bayesian Network model.

libBbMm is the core _C_ experimental toolbox designed to be wraped in any other programing language.
It comes with no more guaranty than the devotion of the developers.

- Vertion: 0.2X
- Author: Guillaume Lozenguez

## Installation

_libBbMm_ is a C librairie with a minimum of dependancies (none, exept for the standard C librarie).
In fact, the compilation of _libBbMm_ relies only on build and testing tools.

The instruction and support is providing only for Linux based machine.
For conveignance, command is provided for debian like distribution.

### Dependancies :

The construction of the librairy relies on `Cmake`.
Also, `Check` is used for test-driven developments.

```sh
sudo apt install build-essential cmake check # check is for testing, but not required.
```

### Compilation

As said compilation relies on `cmake`:

From _libBbMm_ directory:

```sh
mkdir build
cmake . # for testing: `cmake -DTEST=ON -DCMAKE_BUILD_TYPE=Debug . ` (requires check)
make
sudo make install
```

The `libbbmm.so` librairy and the `run-test-BbMm` executable should be generated into the `build` directory.
You can run the `run-test-BbMm`.


## Hello World


