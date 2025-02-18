#include <ap_fixed.h>
#include <hls_stream.h>

void streaming(hls::stream<ap_uint<512>>& axis_in, ap_uint<512>* out, ap_uint<32> size) {
    #pragma hls interface mode=s_axilite port=size
    #pragma hls interface mode=s_axilite port=return
    #pragma hls interface mode=axis port=axis_in
    #pragma hls interface m_axi bundle=gmem0 port=out max_widen_bitwidth=64
    for (ap_uint<32> i = 0; i < size; i++) {
        ap_uint<512> tmp_val = axis_in.read();
        out[i] = tmp_val;

    }
}