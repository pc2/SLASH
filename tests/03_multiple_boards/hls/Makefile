TARGET=ip
DEVICE=xcv80-lsva4737-2MHP-e-S

INCREMENT_BUILD_DIR=build_increment.$(DEVICE)
ACCUMULATE_BUILD_DIR=build_accumulate.$(DEVICE)

all: $(INCREMENT_BUILD_DIR) $(ACCUMULATE_BUILD_DIR)

$(INCREMENT_BUILD_DIR):
	if [ ! -d "$(INCREMENT_BUILD_DIR)" ]; then \
        vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) increment; \
	fi

$(ACCUMULATE_BUILD_DIR):
	if [ ! -d "$(ACCUMULATE_BUILD_DIR)" ]; then \
        vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) accumulate; \
	fi

clean:
	rm -rf $(INCREMENT_BUILD_DIR) $(ACCUMULATE_BUILD_DIR) vitis_hls.log