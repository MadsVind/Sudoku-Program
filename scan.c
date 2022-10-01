int scanFloat() { //kudos "Painless Programming" on YT : youtube/watch?v=wlS0ViH0-Sw
    int i = 0, isFloat = 0, isChar = 0;
    float floatNum = -1;
    char string[100];
    printf("\nEnter Number: ");
    scanf("%s", string);
    for (i = 0; i < strlen(string); ++i) {
        if (!((string[i] >= '0') && (string[i] <= '9'))) {
            if (string[i] == '.') isFloat++;
            else isChar++;
        }
    }
    if (isChar || isFloat > 1) {
        printf("\nError not a number");
    } else {
        floatNum = atof(string);
    }
    return floatNum;
}

int scanInt() { //kudos "Painless Programming" on YT : youtube/watch?v=wlS0ViH0-Sw
    int i = 0, isFloat = 0, isChar = 0, intNum = -1;
    char string[100];
    printf("\nEnter Number: ");
    scanf("%s", string);
    for (i = 0; i < strlen(string); ++i) {
        if (!((string[i] >= '0') && (string[i] <= '9'))) {
            if (string[i] == '.') isFloat++;
            else isChar++;
        }
    }
    if (isChar || isFloat) {
        printf("\nError not an int");
    } else {
        intNum = atoi(string);
    }
    return intNum;
}