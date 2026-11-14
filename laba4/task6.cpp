
#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

using namespace std;

struct Point
{
    double x, y, z;
};

int main()
{
    setlocale(LC_ALL, "Russian");

    vector<Point> points;
    Point p;

    string filename;

    cout << "Enter input file name: ";
    cin >> filename;

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return 1;
    }

    while (file >> p.x >> p.y >> p.z)
    {
        points.push_back(p);
    }

    file.close();

    int N = points.size();

    if (N == 0)
    {
        cout << "File contains no points!" << endl;
        return 1;
    }

    double globalSum = 0.0;

    double sumX = 0.0;
    double sumY = 0.0;
    double sumZ = 0.0;

#pragma omp parallel sections num_threads(3)
    {
#pragma omp section
        {
            for (int i = 0; i < N; i++)
            {
                sumX += points[i].x;
            }

#pragma omp critical
            {
                globalSum += sumX;
            }
        }

#pragma omp section
        {
            for (int i = 0; i < N; i++)
            {
                sumY += points[i].y;
            }

#pragma omp critical
            {
                globalSum += sumY;
            }
        }

#pragma omp section
        {
            for (int i = 0; i < N; i++)
            {
                sumZ += points[i].z;
            }

#pragma omp critical
            {
                globalSum += sumZ;
            }
        }
    }

    double result = globalSum / (3.0 * N);

    cout << "\nNumber of points: " << N << endl;
    cout << "Global sum: " << globalSum << endl;
    cout << "Result: " << result << endl;

    return 0;
}
