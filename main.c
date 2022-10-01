#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "scan.c"
#include "array.c"

#define BOARD_SIZE 9
#define MULIGE_INTS 10

void menu();

int *** makeBoardFromFile(char* file);

int *** uiTable();

void printFolder(char * path);

int checkIfInFolder(char * pFileName, char * path);

void createSudokuFile(int*** pBoard);

void solver(int*** pBoard);

void guesser(int *** pBoard, int attemptNumb);

void logickSolve(int*** pBoard);

int isCorrect(int *** pBoard);

int remainingZeros(int *** pBoard);

int * possibilities(int*** pBoard, int i, int j);

int * posLine(int *** pBoard, int i, int j, int * posArray);

int * posSquare(int *** pBoard, int i, int j, int * posArray);

int possibilitiesAmount(int *posArray);

int findIndexOf(int numb, int* posArray);

void printTable(int*** pBoard);

void free3DpArray(int ***pBoard);

int main() {
    menu();
    return 0;
}


/*
 * Menu of the entire program interacted with through the console.
 */
void menu() {
    int option, exit = 0;
    do {
        printf("\nMenu: \n1: Input board and solve\n2: Input board to file\n3: Solve board from file\n4: Exit");
        option = scanInt();
        printf("\n");
        int ***board;
        char *path;
        switch (option) {
            case 1:
                board = uiTable();
                logickSolve(board);
                free3DpArray(board);
                break;
            case 2:
                board = uiTable();
                createSudokuFile(board);
                free3DpArray(board);
                break;
            case 3:
                path = "SudokuBoards/";
                printFolder(path);
                //get user input for file which is an actual file in the folder
                char pFileName[100];
                do {
                    printf("\n\nWrite the name of the file with the Sudoku board\nEnter String: ");
                    scanf("%s", &pFileName);
                    strcat(pFileName, ".txt");
                } while (!checkIfInFolder(pFileName, path));
                //make string with the entire file path
                char pFileTxt[100];
                strcat(pFileTxt, "SudokuBoards/");
                strcat(pFileTxt, pFileName);
                //makes 3darray from choosen file
                board = makeBoardFromFile(pFileTxt);
                //empty the files
                strcpy(pFileName, "");
                strcpy(pFileTxt, "");
                //try to solve the board and the free it
                solver(board);
                free3DpArray(board);
                break;
            case 4:
                exit = 1;
                break;
        }
    } while((option > 5 && 0 > option) || exit != 1);
}

/*
 * Reads from file with a sudoku board to make a 4D pointer array with the sudoku boards numbers in the 0 position in
 * fourth layer by going through 2 for loops.
 * */
int *** makeBoardFromFile(char* file) {
    int*** pBoard = create_3d_int_array(BOARD_SIZE, BOARD_SIZE, MULIGE_INTS);  //makes pointer array in the size of a sudoku board
    FILE * pFile;                                                                    //makes a pointer to a file
    printf("\nfile with the name %s:", file);
    pFile = fopen(file, "r");                                           //locates the file for the above pointer and says it's for reading
    signed char c;                                                                   //c represents one char in the file

    if (pFile == NULL) {                                                             //check if there is anything at the address of the pointer (the file)
        printf("There was no file with the name %s", file);                    // if it is not the case it informs the user
        return pBoard;                                                               // and then return to the call
    }
    else {                                                                           //If it's not the case it starts to take what is in the-
        int i = 0, j = 0;                                                            //file and tries to put it in the pointer array of a board
        while ((c = fgetc(pFile)) != EOF) {                                      //gets a char and put it in c from the file as long as the end hasn't been reached
            if (c != ' ' && c != '\n') {                                             //makes sure c is actually a character
                pBoard[i][j][0] = c - '0';                                           //converts char to int, something about ascii and  negating by 0
                i++;
                if (i == 9) {                                                        //next line is reached on the board
                    i = 0;
                    j++;
                    if (j == 9) {                                                    //last line of the board has been reached
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
    int *** pBoard = create_3d_int_array(BOARD_SIZE, BOARD_SIZE, MULIGE_INTS);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int ui;
            do {
                printf("\nsquare: %d, %d ", i, j);
                ui = scanInt();
            } while (ui < 0 || ui > 9);
            pBoard[i][j][0] = ui;
        }
    }
    return pBoard;
}

void printFolder(char * path) {
    struct dirent * pEntry;
    DIR * folder = opendir(path);
    if (folder == NULL) {
        printf("Folder Error");
        return;
    }
    printf("\nFiles in folder:");
    while ((pEntry = readdir(folder)) != NULL) {
        printf("\n%s", pEntry->d_name);
    }
    closedir(folder);
}

int checkIfInFolder(char * pFileName, char * path) {
    int bool = 0;
    struct dirent * pEntry;
    DIR * folder = opendir(path);
    if (folder == NULL) {
        printf("Folder Error");
        return 0;
    }
    while ((pEntry = readdir(folder)) != NULL) {
        if (strcmp(pEntry->d_name, pFileName) == 0) {
            bool = 1;
            printf("%d",bool);
        }
    }
    return bool;
}

/*
 * Takes an 4D pointer array with a sudoku board in it and prints it to a txt file.
 */
void createSudokuFile(int*** pBoard) {
    const int charsSize = 100;
    char pFileName[charsSize];
    printf("Name the file for the Sudoku board \nEnter String:  ");
    scanf("%s", &pFileName);

    char pFileTxt[100];
    strcat(pFileName, ".txt");
    strcat(pFileTxt, "SudokuBoards/");
    strcat(pFileTxt, pFileName);

    FILE * pFile;
    pFile = fopen(pFileTxt, "w+");

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

/*
 * prints the first int of the last pointer array in the shape of a sudoku table
 */
void printTable(int*** pBoard) {
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d ",pBoard[j][i][0]);
            if (j == BOARD_SIZE-1) {
                printf("\n");
            }
        }
    }
}
/*
 * TODO
 * run iterations of logickSolve and check them for mistakes at the end
 */
void solver(int*** pBoard) {
    int attemptNumb = 1;
    logickSolve(pBoard);
    int *** copyPBoard = create_3d_int_array(BOARD_SIZE, BOARD_SIZE, MULIGE_INTS);
    copy_3d_int_array(pBoard, copyPBoard, BOARD_SIZE, BOARD_SIZE, MULIGE_INTS);

    while (!isCorrect(pBoard) || remainingZeros(copyPBoard) > 0 && attemptNumb < MULIGE_INTS) {
        int stuck = (remainingZeros(pBoard) < 5);
        printf("\nstuck: %d  zero remaining: %d", stuck, remainingZeros(pBoard));
        if (stuck == 1) {
            attemptNumb++;
            printf("\nAttempt number %d", attemptNumb);
            pBoard = create_3d_int_array(BOARD_SIZE, BOARD_SIZE, MULIGE_INTS);
            copy_3d_int_array(copyPBoard, pBoard, BOARD_SIZE, BOARD_SIZE, MULIGE_INTS);
        }
        guesser(pBoard, attemptNumb);
        printf("after stuck");
        printTable(pBoard);
    }
}


void guesser(int *** pBoard, int attemptNumb) {
    int stuck = (remainingZeros(pBoard) < 5);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (pBoard[i][j][0] == 0) {
                if (possibilitiesAmount(pBoard[i][j]) <= 2) {
                    int k = 1, attempt = 0;
                    while (k < 11) {
                        if (pBoard[i][j][k] > 0) {
                            attempt++;
                            if (attempt >= attemptNumb) {
                                attemptNumb--;
                                printf("\nPBoard[%d][%d][%d] = %d", i, j, k, pBoard[i][j][k]);
                                pBoard[i][j][0] = pBoard[i][j][k];
                                logickSolve(pBoard);
                                break;
                            }
                        }
                        k++;
                        stuck = (remainingZeros(pBoard) < 5);
                        if (stuck) break;
                    }
                }
            }
            if (stuck) break;
        }
        if (stuck) {
            printf("\n\nstuck");
            printTable(pBoard);
            break;
        }
    }
}

void logickSolve(int*** pBoard) {
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
    } while (lastZeroCount != zeroCounter);
}

int isCorrect(int *** pBoard) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int endValI = ((i + 3) / 3) * 3;
            int endValJ = (j + 3) / 3 * 3;

            for (int k = endValI - 3; k < endValI; ++k) {
                for (int l = endValJ - 3; l < endValJ; ++l) {
                    if (pBoard[i][j][0] == pBoard[k][l][0] && (i != k && j != l) && pBoard[i][j][0] != 0) {
                        return 0;
                    }
                }
            }

            for (int x = 0; x < BOARD_SIZE; ++x) {
                for (int y = 0; y < BOARD_SIZE; ++y) {
                    if (y == j && pBoard[x][y][0] != 0 || x == i && pBoard[x][y][0] != 0) {
                        if (pBoard[i][j][0] == pBoard[x][y][0] && (i != x && j != y) && pBoard[i][j][0] != 0) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int remainingZeros(int *** pBoard) {
    printTable(pBoard);
    int zeros = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (pBoard[i][j][0] < 1) {
                zeros++;
            }
        }
    }
    return zeros;
}

/*
 * makes a array of possible int for the first space in the int array of an square in the sudoku board
 */
int * possibilities(int*** pBoard, int i, int j) {
    int* posArray = malloc((sizeof (int)) * MULIGE_INTS);

    for (int k = 0; k < MULIGE_INTS; ++k) {
        posArray[k] = k;
    }

    posArray = posLine(pBoard, i, j, posArray);
    posArray = posSquare(pBoard, i, j, posArray);

    return posArray;
}

/*
 * If a number is on a line with the chosen square
 * the number will be deleted from the possible ints for the square
 * RETURN the remaining possible numbers in an array
 */
int * posLine(int *** pBoard, int i, int j, int * posArray) {
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
/*
 * If a number is in the chosen squares 3 by 3
 * the number will be deleted from the possible ints for the square
 * RETURN the remaining possible numbers in an array
 */
int * posSquare(int *** pBoard, int i, int j, int * posArray) {
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
    return posArray;
}

/*
 * RETURN amount of numbers between 1 - 9 in an array execpt the first space.
 */
int possibilitiesAmount(int *posArray) {
    int amount = 0;
    for (int k = 1; k < MULIGE_INTS; ++k) {
        if (posArray[k] > 0 && posArray[k] < MULIGE_INTS) {
            amount++;
        }
    }
    return amount;
}

/*
 * RETURN the index of a specific number in an array.
 */
int findIndexOf(int numb, int *posArray) {
    for (int i = 0; i < MULIGE_INTS; ++i) {
        if (posArray[i] == numb) {
            return i;
        }
    }
    return -1;
}
/*
 * runs through entire 3D array and frees it
 */
void free3DpArray(int*** pBoard) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            free(pBoard[i][j]);
        }
        free(pBoard[i]);
    }
}




