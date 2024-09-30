#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_ITER 255

// Function to calculate the number of iterations for a point
int mandelbrot(double real, double imag, int max_iter) {
    double z_real = 0.0, z_imag = 0.0;
    int iter = 0;
    while (z_real * z_real + z_imag * z_imag <= 4.0 && iter < max_iter) {
        double temp_real = z_real * z_real - z_imag * z_imag + real;
        z_imag = 2.0 * z_real * z_imag + imag;
        z_real = temp_real;
        iter++;
    }
    return iter;
}

int main(int argc, char* argv[]) {
    if (argc != 8) {
        printf("Usage: %s <n_x> <n_y> <x_L> <y_L> <x_R> <y_R> <I_max>\n", argv[0]);
        return 1;
    }

    int n_x = atoi(argv[1]);
    int n_y = atoi(argv[2]);
    double x_L = atof(argv[3]);
    double y_L = atof(argv[4]);
    double x_R = atof(argv[5]);
    double y_R = atof(argv[6]);
    int I_max = atoi(argv[7]);

    // Allocate memory for the result matrix
    int **M = (int **) malloc(n_y * sizeof(int *));
    for (int i = 0; i < n_y; i++) {
        M[i] = (int *) malloc(n_x * sizeof(int));
    }

    // Measure the time for the computation only
    double start_time = omp_get_wtime();

    // Parallelize the loop with OpenMP
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < n_y; i++) {
        for (int j = 0; j < n_x; j++) {
            double real = x_L + j * (x_R - x_L) / (n_x - 1);
            double imag = y_L + i * (y_R - y_L) / (n_y - 1);
            M[i][j] = mandelbrot(real, imag, I_max);
        }
    }

    double end_time = omp_get_wtime();
    printf("Computational time: %f seconds\n", end_time - start_time);

    // Write the image as a PGM file (outside of the timing measurement)
    FILE *pgmfile = fopen("mandelbrot.pgm", "w");
    fprintf(pgmfile, "P5\n%d %d\n%d\n", n_x, n_y, MAX_ITER);
    for (int i = 0; i < n_y; i++) {
        for (int j = 0; j < n_x; j++) {
            fputc((char) M[i][j], pgmfile);
        }
    }
    fclose(pgmfile);

    // Free the allocated memory
    for (int i = 0; i < n_y; i++) {
        free(M[i]);
    }
    free(M);

    return 0;
}

