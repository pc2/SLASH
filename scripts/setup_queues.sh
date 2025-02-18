#!/bin/bash

# Function to print usage
usage() {
    echo "Usage: $0 <BDF> [--mm <queue_id> <direction>] [--st <queue_id>] [--dir <direction>]"
    exit 1
}

# Check if at least one argument is provided
if [ $# -lt 1 ]; then
    usage
fi

BDF=$1
shift

BDF_PCI="${BDF/.0/.1}" # Replace .0 with .1 for the PCI path
BDF_QDMA="${BDF:0:2}${BDF:3:2}1"

echo "Starting QDMA queue..." >> /var/log/setup_queues.log
echo "Setting qmax..." >> /var/log/setup_queues.log
echo 4096 > /sys/bus/pci/devices/0000\:${BDF_PCI}/qdma/qmax 

# Parse the remaining arguments
while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        --mm)
        QUEUE_ID="$2"
        MM_DIRECTION="$3"
        echo "Adding MM queue with ID $QUEUE_ID and direction $MM_DIRECTION..." >> /var/log/setup_queues.log
        dma-ctl qdma${BDF_QDMA} q add idx $QUEUE_ID mode mm dir $MM_DIRECTION >> /var/log/setup_queues.log
        sleep 1
        dma-ctl qdma${BDF_QDMA} q start idx $QUEUE_ID idx_ringsz 15 dir $MM_DIRECTION >> /var/log/setup_queues.log
        sleep 1
        chmod 666 /dev/qdma${BDF_QDMA}-MM-$QUEUE_ID
        shift # past argument
        shift # past value
        shift # past direction
        ;;
        --st)
        QUEUE_ID="$2"
        shift # past argument
        shift # past value
        ;;
        --dir)
        DIRECTION="$2"
        echo "Adding Stream queue with ID $QUEUE_ID and direction $DIRECTION..." >> /var/log/setup_queues.log
        dma-ctl qdma${BDF_QDMA} q add idx $QUEUE_ID mode st dir $DIRECTION >> /var/log/setup_queues.log
        sleep 1
        dma-ctl qdma${BDF_QDMA} q start idx $QUEUE_ID idx_ringsz 15 dir $DIRECTION >> /var/log/setup_queues.log
        sleep 1
        chmod 666 /dev/qdma${BDF_QDMA}-ST-$QUEUE_ID
        shift # past argument
        shift # past value
        ;;
        *)
        echo "Unknown option $key"
        usage
        ;;
    esac
done