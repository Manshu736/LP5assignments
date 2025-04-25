#include <iostream>
#include <omp.h>

using namespace std;

// ========== Merge Sort (Sequential) ==========
void merge(int arr[], int low, int mid, int high) {
    int n1 = mid - low + 1, n2 = high - mid;
    int left[n1], right[n2];

    for (int i = 0; i < n1; i++) left[i] = arr[low + i];
    for (int j = 0; j < n2; j++) right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = low;

    while (i < n1 && j < n2)
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];

    while (i < n1) arr[k++] = left[i++];
    while (j < n2) arr[k++] = right[j++];
}

void mergeSort(int arr[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

// ========== Merge Sort (Parallel) ==========
void parallelMergeSort(int arr[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, low, mid);

            #pragma omp section
            parallelMergeSort(arr, mid + 1, high);
        }

        merge(arr, low, mid, high);
    }
}

// ========== Bubble Sort (Sequential) ==========
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
}

// ========== Bubble Sort (Parallel Odd-Even) ==========
void parallelBubbleSort(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        // Odd indexed passes
        #pragma omp for
        for (int j = 1; j < n - 1; j += 2)
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);

        #pragma omp barrier

        // Even indexed passes
        #pragma omp for
        for (int j = 0; j < n - 1; j += 2)
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
    }
}

// ========== Utility Function ==========
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";
}

// ========== Main ==========
int main() {
    const int n = 10;
    int arr[n];
    double start, end;

    cout << "------ MERGE SORT ------\n";

    for (int i = 0, j = n; i < n; i++, j--) arr[i] = j;
    start = omp_get_wtime();
    mergeSort(arr, 0, n - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end - start << " seconds\n";
    printArray(arr, n);

    for (int i = 0, j = n; i < n; i++, j--) arr[i] = j;
    start = omp_get_wtime();
    parallelMergeSort(arr, 0, n - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end - start << " seconds\n";
    printArray(arr, n);

    cout << "\n------ BUBBLE SORT ------\n";

    for (int i = 0, j = n; i < n; i++, j--) arr[i] = j;
    start = omp_get_wtime();
    bubbleSort(arr, n);
    end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << end - start << " seconds\n";
    printArray(arr, n);

    for (int i = 0, j = n; i < n; i++, j--) arr[i] = j;
    start = omp_get_wtime();
    #pragma omp parallel
    parallelBubbleSort(arr, n);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << end - start << " seconds\n";
    printArray(arr, n);

    return 0;
}
