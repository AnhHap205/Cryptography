#include <iostream>
using namespace std;

int x_nhan_gx(int g, int m, int n) {
    g = g << 1;
    if ((g >> n) & 1)
        g = g ^ m;
    return g;
}
int tich(int f, int g, int m, int n) {
    if (g == 0)
        return 0;

    int phan_du;
    if (g & 1)
        phan_du = f;
    else
        phan_du = 0;
    int phan_con = tich(f, g >> 1, m, n);

    return x_nhan_gx(phan_con, m, n) ^ phan_du;
}
int doc_bit(int so_bit) {
    if (so_bit == 0)
        return 0;
    char c;
    cin >> c;
    return ((c - '0') << (so_bit - 1)) | doc_bit(so_bit - 1);
}

void in_bit(int v, int so_bit) {
    if (so_bit == 0)
        return;

    in_bit(v >> 1, so_bit - 1);
    cout << (v & 1);
}

int main() {
    int n = 4;            
    int m = 0b10011;        // Đa thuc toi gian m(x) = x^4 + x + 1

    cout << "Nhap f(x) (4 bit, VD: x^3+x+1 -> 1011): ";
    int f = doc_bit(n);
    cout << "Nhap g(x) (4 bit, VD: x^2+x   -> 0110): ";
    int g = doc_bit(n);

    int kq = tich(f, g, m, n);

    cout << "f(x)*g(x) = ";
    in_bit(kq, n);
    cout << " (" << kq << ")\n";

    return 0;
}