#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <string>
#include <omp.h>

using namespace std;
using namespace chrono;

using Matrix = vector<double>;

// Sequential matrix multiplication
void multiplySequential(const Matrix& A, const Matrix& B,
    Matrix& C, int N) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            double a = A[i * N + k];

            for (int j = 0; j < N; j++) {
                C[i * N + j] += a * B[k * N + j];
            }
        }
    }
}

// Parallel matrix multiplication using OpenMP
void multiplyParallel(const Matrix& A, const Matrix& B,
    Matrix& C, int N, int threads) {
    omp_set_num_threads(threads);

#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            double a = A[i * N + k];

            for (int j = 0; j < N; j++) {
                C[i * N + j] += a * B[k * N + j];
            }
        }
    }
}

// Check the difference between matrix results
double maxDifference(const Matrix& A, const Matrix& B) {
    double diff = 0.0;

    for (size_t i = 0; i < A.size(); i++) {
        diff = max(diff, abs(A[i] - B[i]));
    }

    return diff;
}

int main() {
    int N;
    int threadCount;

    cout << "Enter matrix size: ";
    cin >> N;

    if (N <= 0) {
        cerr << "Matrix size must be positive.\n";
        return 1;
    }

    cout << "Enter number of threads: ";
    cin >> threadCount;

    if (threadCount <= 0) {
        cerr << "Number of threads must be positive.\n";
        return 1;
    }

    cout << fixed << setprecision(4);
    cout << "\nMatrix size: " << N << " x " << N << '\n';
    cout << "Number of threads: " << threadCount << '\n';

    Matrix A(N * N), B(N * N);
    Matrix Cseq(N * N, 0.0);
    Matrix Cpar(N * N, 0.0);

    // Initialize matrices
    for (int i = 0; i < N * N; i++) {
        A[i] = (i % 100) * 0.01;
        B[i] = ((i * 7) % 100) * 0.01;
    }

    // Sequential execution
    auto start = steady_clock::now();

    multiplySequential(A, B, Cseq, N);

    auto finish = steady_clock::now();

    double sequentialTime =
        duration<double>(finish - start).count();

    cout << "\nSequential execution:\n";
    cout << "Time: " << sequentialTime << " seconds\n";

    // Parallel execution
    fill(Cpar.begin(), Cpar.end(), 0.0);

    start = steady_clock::now();

    multiplyParallel(A, B, Cpar, N, threadCount);

    finish = steady_clock::now();

    double parallelTime =
        duration<double>(finish - start).count();

    double speedup = sequentialTime / parallelTime;
    double difference = maxDifference(Cseq, Cpar);

    cout << "\nParallel execution:\n";
    cout << "Threads: " << threadCount << '\n';
    cout << "Time: " << parallelTime << " seconds\n";
    cout << "Speedup: " << speedup << "x\n";
    cout << "Validation: "
        << (difference < 1e-8 ? "OK" : "ERROR") << '\n';

    return 0;
}