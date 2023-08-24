#include <array>
#include <string>
#include <iostream>
#include <pthread.h>

using namespace std;

#define ERROR_CREATE_THREAD - 11
#define ERROR_JOIN_THREAD -12
#define SUCCESS 0

#define MULTIPLICATION_FAILED 1

#define NUM_THREADS 2

typedef struct matrix_t {
    int rows;
    int cols;
    int** data;
} matrix_t;

typedef struct matrices_t {
    matrix_t* matrixA;
    matrix_t* matrixB;
} matrices_t;

void printMatrix(matrix_t* matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            cout << matrix->data[i][j] << ' ';
        }
        cout << endl;
    }
}

void* generateMatrix(void *property) {
    matrix_t* matrix = (matrix_t*) property;
    matrix->data = (int**)malloc(sizeof(int*) * matrix->rows);
    for (int i = 0; i < matrix->rows; i++) {
        matrix->data[i] = (int*)malloc(sizeof(int) * matrix->cols);
        for (int j = 0; j < matrix->cols; j++) {
            matrix->data[i][j] = rand() % 10;
        }
    }
    return SUCCESS;
}

void* multiplicationMatrices(void *args) {
    matrices_t *matrices = (matrices_t*) args;
    matrix_t* matrixA = (matrix_t*)matrices->matrixA;
    matrix_t* matrixB = (matrix_t*)matrices->matrixB;

    cout << endl << "The result of matrix multiplication A x B:" << endl;
    if (matrixA->rows != matrixB->cols) {
        cout << "The number of columns of the matrix A isn't equal ";
        cout << "to the number of rows of the matrix B!" << endl;
    } else {
        for (int i = 0; i < matrixA->rows; i++) {
            for (int j = 0; j < matrixB->cols; j++) {
                int total = 0;
                for (int k = 0; k < matrixB->rows; k++) {
                    total += matrixA->data[i][k] * matrixB->data[k][j];
                }
                cout << total << ' ';
            }
            cout << endl;
        }
    }
    cout << endl;
    return SUCCESS;
}

int main() {
    int cols, rows;
    int status, status_addr;
    
    pthread_t thread_m, thread_g[NUM_THREADS];
    matrix_t matrix[NUM_THREADS];

    matrices_t matrices;

    for(int i = 0; i < NUM_THREADS; i++) {
        cout << "Input size of matrix ";
        i == 0 ? cout << "A:" : cout << "B:";
        cout << endl;
        cin >> rows >> cols;
        matrix[i].rows = rows;
        matrix[i].cols = cols;
        i == 0 ? matrices.matrixA = &matrix[i] : matrices.matrixB = &matrix[i];
    }

    srand(time(NULL));
    for (int i = 0; i < NUM_THREADS; i++) {
        status = pthread_create(&thread_g[i], NULL, generateMatrix, (void *) &matrix[i]);
        if (status != SUCCESS) {
            cout << "Main error: can't create thread, status = " << status << endl;
            exit(ERROR_CREATE_THREAD);
        }    
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        status = pthread_join(thread_g[i], (void**) &status_addr);
        if (status != SUCCESS) {
            cout << "Main error: can't join thread, status = " << status << endl;
            exit(ERROR_JOIN_THREAD);
        }

        cout << endl << "Matrix ";
        i == 0 ? cout << "A:" : cout << "B:";
        cout << endl;
        printMatrix(&matrix[i]);

        cout << endl << "Joined with address: " << status_addr << endl;
    }
    
    status = pthread_create(&thread_m, NULL, multiplicationMatrices, (void *) &matrices);
    if (status != SUCCESS) {
        cout << "Main error: can't create thread, status = " << status << endl;
        exit(ERROR_CREATE_THREAD);
    }

    status = pthread_join(thread_m, (void**) &status_addr);
    if (status != SUCCESS) {
        cout << "Main error: can't join thread, status = " << status << endl;
    }
    cout << "Joined with address: " << status_addr << endl;

    return 0;
}