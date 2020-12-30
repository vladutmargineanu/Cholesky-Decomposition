#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

/* The algorithm for Cholesky decomposition */
void cholesky(double *mat, int n) {
    int i, j, k;
    if (mat == NULL)
        exit(EXIT_FAILURE);
    // main Cholesky algorithm
    for (k = 0; k < n; k++) {
        mat[k * n + k] = sqrt(mat[k * n + k]);

        #pragma omp parallel for
        for (i = k + 1; i < n; i++) {
            mat[i * n + k] = mat[i * n + k] / mat[k * n + k];
        }

        #pragma omp parallel for private (j, i)
        for (j = k + 1; j < n; j++) {
            for(i = 0; i < n; i++) {
                if (i >= j) {
                    mat[i * n + j] = mat[i * n + j] - mat[i * n + k] * mat[j * n + k];
                }
            }
        }
    }
    // zero all elements above the diagonal
    #pragma omp parallel for collapse(2)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (j > i) {
                mat[i * n + j] = 0;
            }
        }
    }
}
/* Function to print a matrix */
void show_matrix(double *mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%2.5f ", mat[i * n + j]);
        printf("\n");
    }
}

 /* Driver program to test above functions */
int main(int argc, char *argv[]) {
    int n, i, j, tid;
    FILE * f = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <in_file>\n", argv[0]);
        exit(1);
    }

    f = fopen(argv[1], "r");

    fscanf(f, "%d", &n);

    printf("%d : ", n);

    omp_set_num_threads(16);

    double *mat = (double*) malloc(n * n * sizeof(double));

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            fscanf(f, "%lf", &mat[i * n + j]);
        }
    }

    clock_t begin = clock();
    cholesky(mat, n);
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%f\n", time_spent);

    free(mat);
    fclose(f);

    return 0;
}
