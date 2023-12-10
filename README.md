libBbMm - a librayry for Bayesian-based Markov-models
======================================================================

![](./resources/logo-BbMm-128.png)

BbMm, aims to become a multi-language library to create deliberative agent based on Decision Making under Uncertainty.
The implemented approach for that relies on Markov Decision Process with transition based on Bayesian Network model.

BbMm is an experimental toolbox, it comes with no more guaranty than the devotion of the developers.

- Vertion 0.2X


## Installation

_BbMm_ is mainly a C librairie with wrapers in differents languages, nativelly Python and (soon) Pharo.
It requires the compilation of the _c_ librairie (core).

The instruction and support is providing only for Linux based machine.
For conveignance, command is provided for debian like distribution.

### Dependancies :

The construction of the librairy relies on `Cmake`.
Also, `Check` is used for test-driven developments.

```sh
sudo apt install python3 build-essential cmake check
pip install pytest
```


### Compilation

As said compilation relies on `cmake`:

From _libBbMm_ directory:

```sh
mkdir build
cd build
cmake .. # for testing add: -DTEST=ON -DCMAKE_BUILD_TYPE=Debug
make
sudo make install
```

the `libbbmm.so` librairy and the `run-test-BbMm` executable should be generated into a `build` directory, 
You can run the `run-test-BbMm`.



### Python

Python wrapper suppose that you built `libbbbmm.so` (cf. section: **Compilation**) . 
Then the python wrapper can be tested using pytest

```sh
pytest python-src
```

### Pharo

comming soon


## Get-in



## Contribution guidelines

* Writing tests
* Code review
* Other guidelines








### How do I get set up?

* Summary of set up
* Configuration
* Dependencies
* Database configuration
* How to run tests
* Deployment instructions

### Who do I talk to?

* Repo owner or admin
* Other community or team contact



