TARGET=ip
DEVICE=xcv80-lsva4737-2MHP-e-S

STREAMING_BUILD_DIR=build_streaming.$(DEVICE)

all: $(STREAMING_BUILD_DIR)

$(STREAMING_BUILD_DIR):
	if [ ! -d "$(STREAMING_BUILD_DIR)" ]; then \
        vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) streaming; \
	fi

clean:
	rm -rf $(STREAMING_BUILD_DIR) vitis_hls.log