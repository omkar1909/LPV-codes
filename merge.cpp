#include <iostream>
#include <stdlib.h>
#include <array>
#include <functional>
#include <string>
using namespace std;
//For the purpose of open mp
#include<omp.h>
//to calculate CPU execution time
#include <chrono>
using namespace std;
//defining the function
void p_mergesort(int a[], int i, int j);
void s_mergesort(int a[], int i, int j);
void merge(int a[], int i1, int j1, int i2, int j2);
//Sequential merge sort
void s_mergesort(int a[], int i, int j) {
int mid;
if (i < j) {
mid = (i + j) / 2;
s_mergesort(a, i, mid);
s_mergesort(a, mid + 1, j);
merge(a, i, mid, mid + 1, j);
}
}
//code for parallel mergesort
void p_mergesort(int a[], int i, int j) {
int mid;
if (i < j) {
if ((j - i) > 1000) {
mid = (i + j) / 2;
#pragma omp task firstprivate(a, i, mid)
p_mergesort(a, i, mid);
#pragma omp task firstprivate(a, mid, j)
p_mergesort(a, mid + 1, j);
#pragma omp taskwait
merge(a, i, mid, mid + 1, j);
}
}
else {
s_mergesort(a, i, j);
}
}
//code for eunning parallel merge sort on multiple thread
void parallel_mergesort(int a[], int i, int j) {
#pragma omp parallel num_threads(2)
{
#pragma omp single
{
p_mergesort(a, i, j);
}
}
}
//function to merge the sorted array
void merge(int a[], int i1, int j1, int i2, int j2) {
int temp[1000000];
int i, j, k;
i = i1;
j = i2;
k = 0;
while (i <= j1 && j <= j2) {
if (a[i] < a[j]) {
temp[k++] = a[i++];
}
else {
temp[k++] = a[j++];
}
}
while (i <= j1) {
temp[k++] = a[i++];
}
while (j <= j2) {
temp[k++] = a[j++];
}
for (i = i1, j = 0; i <= j2; i++, j++) {
a[i] = temp[j];
}
}
string timeCalulator(function<void()> sort_fn)
{
auto start = chrono::high_resolution_clock::now();
sort_fn();
auto stop = chrono::high_resolution_clock::now();
auto result = stop - start;
auto duration = chrono::duration_cast<chrono::microseconds>(result);
return to_string(duration.count());
}
int main() {
int *a, n, i;
cout << "Specify array length.\n";
cin >> n;
a = new int[n];
//creating array of random number for testing purpose
for (int i = 0; i < n; i++) {
a[i] = rand() % n;
}
cout << "Generated random array of length " << n << "\n\n";
//calling the function and calculating the time for sequential search
cout << "Sequential Merge sort: " << timeCalulator([&]{ s_mergesort(a, 0, n-1); })
<< " microsecond\n";
//calling the function and calculating the time for parallel search
//setting the thread befor executing parallel programming
omp_set_num_threads(2);
cout << "Parallel (2) Merge sort: "<< timeCalulator([&]{ parallel_mergesort(a, 0, n -
1); }) << " microsecond\n";
return 0;
}
