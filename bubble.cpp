#include <iostream>
#include <array>
#include <chrono>
#include <cstdlib>
#include <omp.h>
using namespace std;

void sequentialBubbleSort(int *arr, int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = 0; j < size - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(int *arr, int size)
{
#pragma omp parallel for
    for (int i = 0; i < size - 1; ++i)
    {
#pragma omp parallel for
        for (int j = 0; j < size - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

string calculateExecutionTime(void (*sortFn)(int *, int), int *arr, int size)
{
    auto start = chrono::high_resolution_clock::now();
    sortFn(arr, size);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    return to_string(duration.count());
}

int main()
{
    int size;
    cout << "Specify array length: ";
    cin >> size;

    int *arr = new int[size];

    // Creating an array of random numbers for testing purpose
    srand(time(0));
    for (int i = 0; i < size; ++i)
    {
        arr[i] = rand() % size;
    }

    cout << "Generated random array of length " << size << "\n\n";

    // Calling the function and calculating the time for sequential bubble sort
    cout << "Sequential Bubble Sort: " << calculateExecutionTime(sequentialBubbleSort, arr, size) << "ms\n";

    srand(time(0));
    for (int i = 0; i < size; ++i)
    {
        arr[i] = rand() % size;
    }
    // Calling the function and calculating the time for parallel bubble sort
    cout << "Parallel Bubble Sort: " << calculateExecutionTime(parallelBubbleSort, arr, size) << "ms\n";

    delete[] arr;

    return 0;
}
