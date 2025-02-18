#!/bin/bash
V80PP_GIT="git@gitenterprise.xilinx.com:aulmamei/v80-vitis-flow.git"
VPP_COMMIT_ID="cbfcc9eea9a3e403e89a5028245ab7bccb96d050"

HLS_BUILD_DIR_STREAMING=build_streaming.xcv80-lsva4737-2MHP-e-S
DESIGN_NAME=06_streaming
HOME_DIR=$(realpath .)
BUILD_DIR=$(realpath ./build)
HLS_DIR=$(realpath ./hls)
RESOURCES_DIR=$(realpath ../resources)
VRT_DIR=$(realpath $HOME_DIR/../../.)

mkdir -p build
cd build
git clone $V80PP_GIT

VPP_DIR=$(realpath $HOME_DIR/build/v80-vitis-flow)

echo "Running HLS step"
pushd ${HLS_DIR}
    make
popd

echo "Running HW step"
pushd ${VPP_DIR}
    ./scripts/v80++ --design-name $DESIGN_NAME --cfg $HOME_DIR/config.cfg --segmented --platform emu --kernels $HLS_DIR/$HLS_BUILD_DIR_STREAMING/sol1
    cp build/${DESIGN_NAME}_emu.vrtbin $BUILD_DIR
popd

# user app build
echo "Running user app build step"
pushd ${HOME_DIR}
    mkdir -p build && cd build
    cmake ..
    make -j9
#    cp $VPP_DIR/build/system_map.xml .
popd
