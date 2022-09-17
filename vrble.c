int *** p3DArray(int num1, int num2, int num3) {
    int*** pBoard = malloc(sizeof(int**) * num1);
    for (int i = 0; i < num1; i++) {
        pBoard[i] = malloc(sizeof(int*) * num2);
        for (int j = 0; j < num2; j++) {
            pBoard[i][j] = malloc(sizeof(int) * num3);
        }
    }
    return pBoard;
}

