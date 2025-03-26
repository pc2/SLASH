HLS_BUILD_DIR_OFFSET=build_offset.xcv80-lsva4737-2MHP-e-S
HLS_BUILD_DIR_DMA=build_dma.xcv80-lsva4737-2MHP-e-S
DESIGN_NAME=01_example
HOME_DIR=$(shell realpath .)
BUILD_DIR=$(shell realpath ./build)
HLS_DIR=$(shell realpath ./hls)
V80PP_PATH=$(shell realpath ../../submodules/v80-vitis-flow)
VPP_DIR=$(BUILD_DIR)/v80-vitis-flow

.PHONY: all setup hls hw emu sim app clean

all: setup hls hw app

emu_all: setup hls emu app

sim_all: setup hls sim app

hw_all: setup hls hw app

setup:
	mkdir -p $(BUILD_DIR)
	cp -r $(V80PP_PATH) $(BUILD_DIR)

hls:
	@echo "Running HLS step"
	$(MAKE) -C $(HLS_DIR)

hw: setup hls
	@echo "Running HW step"
	(cd $(VPP_DIR) && \
	./scripts/v80++ --design-name $(DESIGN_NAME) --cfg $(HOME_DIR)/config.cfg --platform hw --segmented --kernels $(HLS_DIR)/$(HLS_BUILD_DIR_OFFSET)/sol1 $(HLS_DIR)/$(HLS_BUILD_DIR_DMA)/sol1 && \
	cp build/$(DESIGN_NAME)_hw.vrtbin $(BUILD_DIR))

emu: setup hls
	@echo "Running EMU step"
	(cd $(VPP_DIR) && \
	./scripts/v80++ --design-name $(DESIGN_NAME) --cfg $(HOME_DIR)/config.cfg --platform emu --segmented --kernels $(HLS_DIR)/$(HLS_BUILD_DIR_OFFSET)/sol1 $(HLS_DIR)/$(HLS_BUILD_DIR_DMA)/sol1 && \
	cp build/$(DESIGN_NAME)_emu.vrtbin $(BUILD_DIR))

sim: setup hls
	@echo "Running SIM step"
	(cd $(VPP_DIR) && \
	./scripts/v80++ --design-name $(DESIGN_NAME) --cfg $(HOME_DIR)/config.cfg --platform sim --segmented --kernels $(HLS_DIR)/$(HLS_BUILD_DIR_OFFSET)/sol1 $(HLS_DIR)/$(HLS_BUILD_DIR_DMA)/sol1 && \
	cp build/$(DESIGN_NAME)_sim.vrtbin $(BUILD_DIR))

app: setup
	@echo "Running user app build step"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && \
	cmake .. && \
	make -j9
	@echo "Setting LD_LIBRARY_PATH"
	export LD_LIBRARY_PATH=$$(dirname $$(which vivado))/../../lib/lnx64.o:$$LD_LIBRARY_PATH
	@echo "Setting PATH"
	export PATH=$$PATH:/usr/local/sbin
clean:
	rm -rf $(BUILD_DIR)