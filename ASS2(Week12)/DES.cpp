#include "DES.h"

uint64_t IP_function(uint64_t plaintext) {
    uint64_t ket_qua = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t bit = (plaintext >> (64 - IP[i])) & 1;
        ket_qua = (ket_qua << 1) | bit;
    }
    return ket_qua;
}

uint64_t IP_inverse_function(uint64_t input) {
    uint64_t ket_qua = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t bit = (input >> (64 - IP_INV[i])) & 1;
        ket_qua = (ket_qua << 1) | bit;
    }
    return ket_qua;
}

uint64_t E_function(uint32_t input) {
    uint64_t ket_qua = 0;
    for (int i = 0; i < 48; i++) {
        uint64_t bit = (input >> (32 - E[i])) & 1;
        ket_qua = (ket_qua << 1) | bit;
    }
    return ket_qua;
}