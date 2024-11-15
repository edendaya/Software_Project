#ifndef SYM_NMF_H
#define SYM_NMF_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Define constants */
#define EPSILON 0.0001
#define MAX_ITER 300
#define BETA 0.5

typedef struct Node
{
    char data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
    int length;
} LinkedList;

typedef struct
{
    double **array;
    int rows;
    int cols;
} ArrayInfo;

typedef struct
{
    double **array;
    int rows;
    int cols;
} Matrix;

/* Function prototypes */
ArrayInfo read_file_to_array(char *filename);
double **sym(double **X, int rows, int cols);
double **ddg(double **A, int n);
double **norm(double **A, int n);
double **symnmf(double **H, double **W, int n, int k);
void printAndExit();
void printAndExitModule();
double **matrixMultiply(double **A, double **B, int n);
double **computeDHalfInverse(double **D, int n);
double **allocateMatrix(int rows, int cols);


#endif
