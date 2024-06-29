#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Structure to hold matrix data
typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

// Structure to hold thread arguments
typedef struct {
    Matrix *A;
    Matrix *B;
    Matrix *C;
    int thread_id;
    int num_threads;
} ThreadArgs;

// Function prototypes
Matrix* create_matrix(int rows, int cols);
void free_matrix(Matrix *matrix);
void fill_matrix(Matrix *matrix);
void print_matrix(Matrix *matrix);
void* multiply_matrices(void *args);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <rows_A> <cols_A_rows_B> <cols_B> <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int rows_A = atoi(argv[1]);
    int cols_A_rows_B = atoi(argv[2]);
    int cols_B = atoi(argv[3]);
    int num_threads = atoi(argv[4]);

    if (rows_A <= 0 || cols_A_rows_B <= 0 || cols_B <= 0 || num_threads <= 0) {
        fprintf(stderr, "All dimensions and number of threads must be positive integers.\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    // Create and fill matrices
    Matrix *A = create_matrix(rows_A, cols_A_rows_B);
    Matrix *B = create_matrix(cols_A_rows_B, cols_B);
    Matrix *C = create_matrix(rows_A, cols_B);

    fill_matrix(A);
    fill_matrix(B);

    printf("Matrix A:\n");
    print_matrix(A);
    printf("Matrix B:\n");
    print_matrix(B);

    // Create threads
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadArgs *args = malloc(num_threads * sizeof(ThreadArgs));

    for (int i = 0; i < num_threads; i++) {
        args[i].A = A;
        args[i].B = B;
        args[i].C = C;
        args[i].thread_id = i;
        args[i].num_threads = num_threads;
        pthread_create(&threads[i], NULL, multiply_matrices, &args[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Resultant Matrix C:\n");
    print_matrix(C);

    // Free memory
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    free(threads);
    free(args);

    return EXIT_SUCCESS;
}

Matrix* create_matrix(int rows, int cols) {
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        matrix->data[i] = malloc(cols * sizeof(double));
    }
    return matrix;
}

void free_matrix(Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

void fill_matrix(Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            matrix->data[i][j] = rand() % 10;
        }
    }
}

void print_matrix(Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%5.2f ", matrix->data[i][j]);
        }
        printf("\n");
    }
}

void* multiply_matrices(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    Matrix *A = thread_args->A;
    Matrix *B = thread_args->B;
    Matrix *C = thread_args->C;
    int thread_id = thread_args->thread_id;
    int num_threads = thread_args->num_threads;

    int rows_A = A->rows;
    int cols_A = A->cols;
    int cols_B = B->cols;

    for (int i = thread_id; i < rows_A; i += num_threads) {
        for (int j = 0; j < cols_B; j++) {
            C->data[i][j] = 0.0;
            for (int k = 0; k < cols_A; k++) {
                C->data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }

    pthread_exit(NULL);
}
