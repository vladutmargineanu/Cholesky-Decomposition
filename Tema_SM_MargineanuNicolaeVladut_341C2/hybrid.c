#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

/* The algorithm for Cholesky decomposition */
void cholesky(double *a, int n, int rank, int nProcesses) {
	int i, j, k;
	double *copy = (double*) malloc(n * sizeof(double));

    for (j = 0; j < n; j++) {
        // replace the entries above the diagonal with zeroes
        if (rank == 0) {
			for (i = 0; i < j; i++) {
				a[i * n + j] = 0;
            }
		}
        // update the diagonal element
        if (j % nProcesses == rank) {
            for (k = 0; k < j; k++) {
                a[j * n + j] = a[j * n + j] - a[j * n + k] * a[j * n + k];
            }

            a[j * n + j] = sqrt(a[j * n + j]);
        }
        // broadcast row with new values to other processes
        #pragma omp parallel for
		for (i = 0; i < n; i++) {
            copy[i] = a[j * n + i];
		}
		MPI_Bcast(copy, n, MPI_DOUBLE, j % nProcesses, MPI_COMM_WORLD);
		// this is necessary in order to use broadcast
        #pragma omp parallel for
        for (i = 0; i < n; i++) {
			a[j * n + i] = copy[i];
		}
        // divide the rest of the work and update the elements below the diagonal
        #pragma omp parallel for private (i, k)
        for (i = j + 1; i < n; i++) {
			if (i % nProcesses == rank) {
				for (k = 0; k < j; k++) {
                    a[i * n + j] = a[i * n + j] - a[i * n + k] * a[j * n + k];
				}

                a[i * n + k] = a[i * n + k] / a[j * n + j];
			}
		}
	}
	free(copy);
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
    int n, i, j, rank, nProcesses;
    double *mat;
	FILE * f = NULL;

	if (argc < 2) {
        fprintf(stderr, "Usage: mpirun --oversubscribe -np <num_procs> %s <in_file>\n", argv[0]);
        exit(1);
    }

    MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

    if(rank == 0) {
		f = fopen(argv[1], "r");

	    fscanf(f, "%d", &n);
	    
        printf("%d : ", n);
    }

	// share n with all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    mat = (double*) malloc(n * n * sizeof(double));

	// only first process reads matrix
    if (rank == 0) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                fscanf(f, "%lf", &mat[i * n + j]);
            }
        }
        fclose(f);
    }

	// share matrix with all processes
    MPI_Bcast(mat, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    clock_t begin = clock();
    cholesky(mat, n, rank, nProcesses);
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%f\n", time_spent);

    free(mat);

    MPI_Finalize();
    return 0;
}
