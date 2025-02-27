TARGET=ip
DEVICE=xcv80-lsva4737-2MHP-e-S

DMA_BUILD_DIR=build_dma.$(DEVICE)
OFFSET_BUILD_DIR=build_offset.$(DEVICE)

all: $(DMA_BUILD_DIR) $(OFFSET_BUILD_DIR)

$(DMA_BUILD_DIR):
	if [ ! -d "$(DMA_BUILD_DIR)" ]; then \
		vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) dma; \
	fi

$(OFFSET_BUILD_DIR):
	if [ ! -d "$(OFFSET_BUILD_DIR)" ]; then \
		vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) offset; \
	fi

clean:
	rm -rf $(DMA_BUILD_DIR) $(OFFSET_BUILD_DIR) vitis_hls.log