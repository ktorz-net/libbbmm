BeMAge - a librayry for Bayesian-based Markovian Models
======================================================================

![](./resources/logo-BbMm-128.png)


Wanda, aims to become a multi-language library to create deliberative agent based on Decision Making under Uncertainty (mainly Markov Decision Process with transition based on Bayesian Network model).

Wanda is an experimental toolbox, it comes with no more guaranty than the devotion of the developers.


## Concurency:

**BeMAge** is not what you looking for ? Try those solutions:

- [gobnilp](https://www.cs.york.ac.uk/aig/sw/gobnilp/)


## Installation

_BeMAge_ is mainly a C librairie with wrapers in differents languages, nativelly Python and (soon) Pharo.
It requires the compilation of the _c_ librairie (core).

The instruction and support is providing only for Linux based machine.


### Dependancies :

_BeMAge_ is developped under linux platform and supports are provided only for linux platform.
For conveignance, command is provided for debian like distribution.

The construction of the librairy relies on `cmake`. Then `Check` is used for test-driven developments.

```sh
sudo apt install python3 build-essential cmake check
pip install pytest
```

### Compilation

As said compilation relies on `cmake`.
The `build.sh` script permits to build _BeMAge_ localy.

From _BeMAge_ directory:

```sh
./bin/build.sh
```

A `build` directory, the `libbemage.so` librairy and the `test-bemage` executable should be generated.
You can run the `test-bemage`.

```sh
./test-wanda
```

### Python

Python wrapper suppose that you built `libbemage.so` (cf. section: **Compilation**) . 
Then the python wrapper can be tested using pytest

```sh
pytest python-src
```

### Pharo

comming soon









### How do I get set up?

* Summary of set up
* Configuration
* Dependencies
* Database configuration
* How to run tests
* Deployment instructions

### Contribution guidelines

* Writing tests
* Code review
* Other guidelines

### Who do I talk to?

* Repo owner or admin
* Other community or team contact




## Concurency:

_BeMAge_ is not what you are looking for ? you can look at: 

- SaLinA : <https://arxiv.org/pdf/2110.07910.pdf> - <https://github.com/facebookresearch/salina>
- MDPToolBox :
