#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 9
#define MULIGE_INTS 10

int *** make3dArray();

int *** makeBoardFromFile(char* file);

int *** uiTable();

void createSudokuFile(int*** pboard);

int *** solveTable(int*** pboard);

int * possibilities(int*** pboard, int i, int j);

int findIndexOf(int numb, int* posiArray);

void printTable(int*** pboard);

void free3DpArray(int ***pboard);

int main() {
    printf("Write the name of the file with the Sudoku board (max of 20 chars)\n =>");
    char pfileName[20];

    scanf("%s", &pfileName);

    int *** standardBoard = makeBoardFromFile(pfileName);

    /*
    int *** standardBoard = uiTable(standardBoard);
    createSudokuFile(standardBoard);
    */


    printTable(solveTable(standardBoard));
    free3DpArray(standardBoard);
    return 0;
}

int *** make3dArray() {
    int*** pboard = malloc(sizeof(int**) * BOARD_SIZE);
    for (int i = 0; i < BOARD_SIZE; i++) {
        pboard[i] = malloc(sizeof(int*) * BOARD_SIZE);
        for (int j = 0; j < BOARD_SIZE; j++) {
            pboard[i][j] = malloc(sizeof(int) * MULIGE_INTS);
        }
    }
    return pboard;
}

int *** makeBoardFromFile(char* file) {
    int*** pboard = make3dArray();                                   //makes pointer array in the size of a sudoku board
    FILE * fpointer;                                                //makes a pointer to a file
    printf("file with the name %s", file);
    fpointer = fopen(file, "r");                       //locates the file for for above pointer and says its for reading
    char c;                                                         //c represents one char in the file

    if (fpointer == NULL) {                                         //check if there is anything at the address of the pointer (the file)
        printf("There was no file with the name %s", file);   // if it is not the case it informs the user
        return pboard;                                               // and then return to the call
    }
    else {                                                          //If it's not the case it starts to take what is in the-
        int i = 0, j = 0;                                           //file and tries to put it in the pointer array of a board
        while ((c = fgetc(fpointer)) != EOF) {                  //gets a char and put it in c from the file as long as the end hasn't been reached
            if (c != ' ' && c != '\n') {                            //makes sure c is actually a character
                pboard[i][j][0] = c - '0';                           //converts char to int, something about ascii and  negating by 0
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
    fclose(fpointer);
    return pboard;
}
int *** uiTable() {
    int *** pboard = make3dArray();
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int ui;
            printf("%d, %d: ", i, j);
            scanf( "%d", &ui);
            if (ui > 0 && ui < 10) {
                pboard[i][j][0] = ui;
            }
            else {
                pboard[i][j][0] = 0;
            }
        }
    }
    return pboard;
}

void createSudokuFile(int*** pboard) {
    char fileName[20];
    printf("Name the file for the Sudoku board (max 20 chars) \n => ");
    scanf("s%", &fileName);
    FILE * pFile;
    pFile = fopen(fileName, "w");

    char sudokuString[400];

    int i = 0;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            sudokuString[i] = pboard[x][y][0] + '0';
            i++;
            sudokuString[i] = ' ';
            i++;
        }
        sudokuString[i] = '\n';
        i++;
    }
    fclose(pFile);
}

void printTable(int*** pboard) {
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d ",pboard[i][j][0]);
            if (j == BOARD_SIZE-1) {
                printf("\n");
            }
        }
    }
}

int *** solveTable(int*** pboard) {
    printTable(pboard);

    int zeroCounter;
    do {
        printTable(pboard);
        zeroCounter = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (pboard[i][j][0] == 0) {
                    zeroCounter++;

                    pboard[i][j] = possibilities(pboard, i, j);

                    int taeller = 0;
                    int fundetTal = 0;
                    for (int k = 1; k < MULIGE_INTS; ++k) {
                        if (pboard[i][j][k] != 0) {
                            taeller++;
                            fundetTal = pboard[i][j][k];
                        }
                    }
                    if (taeller == 1) {
                        pboard[i][j][0] = fundetTal;
                    }
                }
            }
        }
    } while (zeroCounter != 0);
    return pboard;
}



int * possibilities(int*** pboard, int i, int j) {
    int* posiArray = malloc((sizeof (int)) * MULIGE_INTS);

    for (int k = 0; k < MULIGE_INTS; ++k) {
        posiArray[k] = k;
    }

    int endValI = ((i + 3) / 3) * 3;
    int endValJ = (j + 3) / 3 * 3;

    for (int k = endValI - 3; k < endValI; ++k) {
        for (int l = endValJ - 3; l < endValJ; ++l) {
            if (pboard[k][l][0] != 0) {
                int index = findIndexOf(pboard[k][l][0], posiArray);
                if (index > -1) {
                    posiArray[index] = 0;
                }
            }
        }
    }

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if (y == j && pboard[x][y][0] != 0 || x == i && pboard[x][y][0] != 0) {
                int index = findIndexOf(pboard[x][y][0], posiArray);
                if (index > -1) {
                    posiArray[index] = 0;
                }
            }
        }
    }
    return posiArray;
}

int findIndexOf(int numb, int *posiArray) {
    for (int i = 0; i < MULIGE_INTS; ++i) {
        if (posiArray[i] == numb) {
            return i;
        }
    }
    return -1;
}

void free3DpArray(int*** pboard) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            free(pboard[i][j]);
        }
        free(pboard[i]);
    }
}




