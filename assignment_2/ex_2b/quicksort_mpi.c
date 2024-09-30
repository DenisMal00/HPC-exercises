#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// ================================================================
//  MACROS and DATATYPES
// ================================================================

#if defined(_OPENMP)

// measure the wall-clock time
#define CPU_TIME (clock_gettime(CLOCK_REALTIME, &ts), (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9)

// measure the CPU thread time
#define CPU_TIME_th (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &myts), (double)myts.tv_sec + (double)myts.tv_nsec * 1e-9)

#else

// measure the CPU process time
#define CPU_TIME (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts), (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9)

#endif

#if defined(DEBUG)
#define VERBOSE
#endif

#if defined(VERBOSE)
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

// Define the size of the data array and which element is the "HOT" one
#if !defined(DATA_SIZE)
#define DATA_SIZE 8
#endif
#define HOT 0

// Data structure to hold an array of doubles
typedef struct {
    double data[DATA_SIZE];
} data_t;

// ================================================================
//  PROTOTYPES
// ================================================================

typedef int (compare_t)(const void*, const void*);
void my_qsort(data_t *, int, int, compare_t);
int verify_sorting(data_t *, int, int);
void merge_sorted_chunks(data_t *global_data, int *recvcounts, int *displs, int size);
extern inline compare_t compare;
extern inline compare_t compare_ge;
extern inline int partitioning(data_t *, int, int, compare_t);

// ================================================================
//  CODE
// ================================================================

int main(int argc, char **argv) {

    int rank, size, N;
    data_t *global_data = NULL;
    int *recvcounts = NULL, *displs = NULL;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    N = atoi(argv[1]);
    
    MPI_Datatype MPI_DATA_T;
    MPI_Type_contiguous(DATA_SIZE, MPI_DOUBLE, &MPI_DATA_T);
    MPI_Type_commit(&MPI_DATA_T);

    if (rank == 0) {
        srand(time(NULL));

        global_data = (data_t *)malloc(N * sizeof(data_t));

        for (int i = 0; i < N; i++) {
            global_data[i].data[HOT] = (double)rand() / RAND_MAX;
        }

        recvcounts = (int *)malloc(size * sizeof(int));
        displs = (int *)malloc(size * sizeof(int));

        int offset = 0;
        for (int i = 0; i < size; i++) {
            recvcounts[i] = N / size + (i < (N % size));
            displs[i] = offset;
            offset += recvcounts[i];
        }
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_size = N / size + (rank < (N % size));
    data_t *local_data = (data_t *)malloc(local_size * sizeof(data_t));

    MPI_Scatterv(global_data, recvcounts, displs, MPI_DATA_T, local_data, local_size, MPI_DATA_T, 0, MPI_COMM_WORLD);

    // Start timing before sorting
    struct timespec ts;
    double tstart = CPU_TIME;

    my_qsort(local_data, 0, local_size, compare_ge);

    MPI_Gatherv(local_data, local_size, MPI_DATA_T, global_data, recvcounts, displs, MPI_DATA_T, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        merge_sorted_chunks(global_data, recvcounts, displs, size);

        if (verify_sorting(global_data, 0, N)) {
            printf("Array ordered \n");
        } else {
            printf("Array not ordered\n");
        }

        double tend = CPU_TIME;
        printf("Sorting completed in %f seconds\n", tend - tstart);

        free(global_data);
        free(recvcounts);
        free(displs);
    }

    free(local_data);
    MPI_Type_free(&MPI_DATA_T);
    MPI_Finalize();
    return 0;
}

#define SWAP(A, B, SIZE) do {int sz = (SIZE); char *a = (A); char *b = (B); do { char _temp = *a; *a++ = *b; *b++ = _temp; } while (--sz);} while (0)

inline int partitioning(data_t *data, int start, int end, compare_t cmp) {
    --end;
    void *pivot = (void*)&data[end];
    int pointbreak = end - 1;
    for (int i = start; i <= pointbreak; i++) {
        if (cmp((void*)&data[i], pivot) > 0) {
            while (pointbreak > i && cmp((void*)&data[pointbreak], pivot) > 0) pointbreak--;
            if (pointbreak > i)
                SWAP((void*)&data[i], (void*)&data[pointbreak--], sizeof(data_t));
        }
    }
    pointbreak += (cmp((void*)&data[pointbreak], pivot) <= 0);
    SWAP((void*)&data[pointbreak], pivot, sizeof(data_t));
    return pointbreak;
}

void my_qsort(data_t *data, int start, int end, compare_t cmp) {
    int size = end - start;
    if (size > 2) {
        int mid = partitioning(data, start, end, cmp);
        my_qsort(data, start, mid, cmp);
        my_qsort(data, mid + 1, end, cmp);
    } else if (size == 2 && cmp((void*)&data[start], (void*)&data[end - 1]) > 0) {
        SWAP((void*)&data[start], (void*)&data[end - 1], sizeof(data_t));
    }
}

void merge_sorted_chunks(data_t *global_data, int *recvcounts, int *displs, int size) {
    int total_size = 0;
    for (int i = 0; i < size; i++) {
        total_size += recvcounts[i];
    }

    data_t *merged_data = (data_t *)malloc(total_size * sizeof(data_t));
    int *indices = (int *)calloc(size, sizeof(int));

    for (int i = 0; i < total_size; i++) {
        int min_idx = -1;
        for (int j = 0; j < size; j++) {
            if (indices[j] < recvcounts[j]) {
                if (min_idx == -1 || global_data[displs[j] + indices[j]].data[HOT] < global_data[displs[min_idx] + indices[min_idx]].data[HOT]) {
                    min_idx = j;
                }
            }
        }
        merged_data[i] = global_data[displs[min_idx] + indices[min_idx]];
        indices[min_idx]++;
    }

    memcpy(global_data, merged_data, total_size * sizeof(data_t));
    free(merged_data);
    free(indices);
}

int verify_sorting(data_t *data, int start, int end) {
    for (int i = start + 1; i < end; i++) {
        if (data[i].data[HOT] < data[i - 1].data[HOT]) return 0;
    }
    return 1;
}

inline int compare(const void *A, const void *B) {
    data_t *a = (data_t*)A;
    data_t *b = (data_t*)B;

    double diff = a->data[HOT] - b->data[HOT];
    return (diff > 0) - (diff < 0);
}

inline int compare_ge(const void *A, const void *B) {
    data_t *a = (data_t*)A;
    data_t *b = (data_t*)B;

    return a->data[HOT] >= b->data[HOT];
}

