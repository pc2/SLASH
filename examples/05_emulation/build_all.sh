#!/bin/bash
HLS_BUILD_DIR_ACCUMULATE=build_offset.xcv80-lsva4737-2MHP-e-S
HLS_BUILD_DIR_INCREMENT=build_dma.xcv80-lsva4737-2MHP-e-S
DESIGN_NAME=05_emulation
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

PLATFORM="sim"

echo "Running HW step"
pushd ${VPP_DIR}
    ./scripts/v80++ --design-name $DESIGN_NAME --cfg $HOME_DIR/config.cfg --segmented --platform $PLATFORM --kernels $HLS_DIR/$HLS_BUILD_DIR_ACCUMULATE/sol1 $HLS_DIR/$HLS_BUILD_DIR_INCREMENT/sol1
    cp build/${DESIGN_NAME}_${PLATFORM}.vrtbin $BUILD_DIR
popd

# user app build
echo "Running user app build step"
pushd ${HOME_DIR}
    mkdir -p build && cd build
    cmake ..
    make -j9
#    cp $VPP_DIR/build/system_map.xml .
popd
