#include <iostream>
#include <string>
#include <array>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

void printMatrix(int arr[10][10], int col, int row, string message) {
    cout << endl << message << endl;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << arr[i][j] << ' ';
            }
            cout << endl;
        }
}

void generateMatrix(int arr[10][10], int col, int row) {   
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            arr[i][j] = (rand() % 10);
        }
    }
}

int main() {
    int fd[2];
    int arr_a[10][10], arr_b[10][10];
    int col1, col2, row1, row2;
    
    pid_t p;

    if (pipe(fd) == -1) {
        cout << stderr << "Pipe failed" << endl;
        return 1;
    }

    cout << "Введите размер матрицы A:" << endl;
    cin >> row1 >> col1;
    cout << "Введите размер матрицы B:" << endl;
    cin >> row2 >> col2;

    if (row1 != col2) {
        cout << "Количество столбцов матрицы A не равно количеству строк матрицы B!" << endl;
        return 1;
    } 

    srand(time(NULL));
    generateMatrix(arr_a, col1, row1);
    printMatrix(arr_a, col1, row1, "Матрица A:");
    generateMatrix(arr_b, col2, row2);
    printMatrix(arr_b, col2, row2, "Матрица B:");

    p = fork();

    if (p < 0) {
        cout << stderr << "Fork failed" << endl;
        return 1;
    } else if (p > 0) {
        int arr_c[10][10];
        close(fd[1]);
        wait(NULL);
        read(fd[0], arr_c, sizeof(arr_c));
        printMatrix(arr_c, col2, row1, "Результат умножения A x B:");    
        close(fd[0]);
    } else {
        int arr_c[10][10];
        close(fd[0]);
        for (int i = 0; i < row1; i++) {
            for (int j = 0; j < col2; j++) {
                arr_c[i][j] = 0;
                for (int k = 0; k < row2; k++) {
                    arr_c[i][j] += arr_a[i][k] * arr_b[k][j];
                }
            }
        }
        write(fd[1], arr_c, sizeof(arr_c));
        close(fd[1]);
        exit(0);
    }
    return 0;
}