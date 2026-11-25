#include "AES.h"

uint8_t nhan_x(uint8_t a) {
    if (a & 0x80) {
        return (a << 1) ^ 0x1B;
    } else {
        return a << 1;                  // Tinh a*x trong GF(2^8) voi modulo = x^8 + x^4 + x^3 + x + 1
    }
}

uint8_t nhan_gf(uint8_t a, uint8_t b) {
    uint8_t ket_qua = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            ket_qua ^= a;
        }
        a = nhan_x(a);
        b >>= 1;
    }
    return ket_qua;                     // Tinh a*b trong GF(2^8)
}