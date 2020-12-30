#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define NUMTHREADS 16

int n;
double *mat, *matOriginal;
pthread_barrier_t mybarrier;

struct arg_struct {
    int rank;
};

/* The algorithm for Cholesky decomposition */
void cholesky(int rank) {
	int i, j, k;

	for (j = 0; j < n; j++) {
        // replace the entries above the diagonal with zeroes
		if (rank == 0) {
			for (i = 0; i < j; i++) {
				mat[i * n + j] = 0;
			}
		}
        // update the diagonal element
		if (j % NUMTHREADS == rank) {
			for (k = 0; k < j; k++) {
                mat[j * n + j] = mat[j * n + j] - mat[j * n + k] * mat[j * n + k];
			}

            mat[j * n + j] = sqrt(mat[j * n + j]);
		}
        // wait for all threads to update the diagonal
        pthread_barrier_wait(&mybarrier);
		// divide the rest of the work and update the elements below the diagonal
		for (i = j + 1; i < n; i++) {
			if (i % NUMTHREADS == rank) {
				for (k = 0; k < j; k++) {
                    mat[i * n + j] = mat[i * n + j] - mat[i * n + k] * mat[j * n + k];
				}

                mat[i * n + k] = mat[i * n + k] / mat[j * n + j];
			}
		}
	}
}
/* Thread function */
void *choleskyThread(void *arguments) {
    struct arg_struct *args = arguments;
    cholesky(args->rank);
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
    int i, j, iret;
    FILE *f = NULL;

    pthread_t thread[NUMTHREADS];
    struct arg_struct args_s[NUMTHREADS];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <in_file>\n", argv[0]);
        exit(1);
    }

    f = fopen(argv[1], "r");

    fscanf(f, "%d", &n);

    printf("%d : ", n);

    mat = (double*) malloc(n * n * sizeof(double));

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            fscanf(f, "%lf", &mat[i * n + j]);
        }
    }
    fclose(f);

    // rank of each thread
    for (i = 0; i < NUMTHREADS; ++i) {
        args_s[i].rank = i;
    }

    clock_t begin = clock();

    pthread_barrier_init(&mybarrier, NULL, NUMTHREADS);

    // create threads
    for (i = 0; i < NUMTHREADS; ++i) {
        iret = pthread_create(&thread[i], NULL, choleskyThread, (void *)&args_s[i]);
        if (iret) {
            fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
            exit(EXIT_FAILURE);
        }
    }
    // join threads
    for (i = 0; i < NUMTHREADS; ++i) {
        pthread_join( thread[i], NULL);
    }

    pthread_barrier_destroy(&mybarrier);

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);


    free(mat);
    free(matOriginal);

    return 0;
}
