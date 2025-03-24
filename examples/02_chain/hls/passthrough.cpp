#include <ap_fixed.h>
#include <hls_stream.h>

void passthrough(hls::stream<ap_uint<64>> &axis_in, hls::stream<ap_uint<64>> &axis_out) {
    #pragma HLS INTERFACE axis port=axis_in
    #pragma HLS INTERFACE axis port=axis_out
    #pragma HLS INTERFACE ap_ctrl_none port=return

    ap_uint<64> data;
    while (true) {
        #pragma HLS PIPELINE II=1
        if (!axis_in.empty()) {
            data = axis_in.read();
            axis_out.write(data);
        }
    }
}