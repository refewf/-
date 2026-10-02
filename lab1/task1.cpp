#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);

    int numThreads = 8;

    if (argc > 1) {
        numThreads = std::atoi(argv[1]);
    }

    omp_set_num_threads(numThreads);

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int total = omp_get_num_threads();

#pragma omp critical
        {
            std::cout << "Hello World от потока " << tid
                << " из " << total << std::endl;
        }
    }

    return 0;
}