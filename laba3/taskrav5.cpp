#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

using namespace std;

struct Point
{
    double coordX, coordY, coordZ;
};

int main()
{
    setlocale(LC_ALL, "Russian");

    vector<Point> pointList;
    Point currentPoint;

    string fileName;

    cout << "Enter input file name: ";
    cin >> fileName;

    // Read points from file
    ifstream inputFile(fileName);

    if (!inputFile.is_open())
    {
        cout << "Error: cannot open file!" << endl;
        return 1;
    }

    while (inputFile >> currentPoint.coordX >> currentPoint.coordY >> currentPoint.coordZ)
    {
        pointList.push_back(currentPoint);
    }

    inputFile.close();

    int pointCount = static_cast<int>(pointList.size());

    if (pointCount == 0)
    {
        cout << "Error: file contains no points!" << endl;
        return 1;
    }

    int methodChoice;

    cout << "\nChoose calculation method:" << endl;
    cout << "1 - Parallel loop" << endl;
    cout << "2 - Functional decomposition" << endl;
    cout << "Your choice: ";
    cin >> methodChoice;

    double totalX = 0.0;
    double totalY = 0.0;
    double totalZ = 0.0;

    double resultX, resultY, resultZ;

    omp_set_dynamic(0);

    // Method 1: Parallel loop
    if (methodChoice == 1)
    {
        omp_set_num_threads(4);

#pragma omp parallel for reduction(+:totalX, totalY, totalZ)
        for (int index = 0; index < pointCount; index++)
        {
            totalX += pointList[index].coordX;
            totalY += pointList[index].coordY;
            totalZ += pointList[index].coordZ;
        }
    }

    // Method 2: Functional decomposition
    else if (methodChoice == 2)
    {
#pragma omp parallel sections num_threads(3)
        {
#pragma omp section
            {
                for (int index = 0; index < pointCount; index++)
                {
                    totalX += pointList[index].coordX;
                }
            }

#pragma omp section
            {
                for (int index = 0; index < pointCount; index++)
                {
                    totalY += pointList[index].coordY;
                }
            }

#pragma omp section
            {
                for (int index = 0; index < pointCount; index++)
                {
                    totalZ += pointList[index].coordZ;
                }
            }
        }
    }

    else
    {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    // Calculate geometric center
    resultX = totalX / pointCount;
    resultY = totalY / pointCount;
    resultZ = totalZ / pointCount;

    // Display results
    cout << "\nNumber of points: " << pointCount << endl;

    cout << "\nGeometric center:" << endl;
    cout << "X = " << resultX << endl;
    cout << "Y = " << resultY << endl;
    cout << "Z = " << resultZ << endl;

    cout << "\nCalculation completed!" << endl;

    return 0;
}