# VRT (V80 RunTime) API

This repository contains the VRT API implementation, along with examples.

The project was tested with AMD Vivado & Vitis tools version 2024.2, kernel version 5.15.
## Dependencies

- libxml2
- ZeroMQ (zmq) for emulation & simulation
- jsoncpp for emulation & simulation

To install the dependencies:

```bash
sudo apt install libxml2-dev libzmq3-dev libjsoncpp-dev
```

### Submodules
VRT depends on the AVED software stack and the v80++ linker. These can be pulled from git using:
```bash
git submodule update --init --recursive
```

### AVED software stack
To install the AVED software stack, one must run the following:

```bash
cd submodules/v80-vitis-flow/submodules/aved/deploy/
sudo apt install ./ami_<version>_<arch>_<os version>.deb
```

### PCIe hotplug driver

Please consult the driver's Github page for instructions.


## How to build and install the API

To build the API and install it, one must do the following:
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
sudo cmake --install .
```
This will add the library and headers to `/usr/local/vrt`.

## How to build v80-smi, the utility app
To build v80-smi and install it, one must run:

```bash
cd app
mkdir -p build && cd build
cmake ..
cmake --build .
sudo cmake --install .
```
The executable is located in /usr/local/bin.

## How to build the examples

A README file can be found in the examples directory. Please follow the instruction given there to proceed.

## Known limitations
- HLS arguments should not be Verilog or VHDL keywords (eg. `in`, `out`) so on. Some issues may appear in the linker with this configuration.
- In emulation, only HLS functions with axi4lite interfaces work.

## Notes
All the hardware examples are built using the Segmented Configuration flow.
