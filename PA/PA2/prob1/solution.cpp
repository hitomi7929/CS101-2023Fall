#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <numbers>
#include <algorithm>


std::vector<std::complex<double>> FFT(std::vector<std::complex<double>>& a, long l, int fft) {
    if (l == 1) {
        return {a[0]};
    }
    std::vector<std::complex<double>> a0;
    std::vector<std::complex<double>> a1;

    for (long i = 0; i < l; i += 2) {
        a0.push_back(a[i]);
        a1.push_back(a[i+1]);
    }

    std::vector<std::complex<double>> y0 = FFT(a0, a0.size(), fft);
    std::vector<std::complex<double>> y1 = FFT(a1, a1.size(), fft);
    std::vector<std::complex<double>> y(l);
    std::complex<double> omega = std::polar(1.0, 0.0);
    std::complex<double> omega_n = std::polar(1.0, fft*2*std::numbers::pi/l);

    long k = l/2;
    for (long i = 0; i < k; i++) {
        y[i] = y0[i] + omega * y1[i];
        y[i+k] = y0[i] - omega * y1[i];
        omega *= omega_n;
    }

    return y;
}


int main() {
    long n, m;
    std::cin >> n >> m;
    long L = n+m-1;
    long l = 1 << ((long)(log(L) / log(2)) + 1);
    std::vector<std::complex<double>> a(l);
    std::vector<std::complex<double>> b(l);

    double real;
    for (long i = 0; i < n; i++) {
        std::cin >> real;
        a[i] = std::complex<double>(real, 0);
    }
    for (long j = 0; j < m; j++) {
        std::cin >> real;
        b[j] = std::complex<double>(real, 0);
    }

    std::vector<std::complex<double>> A = FFT(a, l, -1);
    std::vector<std::complex<double>> B = FFT(b, l, -1);

    for (long i = 0; i < l; i++) {
        A[i] *= B[i];
    }


    std::vector<std::complex<double>> C = FFT(A, l, 1);
    for (long i = 0; i < L; i++) {
        std::cout << (long)(C[i].real() / (double)l + 0.5);
        std::cout << " ";
    }

    
    return 0;
}