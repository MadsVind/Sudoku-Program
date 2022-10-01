void copy_int_array(int * array, int * new_array, int length) {
    for (int i = 0; i < length; ++i) {
        new_array[i] = array[i];
    }
}

void copy_double_array(double * array, double * new_array, int length) {
    for (int i = 0; i < length; ++i) {
        new_array[i] = array[i];
    }
}

void copy_char_array(char * array, char * new_array, int length) {
    for (int i = 0; i < length; ++i) {
        new_array[i] = array[i];
    }
}

int *** create_3d_int_array(int num1, int num2, int num3) {
    int*** pBoard = malloc(sizeof(int**) * num1);
    for (int i = 0; i < num1; i++) {
        pBoard[i] = malloc(sizeof(int*) * num2);
        for (int j = 0; j < num2; j++) {
            pBoard[i][j] = malloc(sizeof(int) * num3);
        }
    }
    return pBoard;
}

void copy_3d_int_array(int *** org_array, int *** copy_array, int num1, int num2, int num3) {
    for (int i = 0; i < num1; ++i) {
        for (int j = 0; j < num2; ++j) {
            for (int k = 0; k < num3; ++k) {
                copy_array[i][j][k] = org_array[i][j][k];
            }
        }
    }
}

void print_int_array(int * array, int length) {
    for (int i = 0; i < length; ++i) {
        printf("\nelement %d = %d", i, array[i]);
    }
}
