#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 9
#define MULIGE_INTS 10

int *** make3dArray();

int *** makeBoardFromFile(char* file);

int *** uiTable();

void createSudokuFile(int*** pBoard);

int *** solveTable(int*** pBoard);

int * possibilities(int*** pBoard, int i, int j);

int findIndexOf(int numb, int* posArray);

void printTable(int*** pBoard);

void free3DpArray(int ***pBoard);

int main() {
    /*printf("Write the name of the file with the Sudoku board (max of 20 chars)\n =>");
    char pFileName[20];

    scanf("%s", &pfileName);

    int *** standardBoard = makeBoardFromFile(pFileName);
    */

    int *** standardBoard = uiTable();
    createSudokuFile(standardBoard);



    printTable(solveTable(standardBoard));
    free3DpArray(standardBoard);
    return 0;
}

int *** make3dArray() {
    int*** pBoard = malloc(sizeof(int**) * BOARD_SIZE);
    for (int i = 0; i < BOARD_SIZE; i++) {
        pBoard[i] = malloc(sizeof(int*) * BOARD_SIZE);
        for (int j = 0; j < BOARD_SIZE; j++) {
            pBoard[i][j] = malloc(sizeof(int) * MULIGE_INTS);
        }
    }
    return pBoard;
}
/*
 * Reads from file with a sudoku board to make a 4D pointer array with the sudoku boards numbers in the 0 position in
 * fourth layer by going through 2 for loops.
 * */
int *** makeBoardFromFile(char* file) {
    int*** pBoard = make3dArray();                                  //makes pointer array in the size of a sudoku board
    FILE * pFile;                                                   //makes a pointer to a file
    printf("file with the name %s", file);
    pFile = fopen(file, "r");                          //locates the file for the above pointer and says it's for reading
    signed char c;                                                  //c represents one char in the file

    if (pFile == NULL) {                                            //check if there is anything at the address of the pointer (the file)
        printf("There was no file with the name %s", file);   // if it is not the case it informs the user
        return pBoard;                                              // and then return to the call
    }
    else {                                                          //If it's not the case it starts to take what is in the-
        int i = 0, j = 0;                                           //file and tries to put it in the pointer array of a board
        while ((c = fgetc(pFile)) != EOF) {                     //gets a char and put it in c from the file as long as the end hasn't been reached
            if (c != ' ' && c != '\n') {                            //makes sure c is actually a character
                pBoard[i][j][0] = c - '0';                          //converts char to int, something about ascii and  negating by 0
                i++;
                if (i == 9) {                                       //next line is reached on the board
                    i = 0;
                    j++;
                    if (j == 9) {                                   //last line of the board has been reached
                        break;
                    }
                }
            }
        }
    }
    fclose(pFile);
    return pBoard;
}

/*
 * Makes sudoku board in 4D pointer array from user input by 2 for loops.
 */
int *** uiTable() {
    int *** pBoard = make3dArray();
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int ui = 0;
            printf("%d, %d: ", i, j);
            scanf( "%d", &ui);
            pBoard[i][j][0] = ui;
            while (ui < 0 || ui >= 10) {
                printf("Error 400, try again: %d, %d: ", i, j);
                scanf( "%d", &ui);
                pBoard[i][j][0] = ui;
            }
        }
    }
    return pBoard;
}

void createSudokuFile(int*** pBoard) {
    char fileName[20];
    printf("Name the file for the Sudoku board (max 20 chars) \n => ");
    scanf("%s", &fileName);
    FILE * pFile;
    pFile = fopen(fileName, "w+");

    char sudokuString[400];

    int i = 0;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            sudokuString[i] = pBoard[x][y][0] + '0';
            i++;
            sudokuString[i] = ' ';
            i++;
        }
        sudokuString[i] = '\n';
        i++;
    }
    fputs(sudokuString, pFile);
    fclose(pFile);
}

void printTable(int*** pBoard) {
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d ",pBoard[i][j][0]);
            if (j == BOARD_SIZE-1) {
                printf("\n");
            }
        }
    }
}

int *** solveTable(int*** pBoard) {
    printTable(pBoard);

    int zeroCounter = 0, lastZeroCount;
    do {
        printTable(pBoard);
        lastZeroCount = zeroCounter;
        zeroCounter = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (pBoard[i][j][0] == 0) {
                    zeroCounter++;

                    pBoard[i][j] = possibilities(pBoard, i, j);

                    int counter = 0;
                    int fundetTal = 0;
                    for (int k = 1; k < MULIGE_INTS; ++k) {
                        if (pBoard[i][j][k] != 0) {
                            counter++;
                            fundetTal = pBoard[i][j][k];
                        }
                    }
                    if (counter == 1) {
                        pBoard[i][j][0] = fundetTal;
                    }
                }
            }
        }
    } while ((zeroCounter != 0) && (lastZeroCount != zeroCounter));
    return pBoard;
}



int * possibilities(int*** pBoard, int i, int j) {
    int* posArray = malloc((sizeof (int)) * MULIGE_INTS);

    for (int k = 0; k < MULIGE_INTS; ++k) {
        posArray[k] = k;
    }

    int endValI = ((i + 3) / 3) * 3;
    int endValJ = (j + 3) / 3 * 3;

    for (int k = endValI - 3; k < endValI; ++k) {
        for (int l = endValJ - 3; l < endValJ; ++l) {
            if (pBoard[k][l][0] != 0) {
                int index = findIndexOf(pBoard[k][l][0], posArray);
                if (index > -1) {
                    posArray[index] = 0;
                }
            }
        }
    }

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if (y == j && pBoard[x][y][0] != 0 || x == i && pBoard[x][y][0] != 0) {
                int index = findIndexOf(pBoard[x][y][0], posArray);
                if (index > -1) {
                    posArray[index] = 0;
                }
            }
        }
    }
    return posArray;
}

int findIndexOf(int numb, int *posArray) {
    for (int i = 0; i < MULIGE_INTS; ++i) {
        if (posArray[i] == numb) {
            return i;
        }
    }
    return -1;
}

void free3DpArray(int*** pBoard) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            free(pBoard[i][j]);
        }
        free(pBoard[i]);
    }
}




