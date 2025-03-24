# VRT (V80 RunTime) API

This repository contains the VRT API implementation, along with examples.

The project was tested with AMD Vivado & Vitis tools version 2024.2.
## Dependencies

- AVED software stack (https://gitenterprise.xilinx.com/aulmamei/aved-fork)
- PCIe hotplug driver (https://gitenterprise.xilinx.com/aulmamei/pcie-hotplug-drv)
- libxml2
- ZeroMQ (zmq) for emulation
- jsoncpp for emulation

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


To build an example, one must do the following:

```bash
cd tests/<XX>_example/
./build_all.sh
```

This will build the hardware design for the FPGA, using the HLS kernels provided in the `hls/` directory. Beware, this takes around one hour to finish.

## How to run

The following environment variable needs to be set prior to running any examples:

```
mkdir -p /home/<user>/.ami
export AMI_HOME="/home/<user>/.ami"
source <VIVADO>
source <VITIS>
```
To make the changes persistent, add the commands to .bashrc. Sourcing the Vivado scripts are needed for the hardware builds, whereas vitis is needed for emulation.

In order to run one of the built examples, one must identify the BDF for the V80 and input it into the code.

```
v80-smi list
--------------------------------------------------------------------
Listing V80 devices 
--------------------------------------------------------------------
V80 device found with BDF: 0000:e2:00.0
--------------------------------------------------------------------
V80 device found with BDF: 0000:21:00.0
--------------------------------------------------------------------
```

Then, in the example C++ code (`tests/<0X_example/<0X>_example.cpp>`), change the following line with the found BDF number:

```C++
vrt::Device device("21:00.0", "0X_example_hw.vrtbin");
```

Navigate to the `build` directory, then run:

```bash
make
```
This will re-build the host application. After that, run the example.


## Notes
The program type can be either FLASH or JTAG. We recommend using JTAG. Example 04 shows segmented configuration build, which is a dynamic PL reconfiguration example. For that, the flag doesn't have any attribute.

This software depends on the PCIe hotplug driver (https://gitenterprise.xilinx.com/aulmamei/pcie-hotplug-drv) being installed prior to the running of the software.
