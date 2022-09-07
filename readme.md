# Optical Simulation for TRIDENT Pathfinder Experiment

This program is used to study the observed optical phenomena in the TRIDENT Pathfinder Experiment. It can simulate photon propagation in the medium and record the kinematic status of photons. The program is developed based on Geant4 toolkit.

## Requirements

Following libraries are required to build and run this simulation program.

- Geant4 toolkit >= 10.6
- yaml-cpp

You can find [Geant4 installation guide here](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html). The yaml-cpp library can be installed by apt-get on Ubuntu or homebrew in MacOS.

## How to Use

The build and install process is rather simple:

``` shell
cd ${SOURCE_DIR}
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
make -j8 && make install
```

Then, you will find an excutable `OpticalSimulation` and a default configure file `config.yaml` in your `${INSTALL_DIR}` directory. 

You can run the program using the default configure file by:

```
./OpticalSimulation config.yaml
```