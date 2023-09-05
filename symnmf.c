#include <stdio.h>
#include <math.h>
#include <stdlib.h>

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

/*region EXTERN_AND_CONST_VARS*/
const int EPSILON = 0.001;
int dimensionOfVector = 0;
int numberOfVectors = 0;
int K = 0;
int iter = 0;
/*endregion EXTERN_AND_CONST_VARS*/

/*region PROTOTYPE_AREA_OF_CODE*/
double **read_file_to_array(char *filename);
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

double **read_file_to_array(char *filename)
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
    return array;
}
/*endregion PUT_INPUT_IN_ARRAY*/

/*region MAIN*/
int main(int argc, char *argv[])
{
    /* DECLARATION ON VARIABLES */
    int *counters;
    double **centriods, **oldcentriods, **vectors;
    int firstiter;
    int centroidindex;
    int counter;
    int i, j;
    char *mode, *input_file_name;

    if (argc >= 3)
    {
        mode = atoi(argv[1]);
        input_file_name = atoi(argv[2]);
    }

    vectors = read_file_to_array(input_file_name);
    /*CHECKING FOR INPUT ERRORS*/
    if (K > 1 && K < numberOfVectors)
    {
        /*pass*/
    }
    else
    {
        printf("Invalid number of clusters!");
        for (i = 0; i < numberOfVectors; i++)
        {
            free(vectors[i]);
        }
        free(vectors);
        return 1;
    }
    if (iter > 1 && iter < 1000)
    {
        /*pass*/
    }
    else
    {
        printf("Invalid maximum iteration!");
        for (i = 0; i < numberOfVectors; i++)
        {
            free(vectors[i]);
        }
        free(vectors);
        return 1;
    }
    counters = (int *)malloc(K * sizeof(int)); /*array to count number of vectors of each centroid to calculate */
    if (counters == NULL)
    {
        printf("An Error Has Occurred");
        for (i = 0; i < numberOfVectors; i++)
        {
            free(vectors[i]);
        }
        free(vectors);
        free(counters);
        return 1;
    }
    oldcentriods = (double **)malloc(K * sizeof(double *));
    if (oldcentriods == NULL)
    {
        printf("An Error Has Occurred");
        for (i = 0; i < numberOfVectors; i++)
        {
            free(vectors[i]);
        }
        free(counters);
        free(oldcentriods);
        return 1;
    }
    for (i = 0; i < K; i++)
    {
        oldcentriods[i] = (double *)malloc(dimensionOfVector * sizeof(double));
        if (oldcentriods[i] == NULL)
        {
            printf("An Error Has Occurred");
            for (i = 0; i < numberOfVectors; i++)
            {
                free(vectors[i]);
            }
            free(counters);
            return 1;
        }
    }
    for (i = 0; i < K; i++)
    {
        for (j = 0; j < dimensionOfVector; j++)
        {
            oldcentriods[i][j] = 0;
        }
    }

    centriods = (double **)malloc(K * sizeof(double *));
    if (centriods == NULL)
    {
        printf("An Error Has Occurred");
        for (i = 0; i < numberOfVectors; i++)
        {
            free(vectors[i]);
        }
        free(counters);
        free(oldcentriods);
        return 1;
    }
    for (i = 0; i < K; i++)
    {
        centriods[i] = (double *)malloc(dimensionOfVector * sizeof(double));
        if (centriods[i] == NULL)
        {
            printf("An Error Has Occurred");
            for (i = 0; i < numberOfVectors; i++)
            {
                free(vectors[i]);
            }
            free(counters);
            free(oldcentriods);
            return 1;
        }
    }

    /*initialize centroids*/
    for (i = 0; i < K; i++)
    {
        for (j = 0; j < dimensionOfVector; j++)
        {
            centriods[i][j] = vectors[i][j];
        }
    }

    /*region ALGORITHEM*/
    firstiter = 1;
    centroidindex = -1;
    counter = 0;
    while (!convergence(centriods, oldcentriods, firstiter) && counter < iter)
    {
        for (i = 0; i < K; i++)
        {
            counters[i] = 0;
        }
        for (i = 0; i < K; i++)
        {

            for (j = 0; j < dimensionOfVector; j++)
            {
                oldcentriods[i][j] = centriods[i][j];
                centriods[i][j] = 0;
            }
        }
        for (i = 0; i < numberOfVectors; i++)
        {
            centroidindex = find_match_centroid_index(oldcentriods, vectors[i]);
            add_vector_to_centroid(centriods[centroidindex], vectors[i], centroidindex, counters);
        }

        for (i = 0; i < K; i++)
        {
            average(centriods[i], counters[i]);
        }

        firstiter = 0;
        counter++;
    }

    for (i = 0; i < K; i++)
    {
        for (j = 0; j < dimensionOfVector; j++)
        {
            printf("%.4f", centriods[i][j]);
            if (j < dimensionOfVector - 1)
                printf(",");
        }
        printf("\n");
    }
    printf("\n");

    /*free memory*/
    for (i = 0; i < numberOfVectors; i++)
    {
        free(vectors[i]);
    }

    free(counters);

    for (i = 0; i < K; i++)
    {
        free(oldcentriods[i]);
    }

    for (i = 0; i < K; i++)
    {
        free(centriods[i]);
    }
    free(vectors);
    free(centriods);
    free(oldcentriods);
    return 0;
}
/*endregion ALGORITHEM*/
/*endregion MAIN*/