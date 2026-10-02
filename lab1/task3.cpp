#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <windows.h>

void method1(int n) {
    omp_set_num_threads(n);
    std::cout << "Метод 1 (барьер):" << std::endl;
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        for (int i = n - 1; i >= 0; i--) {
            if (id == i) std::cout << "Поток " << id << std::endl;
#pragma omp barrier
        }
    }
}

void method2(int n) {
    omp_set_num_threads(n);
    std::cout << "Метод 2 (flush + critical):" << std::endl;

    volatile int turn = n - 1;

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        while (true) {
#pragma omp flush(turn)
            if (turn == id) {
#pragma omp critical
                {
                    std::cout << "Поток " << id << std::endl;
                    turn = turn - 1;
                }
                break;
            }
        }
    }
}

void method3(int n) {
    omp_set_num_threads(n);
    std::cout << "Метод 3 (critical):" << std::endl;
    int next = n - 1;
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        bool done = false;
        while (!done) {
#pragma omp critical
            {
                if (next == id) {
                    std::cout << "Поток " << id << std::endl;
                    next--;
                    done = true;
                }
            }
        }
    }
}

void method4(int n) {
    omp_set_num_threads(n);
    std::cout << "Метод 4 (lock):" << std::endl;
    omp_lock_t lock;
    omp_init_lock(&lock);
    int turn = n - 1;
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        bool done = false;
        while (!done) {
            omp_set_lock(&lock);
            if (turn == id) {
                std::cout << "Поток " << id << std::endl;
                turn--;
                done = true;
            }
            omp_unset_lock(&lock);
        }
    }
    omp_destroy_lock(&lock);
}

void method5(int n) {
    omp_set_num_threads(n);
    std::cout << "Метод 5 (ordered):" << std::endl;
#pragma omp parallel
    {
#pragma omp for ordered
        for (int i = 0; i < n; i++) {
#pragma omp ordered
            std::cout << "Поток " << (n - 1 - omp_get_thread_num()) << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);

    int n = 8;
    int method = 1;
    if (argc > 1) n = std::atoi(argv[1]);
    if (argc > 2) method = std::atoi(argv[2]);

    switch (method) {
    case 1: method1(n); break;
    case 2: method2(n); break;
    case 3: method3(n); break;
    case 4: method4(n); break;
    case 5: method5(n); break;
    default: std::cout << "Неизвестный метод" << std::endl;
    }

    return 0;
}