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
## Deployment

In order to build and deploy all necessary software for this project to work, follow the steps shown in `deploy/README.md`.

## How to build the examples

A README file can be found in the examples directory. Please follow the instruction given there to proceed.

## Known limitations
- HLS arguments should not be Verilog or VHDL keywords (eg. `in`, `out` so on). Some issues may appear in the linker with this configuration.
- In emulation, HLS kernels must include at least one axi4lite interface to work.
- In the current version of the linker (`submodules/v80-vitis-flow`), a maximum number of 15 kernels can be included. This will be fixed in future versions.

## Notes
All the hardware examples are built using the Segmented Configuration flow.
