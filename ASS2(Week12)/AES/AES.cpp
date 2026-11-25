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
        state[i] ^= round_key[i];            // XOR state với khoa con tung vong
    }
}

// Mo rong khoa
void mo_rong_khoa(const uint8_t key[16], uint8_t khoa_mo_rong[So_byte_khoa]) {
    for (int i = 0; i < 16; i++) {
        khoa_mo_rong[i] = key[i];           // 4 tu khoa dau = khoa doc
    }   
    for (int i = 4; i < 4 * (So_vong + 1); i++) {
        uint8_t temp[4];
        for (int j = 0; j < 4; j++) {
            temp[j] = khoa_mo_rong[4 * (i - 1) + j];
        }
        if (i % 4 == 0) {
            uint8_t t = temp[0];            // Dich trai 1 byte
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;

            for (int j = 0; j < 4; j++) {
                temp[j] = Sbox[temp[j]];    // Thay gtri byte bang gtri Sbox
            }
           
            temp[0] ^= RCON[i];     // XOR hang so vong 
        }
        for (int j = 0; j < 4; j++) {
            khoa_mo_rong[4 * i + j] = khoa_mo_rong[4 * (i - 4) + j] ^ temp[j];
        }
    }
}

void encrypt_aes(const uint8_t plaintext[16], const uint8_t key[16], uint8_t ciphertext[16]) {
    uint8_t state[16];
    uint8_t round_keys[So_byte_khoa];
    mo_rong_khoa(key, round_keys);                          // Mo rong khoa goc

    for (int i = 0; i < 16; i++) {
        state[i] = plaintext[i];
    }

    add_round_key(state, round_keys);

    for (int round = 1; round <= So_vong; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, round_keys + round * 16);      // Thuc hien cac vong AES
    }

    for (int i = 0; i < 16; i++) {
        ciphertext[i] = state[i];
    }
}

void decrypt_aes(const uint8_t ciphertext[16], const uint8_t key[16], uint8_t plaintext[16]) {
    uint8_t state[16];
    uint8_t round_keys[So_byte_khoa];
    mo_rong_khoa(key, round_keys);                          // Mo rong khoa goc

    for (int i = 0; i < 16; i++) {
        state[i] = ciphertext[i];
    }

    for (int round = So_vong; round > 0; round--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, round_keys + round * 16);
        inv_mix_columns(state);                              // Thuc hien cac vong AES nguoc
    }

    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, round_keys);                       // Vong 0

    for (int i = 0; i < 16; i++) {
        plaintext[i] = state[i];
    }
}