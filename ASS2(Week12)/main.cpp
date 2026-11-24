#include <iostream>
#include "DES.h"
using namespace std;

void print_binary64(uint64_t value) {
    for (int i = 63; i >= 0; --i) {
        cout << ((value >> i) & 1);
    }
    cout << '\n';
}

void print_hex64(uint64_t value) {
    const char chu_so[] = "0123456789ABCDEF";
    for (int i = 15; i >= 0; --i) {
        int nhom_4_bit = (value >> (4 * i)) & 0xF;
        cout << chu_so[nhom_4_bit];
    }
    cout << '\n';
}

int main() {
    uint64_t plaintext, key;
    cout << "Nhap plaintext (hex): ";
    cin >> hex >> plaintext;
    cout << "Nhap key (hex)      : ";
    cin >> hex >> key;

    // 2. In plaintext va key
    cout << "\nPlaintext (hex): ";
    print_hex64(plaintext);
    cout << "Plaintext (bin): ";
    print_binary64(plaintext);

    cout << "Key (hex)      : ";
    print_hex64(key);
    cout << "Key (bin)      : ";
    print_binary64(key);

    // 3. Ma hoa
    uint64_t ciphertext = encrypt(plaintext, key);
    cout << "\nCiphertext (hex): ";
    print_hex64(ciphertext);
    cout << "Ciphertext (bin): ";
    print_binary64(ciphertext);

    // 4. Giai ma
    uint64_t giai_ma = decrypt(ciphertext, key);
    cout << "\nGiai ma (hex): ";
    print_hex64(giai_ma);
    cout << "Giai ma (bin): ";
    print_binary64(giai_ma);

    return 0;
}
