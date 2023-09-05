#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*region TYPEDEF_AREA_OF_CODssE*/
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
/*endregion TYPEDEF_AREA_OF_CODE*/

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

/*region EXTERN_AND_CONST_VARS*/
const int EPSILON = 0.001;
int dimensionOfVector = 0;
int numberOfVectors = 0;
int K = 0;
int iter = 0;
/*endregion EXTERN_AND_CONST_VARS*/

/*region PROTOTYPE_AREA_OF_CODE*/
ArrayInfo **read_file_to_array(char *filename);
double **sym(double **X, int n);
double **ddg(double **A, int n);
double **norm(double **A, int n);

/*endregion PROTOTYPE_AREA_OF_CODE*/

/*region PUT_INPUT_IN_ARRAY*/
short didWeGetFirstLine;
Node *createNode(char data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("An Error Has Occurred");
        exit(1);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

/* Function to initialize the linked list */
void initializeList(LinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

/* Function to insert a node at the end of the list */
void insert(LinkedList *list, char data)
{
    Node *newNode = createNode(data);
    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
    list->length++;
}

/* Function to free the memory allocated for the linked list */
void freeList(LinkedList *list)
{
    Node *current = list->head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

/* Function to print the linked list */
void printList(LinkedList *list)
{
    Node *current = list->head;
    printf("Normal List: ");
    while (current != NULL)
    {
        printf("%c ", current->data);
        current = current->next;
    }
    printf("\n");

    current = list->tail;
}

ArrayInfo read_file_to_array(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("An Error Has Occurred: Could not open file.\n");
        exit(1);
    }

    int wordlength;
    int i, j, k;
    double **array;
    char *word;
    char ch;
    Node *current;
    Node *wordStart;
    LinkedList allChars;
    initializeList(&allChars);
    dimensionOfVector = 1;
    numberOfVectors = 0;
    didWeGetFirstLine = 0;

    while ((ch = fgetc(file)) != EOF) // Changed getchar() to fgetc(file)
    {
        if (ch == ',' && didWeGetFirstLine == 0)
        {
            dimensionOfVector++;
        }
        if (ch == '\n')
        {
            insert(&allChars, ',');
            numberOfVectors++;
            didWeGetFirstLine = 1;
        }
        else
        {
            insert(&allChars, ch);
        }
    }
    fclose(file); // Close the file
    array = malloc(numberOfVectors * sizeof(double *));
    if (array == NULL)
    {
        printf("An Error Has Occurred");
        exit(1);
    }
    current = allChars.head;
    wordStart = current;
    wordlength = 0;

    for (i = 0; i < numberOfVectors; i++)
    {
        array[i] = malloc(dimensionOfVector * sizeof(double));
        if (array[i] == NULL)
        {
            printf("An Error Has Occurred");
            exit(1);
        }
        for (j = 0; j < dimensionOfVector; j++)
        {
            wordlength = 0;
            wordStart = current;
            while (current->data != ',')
            {
                current = current->next;
                wordlength++;
            }
            current = current->next;

            /* wordStart; */

            word = malloc((wordlength + 1) * sizeof(char));
            if (word == NULL)
            {
                for (k = 0; k < i; k++)
                {
                    free(array[k]);
                }
                printf("An Error Has Occurred");
                exit(1);
            }
            for (k = 0; k < wordlength; k++)
            {
                word[k] = wordStart->data;
                wordStart = wordStart->next;
            }
            word[wordlength] = '\0'; /*Set the null terminator*/
            array[i][j] = atof(word);
            free(word);
        }
    }

    freeList(&allChars);
    ArrayInfo result;
    result.array = array;
    result.rows = numberOfVectors;
    result.cols = dimensionOfVector;
    return result;
}
/*endregion PUT_INPUT_IN_ARRAY*/

/*region goals functions*/
// Function to calculate and output the similarity matrix
double **sym(double **X, int n)
{
    double **A = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        A[i] = malloc(n * sizeof(double));
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                double squaredDistance = 0.0;
                for (int d = 0; d < n; d++)
                {
                    double diff = X[i][d] - X[j][d];
                    squaredDistance += diff * diff;
                }
                A[i][j] = exp(-squaredDistance / 2);
            }
            else
            {
                A[i][j] = 0.0;
            }
        }
    }
    return A; // Return the dynamically allocated array
}

// Function to calculate and output the diagonal degree matrix
double **ddg(double **A, int n)
{
    // Allocate a 2D array for D
    double **D = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        D[i] = malloc(n * sizeof(double));
    }

    // Initialize the array with zeros
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            D[i][j] = 0.0;
        }
    }

    // Calculate the diagonal degree values
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            D[i][i] += A[i][j];
        }
    }

    return D; // Return the dynamically allocated 2D array
}

// Allocate memory for a matrix
double **allocateMatrix(int rows, int cols)
{
    double *matrix = (double *)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; ++i)
    {
        matrix[i] = (double *)malloc(cols * sizeof(double));
    }
    return matrix;
}
// Calculate the D^-1/2 matrix
double **computeDHalfInverse(double **D, int n)
{
    double **D_half_inv = allocateMatrix(n, n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            D_half_inv[i][j] = (i == j) ? 1 / sqrt(D[i][j]) : 0;
        }
    }
    return D_half_inv;
}
// Multiply two matrices
double **matrixMultiply(double **A, double **B, int n)
{
    double **result = allocateMatrix(n, n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            result[i][j] = 0;
            for (int k = 0; k < n; ++k)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}
// Function to calculate W = D^-1/2 * A * D^-1/2
double **norm(double **A, double **D, int n)
{
    double **D_half_inv = computeDHalfInverse(D, n);
    double **temp = matrixMultiply(D_half_inv, A, n);
    double **W = matrixMultiply(temp, D_half_inv, n);

    // Free allocated memory for intermediate matrices
    for (int i = 0; i < n; ++i)
    {
        free(D_half_inv[i]);
        free(temp[i]);
    }
    free(D_half_inv);
    free(temp);

    return W;
}
double** symnmf(double **H, double **W, int k, int n){
    
}

/*endregion goals functions*/

/*region MAIN*/
int main(int argc, char *argv[])
{
    /* DECLARATION ON VARIABLES */
    double **datapoints;
    char *mode, *input_file_name;
    int number_datapoints;
    double **outputmatrix;

    if (argc >= 3)
    {
        mode = atoi(argv[1]);
        input_file_name = atoi(argv[2]);
    }
    ArrayInfo datapointsstruct = read_file_to_array(input_file_name);
    datapoints = datapointsstruct.array;
    number_datapoints = datapointsstruct.rows;

    // if mode is equal to sym
    if (strcmp(mode, "sym") == 0)
    {
        outputmatrix = sym(datapoints, number_datapoints);
    }
    else if (strcmp(mode, "ddg") == 0)
    {
        outputmatrix = ddg(datapoints, number_datapoints);
    }
    else if (strcmp(mode, "norm") == 0)
    {
        outputmatrix = norm(datapoints, number_datapoints);
    }
    else
    {
        printf("An Error Has Occurred: Invalid mode.\n");
        exit(1);
    }
    // print outputmatrix
    for (int i = 0; i < number_datapoints; i++)
    {
        for (int j = 0; j < number_datapoints; j++)
        {
            if (j != 0)
                printf(",");
            printf("%.4f", outputmatrix[i][j]);
        }
        printf("\n");
    }
}

/*endregion MAIN*/