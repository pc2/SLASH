TARGET=ip
DEVICE=xcv80-lsva4737-2MHP-e-S

all:
	vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) dma_in
	vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) passthrough
	vitis_hls build.tcl -tclargs $(TARGET) $(DEVICE) dma_out

clean:
	rm -r build_* vitis_hls.log