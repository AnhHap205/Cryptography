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

// Thay gtri byte trong state bang gtri Sbox
void sub_bytes(uint8_t state[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] = Sbox[state[i]];    
    }
}

void inv_sub_bytes(uint8_t state[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] = INV_SBOX[state[i]];
    }
}

// Shiftrows
void shift_rows(uint8_t state[16]) {
    uint8_t temp[16];
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            temp[4 * c + r] = state[4 * ((c + r) % 4) + r];             // Dich trai r byte hang r
    for (int i = 0; i < So_byte_khoi; i++) state[i] = temp[i];
}

void inv_shift_rows(uint8_t state[16]) {
    uint8_t temp[16];
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            temp[4 * c + r] = state[4 * ((c - r + 4) % 4) + r];         // Dich phai r byte hang r
    for (int i = 0; i < So_byte_khoi; i++) state[i] = temp[i];
}

// Mixcolumns
void mix_columns (uint8_t state[16]) {
    uint8_t temp[16];
    for (int c = 0; c < 4; c++) {
        temp[4 * c + 0] = nhan_gf(0x02, state[4 * c + 0]) ^ nhan_gf(0x03, state[4 * c + 1]) ^ state[4 * c + 2] ^ state[4 * c + 3];
        temp[4 * c + 1] = state[4 * c + 0] ^ nhan_gf(0x02, state[4 * c + 1]) ^ nhan_gf(0x03, state[4 * c + 2]) ^ state[4 * c + 3];
        temp[4 * c + 2] = state[4 * c + 0] ^ state[4 * c + 1] ^ nhan_gf(0x02, state[4 * c + 2]) ^ nhan_gf(0x03, state[4 * c + 3]);
        temp[4 * c + 3] = nhan_gf(0x03, state[4 * c + 0]) ^ state[4 * c + 1] ^ state[4 * c + 2] ^ nhan_gf(0x02, state[4 * c + 3]);
    }
    for (int i = 0; i < So_byte_khoi; i++) state[i] = temp[i];
}

void inv_mix_columns (uint8_t state[16]) {
    uint8_t temp[16];
    for (int c = 0; c < 4; c++) {
        temp[4 * c + 0] = nhan_gf(0x0E, state[4 * c + 0]) ^ nhan_gf(0x0B, state[4 * c + 1]) ^ nhan_gf(0x0D, state[4 * c + 2]) ^ nhan_gf(0x09, state[4 * c + 3]);
        temp[4 * c + 1] = nhan_gf(0x09, state[4 * c + 0]) ^ nhan_gf(0x0E, state[4 * c + 1]) ^ nhan_gf(0x0B, state[4 * c + 2]) ^ nhan_gf(0x0D, state[4 * c + 3]);
        temp[4 * c + 2] = nhan_gf(0x0D, state[4 * c + 0]) ^ nhan_gf(0x09, state[4 * c + 1]) ^ nhan_gf(0x0E, state[4 * c + 2]) ^ nhan_gf(0x0B, state[4 * c + 3]);
        temp[4 * c + 3] = nhan_gf(0x0B, state[4 * c + 0]) ^ nhan_gf(0x0D, state[4 * c + 1]) ^ nhan_gf(0x09, state[4 * c + 2]) ^ nhan_gf(0x0E, state[4 * c + 3]);
    }
    for (int i = 0; i < So_byte_khoi; i++) state[i] = temp[i];
}

// Addroundkey
void add_round_key(uint8_t state[16], const uint8_t round_key[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= round_key[i];          // XOR state với khoa con tung vong
    }
}