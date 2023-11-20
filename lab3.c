#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    int** board = NULL;

    // replace this comment with your code
    fp = fopen(filename, "r");
    
    int** sudoku_board = (int**)malloc(sizeof(int*)*ROW_SIZE);
    
    for(int row = 0; row < ROW_SIZE; row++){
        sudoku_board[row] = (int*)malloc(sizeof(int)*COL_SIZE);
    }

    for (int i = 0; i<ROW_SIZE*COL_SIZE;i++){
        fscanf(fp, "%d," , &sudoku_board[i/9][i%9]);
    }

    return sudoku_board;
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    
    // replace this comment with your code
    
}
