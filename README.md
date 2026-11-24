# Cryptography
## Assignment 1 (Week 7): Nhân hai đa thức trong trường hữu hạn GF(2^n) với n = 4 và n = 8
### Mỗi phần tử trong trường GF(2^n) là một chuỗi n bit
### Giải thuật sử dụng: Đệ quy - tính kết quả của biểu thức f(x).g(x) bằng cách tính
    h1(x) = x.g(x) - dịch trái một bit
    h2(x) = x^2.g(x) = x.f1(x) 
    hk(x) = x^k.g(x) 
    Nếu kết quả phép nhân vượt quá n bit (xuất hiện x^n) => XOR với m(x)
    Kết quả f(x).g(x):
    f(x)·g(x) = [g(x) · (f(x)chia x)]·x  +  g · b₀ 
    Sau đó tiến hành gọi đệ quy với f lần lượt dịch phải một bit và dừng khi f = 0 (tích bằng 0).
