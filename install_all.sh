# ##################################################################################################
#  The MIT License (MIT)
#  Copyright (c) 2025 Advanced Micro Devices, Inc. All rights reserved.
# 
#  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
#  and associated documentation files (the "Software"), to deal in the Software without restriction,
#  including without limitation the rights to use, copy, modify, merge, publish, distribute,
#  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in all copies or
#  substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# ##################################################################################################

#!/bin/bash

XILINX_TOOLS_VERSION=2024.2
KERNEL_VERSION=5.15
LOG_FILE="install.log"


rm -rf $LOG_FILE
exec > >(tee -a "$LOG_FILE") 2>&1

echo 'Checking kernel version...'
if ! uname -r | grep -q "$KERNEL_VERSION"; then
    echo "Kernel version does not match the required version $KERNEL_VERSION. Please use kernel version 5.15.*."
    exit 1
fi
echo 'Kernel version passed: '$(uname -r)
echo ''

echo 'Checking Vitis installation...'
if ! which vitis > /dev/null 2>&1; then
    echo "Vitis suite is not sourced or not installed. Please source Vitis or install it before proceeding."
    exit 1
fi
echo 'Vitis found: '$(which vitis)
echo ''
echo 'Checking Vivado installation...'
if ! which vivado > /dev/null 2>&1; then
    echo "Vivado suite is not sourced or not installed. Please source Vivado or install it before proceeding."
    exit 1
fi
echo 'Vivado found: '$(which vivado)
echo ''
echo 'Checking Vivado version'
VIVADO_VERSION=$(vivado -version | grep -oP 'vivado v\K[0-9]+\.[0-9]+')
if [ "$VIVADO_VERSION" != "$XILINX_TOOLS_VERSION" ]; then
    echo "Vivado version $VIVADO_VERSION does not match the required version $XILINX_TOOLS_VERSION.
     Please install the correct version or update your PATH."
    exit 1
fi
echo 'Vivado version passed: '$VIVADO_VERSION
echo ''
echo 'Checking Vitis version'
VITIS_VERSION=$(vitis -help | grep -oP 'Vitis v\K[0-9]+\.[0-9]+')
if [ "$VITIS_VERSION" != "$XILINX_TOOLS_VERSION" ]; then
    echo "Vitis version $VITIS_VERSION does not match the required version $XILINX_TOOLS_VERSION. 
     Please install the correct version or update your PATH."
    exit 1
fi
echo 'Vitis version passed: '$VITIS_VERSION

echo 'Updating submodules...'
git submodule update --init --recursive

PACKAGE_NAME="xrt"
echo -n "You are about to uninstall package $PACKAGE_NAME. Are you sure you want to continue? [Y/n] "
read -r response
case "$response" in
    [yY][eE][sS]|[yY])
        echo "Uninstalling package $PACKAGE_NAME..."
        sudo apt remove $PACKAGE_NAME -y 
        ;;
    *)
        echo "Aborted."
        exit 0
        ;;
esac

echo 'Installing AVED...'
sudo apt install  ./submodules/v80-vitis-flow/submodules/aved/deploy/ami*.deb -y
if ! which ami_tool > /dev/null 2>&1; then
    echo "AVED installation failed. Please check the installation logs."
    exit 1
fi
echo 'AVED found: '$(which ami_tool)
ami_tool --version

echo 'Compiling and installing PCIe hotplug driver...'

cd ./submodules/pcie-hotplug-drv && make && sudo make install > /dev/null && make clean &&cd -

for device in /dev/pcie_hotplug*; do
    if [ -e "$device" ]; then
        bdf=$(basename "$device" | sed 's/pcie_hotplug_//')
        echo "V80 device found with BDF: $bdf"
    fi
done

echo 'Installing dependencies...'
sudo apt install libxml2-dev libzmq3-dev libjsoncpp-dev -y

echo 'Compiling and installing VRT...'

mkdir -p vrt/build && cd vrt/build
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j9
sudo make install

mkdir -p smi/build && cd smi/build
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j9
sudo make install

if ! locate libvrt.so | grep -q '/usr/local/lib/libvrt.so'; then
    echo "VRT installation failed. Please check the installation logs."
    exit 1
fi

echo "VRT installation successful."