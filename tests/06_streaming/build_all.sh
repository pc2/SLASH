#!/bin/bash
HLS_BUILD_DIR_STREAMING=build_streaming.xcv80-lsva4737-2MHP-e-S
DESIGN_NAME=06_streaming
HOME_DIR=$(realpath .)
BUILD_DIR=$(realpath ./build)
HLS_DIR=$(realpath ./hls)
V80PP_PATH=$(realpath ../../submodules/v80-vitis-flow)

mkdir -p build
cd build
cp -r $V80PP_PATH .

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
