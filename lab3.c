#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

int** read_board_from_file(char* filename){
    FILE *fp = fopen(filename, "r"); 
    int** board = NULL;
// replace this comment with your code
    sudoku_board=(int**)malloc(sizeof(int*)*ROW_SIZE);
    for (int row=0;row<ROW_SIZE;row++) {
        sudoku_board[row]=(int*)malloc(sizeof(int)*COL_SIZE);
    }
    for (int i=0; i<ROW_SIZE*COL_SIZE; i++) {
        fscanf(fp, "%d,", &sudoku_board[i/9][i%9]);
    }
    return sudoku_board;
}

void* validateRow(void* param) {
    param_struct* arg = (param_struct*) param;
    int row = arg->starting_row;
    int rowArr[ROW_SIZE] = {0};
    for (int i = 0; i < ROW_SIZE; i++) {
        int current = sudoku_board[row][i];
        if (current >= 1 && current <= 9) {
            if (rowArr[current - 1] == 1) {
                worker_validation = (int*)0;
                return NULL;
            }
            rowArr[current - 1]++;
        } else {
            worker_validation = (int*)0;
            return NULL;
        }
    }

    for (int i = 0; i < 9; i++) {
        if (rowArr[i] != 1) {
            worker_validation = (int*)0;
            return NULL;
        }
    }

    worker_validation = (int*)1;
    return NULL;
}

void* validateCol(void* param) {
    param_struct* arg = (param_struct*) param;
    int index = arg->starting_col;
    int colArr[ROW_SIZE] = {0};

    for (int i = 0; i < 9; i++) {
        int current = sudoku_board[i][index];
        if (current >= 1 && current <= 9) {
            if (colArr[current - 1] == 1) {
                worker_validation = (int*)0;
                return NULL;
            }
            colArr[current - 1]++;
        } else {
            worker_validation = (int*)0;
            return NULL;
        }
    }
    for (int i = 0; i < 9; i++) {
        if (colArr[i] != 1) {
            worker_validation = (int*)0;
            return NULL;
        }
    }
    worker_validation = (int*)1;
    return NULL;
}

void* validateSquare(void* param) {
    param_struct* arg = (param_struct*) param;
    int maxRow = arg->ending_row;
    int maxCol = arg->ending_col;
    int sqrArr[ROW_SIZE] = {0};
    for (int i = maxCol - 3; i < maxCol; i++) {
        for (int j = maxRow - 3; j < maxRow; j++) {
            int current = sudoku_board[i][j];
            if (current >= 1 && current <= 9) {
                if (sqrArr[current - 1] == 1) {
                    worker_validation = (int*)0;
                    return NULL;
                }
                sqrArr[current - 1]++;
            } else {
                worker_validation = (int*)0;
                return NULL;
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        if (sqrArr[i] != 1) {
            worker_validation = (int*)0;
            return NULL;
        }
    }
    worker_validation = (int*)1;
    return NULL;
}

int is_board_valid() {
    pthread_t tid[27];  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* params = (param_struct*)malloc(sizeof(param_struct) * 27);
    int thread_count = 0;

    for (int i = 0; i < 9; i++) {
        // Validate rows and columns
        params[thread_count].starting_row = i;
        params[thread_count].starting_col = i;

        pthread_create(&tid[thread_count], NULL, validateRow, &(params[thread_count]));
        pthread_join(tid[thread_count], NULL);

        if (worker_validation != (int*)1) {
            return 0;
        }

        pthread_create(&tid[thread_count + 1], NULL, validateCol, &(params[thread_count]));
        pthread_join(tid[thread_count + 1], NULL);

        if (worker_validation != (int*)1) {
            return 0;
        }

        thread_count += 2;
    }

    // Validate squares
    thread_count = 18;
    for (int a = 2; a < 9; a += 3) {
        for (int b = 2; b < 9; b += 3) {
            params[thread_count].starting_row = a - 2;
            params[thread_count].starting_col = b - 2;
            params[thread_count].ending_row = a + 1;
            params[thread_count].ending_col = b + 1;

            pthread_create(&tid[thread_count], NULL, validateSquare, &(params[thread_count]));
            pthread_join(tid[thread_count], NULL);

            if (worker_validation != (int*)1) {
                return 0;
            }

            thread_count++;
        }
    }

    free(params);
    return 1;
}
