#include <stdio.h>
#include <time.h>
#include <iostream>
#include <omp.h>
#include <stdlib.h>
using namespace std;
#define numOfElem 20000000
#define edge 100
int divison(int h, int r, int *a) // function for finding pivot place
{
    int x = a[h]; // pivot element
    int k = h;
    int l = r + 1; // numOfElem+1
    int t;         // temp
    while (1)
    {
        do
            k++;
        while ((a[k] <= x) && (k < r)); // k++ until left elem < pivot and k = numOfElem
        do
            l--;
        while (a[l] > x);
        while (k < l) // left < right
        {
            t = a[k];
            a[k] = a[l]; // switch places
            a[l] = t;
            do
                k++;
            while (a[k] <= x);
            do
                l--;
            while (a[l] > x);
        }
        t = a[h];
        a[h] = a[l];
        a[l] = t;
        return l; // return final pivot pos
    }
}
void seqQuickSort(int h, int r, int *a) // recursion!
{
    if (h < r)
    {
        int q = divison(h, r, a); // q is division index,
        seqQuickSort(h, q - 1, a);
        seqQuickSort(q + 1, r, a); // afterwords he is smalles
    }
}
void quicksort(int h, int r, int *a, int downEdge)
{
    if (h < r)
    {
        if ((r - h) < downEdge)
        {
            seqQuickSort(h, r, a);
        }
        else
        {
            int q = divison(h, r, a);
#pragma omp task firstprivate(a, downEdge, r, q)
            quicksort(h, q - 1, a, downEdge);
#pragma omp task firstprivate(a, downEdge, r, q)
            quicksort(q + 1, r, a, downEdge);
        }
    }
}
void parallelQuickSort(int n, int *a, int downEdge)
{
#pragma omp parallel { #pragma omp single nowait quicksort(0, n - 1, a, downEdge); }
}
int main()
{
    double t1, t2;
    int *a = new int[numOfElem];
    t1 = omp_get_wtime();
    srand((int)time(NULL));
    for (int i = 0; i < numOfElem; i++)
    {
        a[i] = rand();
    }
    t2 = omp_get_wtime();
    printf("Sequence generation: %g s\n", (t2 - t1));
    printf("Array sorting...\n \n ");
    t1 = omp_get_wtime(); // seqQuickSort(0, numOfElem - 1, a)
    omp_set_num_threads(4);
    parallelQuickSort(numOfElem, a, edge);
    t2 = omp_get_wtime();
    printf("Quicksort finished in: %g s\n", (t2 - t1));
    free(a);
    return 0;
}