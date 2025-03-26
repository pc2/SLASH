TARGET=ip
DEVICE=xcv80-lsva4737-2MHP-e-S

VADD_BUILD_DIR=build_vadd.$(DEVICE)

all: $(VADD_BUILD_DIR)

$(VADD_BUILD_DIR):
	if [ ! -d "$(VADD_BUILD_DIR)" ]; then \
		vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) vadd; \
	fi

clean:
	rm -rf $(VADD_BUILD_DIR) vitis_hls.log