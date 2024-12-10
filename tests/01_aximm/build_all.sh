#!/bin/bash

V80PP_GIT="git@gitenterprise.xilinx.com:aulmamei/v80-vitis-flow.git"
AVED_GIT="https://github.com/Xilinx/AVED.git"
AVED_COMMIT_ID="7497599d2b452846515d7f2f22ad6bea2ebef522"
HLS_BUILD_DIR_ACCUMULATE=build_offset.xcv80-lsva4737-2MHP-e-S
HLS_BUILD_DIR_INCREMENT=build_dma.xcv80-lsva4737-2MHP-e-S
DESIGN_NAME=01_example
HOME_DIR=$(realpath .)
BUILD_DIR=$(realpath ./build)
HLS_DIR=$(realpath ./hls)
RESOURCES_DIR=$(realpath ../resources)
VRT_DIR=$(realpath $HOME_DIR/../../.)

# cloning repositories step

mkdir -p build
cd build
git clone $AVED_GIT
cd AVED
git checkout $AVED_COMMIT_ID
cd ..
git clone $V80PP_GIT

VPP_DIR=$(realpath $HOME_DIR/build/v80-vitis-flow)
AVED_DIR=$(realpath $HOME_DIR/build/AVED)
AVED_HW=$(realpath $AVED_DIR/hw/amd_v80_gen5x8_24.1)
AVED_IPREPO=$(realpath $AVED_DIR/hw/amd_v80_gen5x8_24.1/src/iprepo)

# hls build step
pushd ${HLS_DIR}
    make
    cp -r $HLS_BUILD_DIR_ACCUMULATE/ $AVED_IPREPO
    cp -r $HLS_BUILD_DIR_INCREMENT/ $AVED_IPREPO
    cp -r $RESOURCES_DIR/smbus_v1_1-20240328 $AVED_IPREPO
popd

# v80++ build step & linking to AVED
pushd ${BUILD_DIR}
    
    mkdir -p $VPP_DIR/build
    cd $VPP_DIR/build
    cmake ..
    make -j9
    ./v80++ --cfg $HOME_DIR/config.cfg --kernels $HLS_DIR/$HLS_BUILD_DIR_INCREMENT/sol1 $HLS_DIR/$HLS_BUILD_DIR_ACCUMULATE/sol1
    cp block_design.tcl $AVED_DIR/hw/amd_v80_gen5x8_24.1/src/bd/create_bd_design.tcl
    
popd
# hw build
pushd ${AVED_HW}
    ./build_all.sh
popd

# API build
pushd ${VRT_DIR}
    mkdir -p build && cd build
    cmake ..
    make -j9
popd

# user app build
pushd ${HOME_DIR}
    mkdir -p build && cd build
    cmake ..
    make -j9
#    cp $VPP_DIR/build/system_map.xml .
popd

# vrtbin creation
pushd ${BUILD_DIR}
    cp $AVED_DIR/hw/amd_v80_gen5x8_24.1/build/amd_v80_gen5x8_24.1_nofpt.pdi design.pdi
    cp $VPP_DIR/build/system_map.xml system_map.xml
    tar -cvf $DESIGN_NAME.vrtbin system_map.xml design.pdi
popd