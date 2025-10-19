libBbMm - Bayesian-based Markov-models
======================================================================

![](./resources/logo-BbMm-128.png)

BbMm, aims to become a multi-language library to create deliberative agent based on Decision Making under Uncertainty.
The implemented approach for that relies on Markov Decision Process with transition based on Bayesian Network model.

libBbMm is the core _C_ experimental toolbox designed to be wraped in any other programing language.
It comes with no more guaranty than the devotion of the developers.

- Vertion: 0.2.1
- Author: Guillaume Lozenguez

## Installation

_libBbMm_ is a C librairie with a minimum of dependancies (none, exept for the standard C librarie).
In fact, the compilation of _libBbMm_ relies only on build and testing tools.

The instruction and support is providing only for _Linux_ based machine.
For conveignance, command is provided for debian like distribution.

### Dependancies :

The construction of the librairy relies on `Cmake`.
Also, `Check` is used for test-driven developments.
You can compile the librairy without `Check` installed.

```sh
sudo apt install build-essential cmake check
```

### Compilation and Install

As said, compilation relies on `cmake`:

From _libBbMm_ directory:

```sh
cmake . # for testing: `cmake -DDEV_MODE=ON .` (requires check)
make
```

This should generate the `libbbmm.so.x.y.z` librairy (and the `run-test-BbMm` executable with the `DEV_MODE` flags).

Then a `sudo make install` will install the lib globally on your computer, and make it accecible by with a `find_package(BbMm REQUIRED)`.

If you wan to make the librairy recognized by ldconfig, you should add the `"/usr/local/lib"` path `ld` configurations:

```sh
echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/local.conf
ldconfig -p | grep libBbMm
```

## Get started
