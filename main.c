#include "headers.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Pass input as: <executable> <input string> <nlions> <nrats>\n Example ./schedule lrlrlrlr 4 4");
    }
    int nlion = atoi(argv[2]);
    int nrat = atoi(argv[3]);
    if (strlen(argv[1]) != nlion + nrat)
    {
        printf("Error: strlen(<input string>) != <nlions> + <nrats>\n");
        return -1;
    }
    initialize(argv[1], nlion, nrat);

    return 0;
}