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

uint32_t P_function(uint32_t input) {
    uint64_t ket_qua = 0;
    for (int i = 0; i < 32; i++) {
        uint64_t bit = (input >> (32 - P[i])) & 1;
        ket_qua = (ket_qua << 1) | bit;
    }
    return ket_qua;
}

uint64_t PC1_function(uint64_t key) {
    uint64_t ket_qua = 0;
    for (int i = 0; i < 56; i++) {
        uint64_t bit = (key >> (64 - PC1[i])) & 1;
        ket_qua = (ket_qua << 1) | bit;
    }
    return ket_qua;
}

uint64_t PC2_function(uint64_t key) {
    uint64_t ket_qua = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t bit = (key >> (64 - PC2[i])) & 1;
        ket_qua = (ket_qua << 1) | bit;
    }
    return ket_qua;
}

uint32_t f_function(uint32_t R, uint64_t K) {
    uint64_t mo_rong = E_function(R) ^ K;           // mo rong 32 bits nua phai R len thanh 48 bits roi XOR với khoa con K
    uint32_t ket_qua = 0;
    for (int i = 0; i < 8; i++) {
        int gom_bit = (mo_rong >> (42 - 6 * i)) & 0b00111111;                    // gom lan luot 6 bits tu 48 bits mo rong thanh mot nhom dua vao Sbox
        int hang = ((gom_bit & 0b00100000) >> 4) | (gom_bit & 0b00000001);       // 2 bits dau va cuoi de xac dinh hang cua Sbox
        int cot = (gom_bit >> 1) & 0b00001111;                                   // 4 bits giua de xac dinh cot cua Sbox
        ket_qua = (ket_qua << 4) | SBOX[i][hang][cot];                           // dau ra 4 bits cua tung Sbox 
    }
    return P_function(ket_qua);
}

uint64_t left_shift(uint64_t input, int shift) {
    return ((input << shift) | (input >> (28 - shift))) & 0x0FFFFFFF;       // dich trai 28 bits va lay 28 bits 
}

void tao_khoa_con(uint64_t key, uint64_t K[16]) {
    uint64_t cd = PC1_function(key);
    uint64_t nua_trai = (cd >> 28) & 0x0FFFFFFF;
    uint64_t nua_phai = cd & 0x0FFFFFFF;
    for (int i = 0; i < 16; i++) {
        nua_trai = left_shift(nua_trai, SHIFTS[i]);
        nua_phai = left_shift(nua_phai, SHIFTS[i]);
        K[i] = PC2_function((nua_trai << 28) | nua_phai);
    }
}

// Ham xu ly ma hoa va giai ma
uint64_t ham_xu_ly(uint64_t input, uint64_t key, bool decrypt) {
    uint64_t K[16];
    tao_khoa_con(key, K);

    uint64_t sau_IP = IP_function(input);
    uint32_t L = (uint32_t)(sau_IP >> 32);
    uint32_t R = (uint32_t)(sau_IP & 0xFFFFFFFF);

    for (int i = 0; i < 16; i++) {
        uint64_t khoa_con = decrypt ? K[15 - i] : K[i];
        uint32_t R_cu = R;
        R = L ^ f_function(R, khoa_con);
        L = R_cu;
    }

    uint64_t truoc_IP_dao = ((uint64_t)R << 32) | L;
    return IP_inverse_function(truoc_IP_dao);
}

uint64_t encrypt(uint64_t plaintext, uint64_t key) {
    return ham_xu_ly(plaintext, key, false);
}

uint64_t decrypt(uint64_t ciphertext, uint64_t key) {
    return ham_xu_ly(ciphertext, key, true);
}