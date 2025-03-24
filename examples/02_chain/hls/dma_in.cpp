#include <ap_fixed.h>
#include <hls_stream.h>

void dma_in(ap_uint<64>* in, hls::stream<ap_uint<64> >& axis_out, ap_uint<32> size) {
    #pragma hls interface mode=s_axilite port=size
    #pragma hls interface mode=axis port=axis_out
    #pragma hls interface m_axi bundle=gmem0 port=in max_widen_bitwidth=64
    #pragma hls interface mode=s_axilite port=return

    for (ap_uint<32> i = 0; i < size; i++) {
        #pragma HLS PIPELINE II=1
        axis_out.write(in[i]);
    }
}
