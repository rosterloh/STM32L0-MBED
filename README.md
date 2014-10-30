# STM32L0-MBED

Build system for the [STM32L0 Discovery](http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/LN1848/PF260319?sc=stm32l0-discovery) and [MBED](http://mbed.org/)

### Build Dependencies
* [GNU Make](http://www.gnu.org/software/make/)
* [CMake](http://www.cmake.org/)
* [GNU Tools for ARM Embedded Processors](https://launchpad.net/gcc-arm-embedded/)

## Setup Requirements

For Windows development the easiest way to get this set up is with [Chocolatey](https://chocolatey.org/). One installed setup the dependencies as follows:

```bash
$ cinst git make cmake gcc-arm-embedded
$ git clone https://github.com/rosterloh/STM32L0-MBED
$ git submodule init
$ git submodule update
```

### Updating MBED sources

```bash
$ git submodule update --remote
```
###

```bash
$ cd projects\demo
$ make STM32L053
```

### Building with CMake [**BROKEN**]

```bash
$ cd projects\demo
$ mkdir build
$ cd build
$ cmake ..
$ make
```
