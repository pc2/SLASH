TARGET=ip
DEVICE=xcv80-lsva4737-2MHP-e-S

DMA_IN_BUILD_DIR=build_dma_in.$(DEVICE)
PASSTHROUGH_BUILD_DIR=build_passthrough.$(DEVICE)
DMA_OUT_BUILD_DIR=build_dma_out.$(DEVICE)

all: $(DMA_IN_BUILD_DIR) $(PASSTHROUGH_BUILD_DIR) $(DMA_OUT_BUILD_DIR)

$(DMA_IN_BUILD_DIR):
	if [ ! -d "$(DMA_IN_BUILD_DIR)" ]; then \
        vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) dma_in; \
	fi

$(PASSTHROUGH_BUILD_DIR):
	if [ ! -d "$(PASSTHROUGH_BUILD_DIR)" ]; then \
        vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) passthrough; \
	fi

$(DMA_OUT_BUILD_DIR):
	if [ ! -d "$(DMA_OUT_BUILD_DIR)" ]; then \
        vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) dma_out; \
	fi

clean:
	rm -rf $(DMA_IN_BUILD_DIR) $(PASSTHROUGH_BUILD_DIR) $(DMA_OUT_BUILD_DIR) vitis_hls.log