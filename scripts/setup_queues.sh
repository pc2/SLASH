#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <BDF>"
    exit 1
fi

BDF=$1
BDF_PCI="${BDF/.0/.1}" # Replace .0 with .1 for the PCI path

# Extract bus and device parts, remove colon, and add 1 at the end
BDF_QDMA="${BDF:0:2}${BDF:3:2}1"

echo "Starting QDMA queue..." >> /var/log/setup_queues.log

echo "Setting qmax..." >> /var/log/setup_queues.log
echo 100 > /sys/bus/pci/devices/0000\:${BDF_PCI}/qdma/qmax 

echo "Adding queue..." >> /var/log/setup_queues.log
dma-ctl qdma${BDF_QDMA} q add idx 0 mode mm dir bi >> /var/log/setup_queues.log
sleep 1
dma-ctl qdma${BDF_QDMA} q start idx 0 idx_ringsz 15 dir bi >> /var/log/setup_queues.log
sleep 1
chmod 666 /dev/qdma${BDF_QDMA}-MM-0
