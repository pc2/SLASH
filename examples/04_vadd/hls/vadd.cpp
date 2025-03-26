#include <ap_fixed.h>
#include <ap_int.h>

void vadd(int* a, int* b, int* c, ap_uint<32> n) {
    #pragma hls interface mode=s_axilite port=n
    #pragma hls interface mode=s_axilite port=return
    #pragma hls interface m_axi bundle=gmem0 port=a max_widen_bitwidth=64
    #pragma hls interface m_axi bundle=gmem1 port=b max_widen_bitwidth=64
    #pragma hls interface m_axi bundle=gmem2 port=c max_widen_bitwidth=64

    for(int i = 0; i < n; i++) {
        #pragma HLS unroll factor=4
        c[i] = a[i] + b[i];
    }
}