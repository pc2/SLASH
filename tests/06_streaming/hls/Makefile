TARGET=ip
DEVICE=xcv80-lsva4737-2MHP-e-S

all:
	vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) streaming

clean:
	rm -r build_* vitis_hls.log