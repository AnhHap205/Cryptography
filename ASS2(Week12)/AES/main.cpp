#include <iostream>
#include <string>
#include "AES.h"
using namespace std;

// Doi 1 ky tu hex sang so 0 -> 15
int hex_sang_so(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Dua chuoi 32 ky tu hex thanh 16 byte
bool doc_hex(const string& s, uint8_t out[16]) {
    if (s.size() != 2 * So_byte_khoi) return false;
    for (int i = 0; i < So_byte_khoi; i++) {
        int cao = hex_sang_so(s[2 * i]);
        int thap = hex_sang_so(s[2 * i + 1]);
        if (cao < 0 || thap < 0) return false;
        out[i] = (uint8_t)((cao << 4) | thap);
    }
    return true;
}

void print_binary128(const uint8_t value[16]) {
    for (int i = 0; i < So_byte_khoi; i++) {
        for (int bit = 7; bit >= 0; --bit) {
            cout << ((value[i] >> bit) & 1);
        }
        if (i % 4 == 3 && i != So_byte_khoi - 1) cout << ' '; 
    }
    cout << '\n';
}

void print_hex128(const uint8_t value[16]) {
    const char chu_so[] = "0123456789ABCDEF";
    for (int i = 0; i < So_byte_khoi; i++) {
        cout << chu_so[value[i] >> 4] << chu_so[value[i] & 0xF];
    }
    cout << '\n';
}

int main() {
    uint8_t plaintext[16], key[16], ciphertext[16], giai_ma[16];
    string s_plaintext, s_key;

    // 1. Nhap plaintext va key 
    cout << "Nhap plaintext (hex): ";
    cin >> s_plaintext;
    cout << "Nhap key (hex)      : ";
    cin >> s_key;

    if (!doc_hex(s_plaintext, plaintext) || !doc_hex(s_key, key)) {
        cout << "Loi: plaintext va key gom 32 ky tu hex (0-9, A-F).\n";
        return 1;
    }

    // 2. In plaintext va key
    cout << "\nPlaintext (hex): ";
    print_hex128(plaintext);
    cout << "Plaintext (bin)  : ";
    print_binary128(plaintext);

    cout << "Key (hex)      : ";
    print_hex128(key);
    cout << "Key (bin)      : ";
    print_binary128(key);

    // 3. Ma hoa
    encrypt_aes(plaintext, key, ciphertext);
    cout << "\nCiphertext (hex): ";
    print_hex128(ciphertext);
    cout << "Ciphertext (bin)  : ";
    print_binary128(ciphertext);

    // 4. Giai ma
    decrypt_aes(ciphertext, key, giai_ma);
    cout << "\nGiai ma (hex): ";
    print_hex128(giai_ma);
    cout << "Giai ma (bin)  : ";
    print_binary128(giai_ma);

    return 0;
}