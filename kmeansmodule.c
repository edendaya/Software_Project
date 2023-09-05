#define PY_SSIZE_T_CLEAN
#include <Python.h>
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
double EPSILON = 0.001;
int dimensionOfVector = 0;
int numberOfVectors = 0;
int K = 0;
int iter = 0;
/*endregion EXTERN_AND_CONST_VARS*/

/*region PROTOTYPE_AREA_OF_CODE*/
double **read_file_to_array();
double delta(double *vector1, double *vector2);
int convergence(double **centroids, double **oldcentroids, int oldcentroidslen);
void add_vector_to_centroid(double *centroid, double *vector, int centroidindex, int *counters);
void average(double *centroid, int counter);
int find_match_centroid_index(double **centroids, double *vector);
double **fit(double **centriods, double **vectors, int K, int iter, double epsilon);
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

/*region FUNCTION_AREA_OF_CODE*/
double delta(double *vector1, double *vector2)
{
    double output = 0.0;
    int i;
    for (i = 0; i < dimensionOfVector; i++)
    {
        output += pow(vector1[i] - vector2[i], 2);
    }
    return sqrt(output);
}

int convergence(double **centroids, double **oldcentroids, int firstiter)
{
    int i;
    if (firstiter == 1)
    {
        return 0;
    }
    for (i = 0; i < K; i++)
    {
        if (delta(centroids[i], oldcentroids[i]) >= EPSILON)
        {
            return 0;
        }
    }
    free(oldcentroids);
    free(centroids);
    exit(1);
}

int find_match_centroid_index(double **centroids, double *vector)
{
    double closest_distance = delta(vector, centroids[0]);
    int centroidindex = 0;
    int i;

    for (i = 1; i < K; i++)
    {
        double distance = delta(vector, centroids[i]);

        if (distance < closest_distance)
        {
            closest_distance = distance;
            centroidindex = i;
        }
    }
    return centroidindex;
}

void add_vector_to_centroid(double *centroid, double *vector, int centroidindex, int *counters)
{ /* adds the coordinates of the vector to his centroid */
    int i;
    for (i = 0; i < dimensionOfVector; i++)
    {
        centroid[i] += vector[i];
    }
    counters[centroidindex]++;
}

void average(double *centroid, int counter)
{ /* update centroid after adding it all his vectors by divide it with counter */
    int i;
    for (i = 0; i < dimensionOfVector; i++)
    {
        double tmp = centroid[i];
        if (counter == 0)
        {
            centroid[i] = 0;
            continue;
        }
        else
        {
            centroid[i] = tmp / counter;
        }
    }
}

/*endregion FUNCTION_AREA_OF_CODE*/

/*region PYTHON_WRAPPER*/

static PyObject *py_fit(PyObject *self, PyObject *args)
{
    PyObject *centroidspy;
    PyObject *vectorspy;
    int Kpy;
    int iterpy;
    int epsilonpy;

    /*Parse the input tuple*/
    if (!PyArg_ParseTuple(args, "OOiid", &centroidspy, &vectorspy, &Kpy, &iterpy, &epsilonpy))
    {
        return NULL;
    }

    /*Get the length of the outer list and allocate our arrays*/
    Py_ssize_t outer_length_centroidspy = PyList_Size(centroidspy);
    Py_ssize_t outer_length_vectorspy = PyList_Size(vectorspy);
    double **c_array = malloc(outer_length_centroidspy * sizeof(double *));
    double **v_array = malloc(outer_length_vectorspy * sizeof(double *));

    // Parse the outer list of centroidspy
    for (Py_ssize_t i = 0; i < outer_length_centroidspy; i++)
    {
        PyObject *sublist_obj = PyList_GetItem(centroidspy, i);
        Py_ssize_t inner_length = PyList_Size(sublist_obj);
        c_array[i] = malloc(inner_length * sizeof(double));

        // Parse the inner list of centroidspy
        for (Py_ssize_t j = 0; j < inner_length; j++)
        {
            PyObject *float_obj = PyList_GetItem(sublist_obj, j);
            // Get the float value
            c_array[i][j] = PyFloat_AsDouble(float_obj);
        }
    }

    // Parse the outer list of vectorspy
    for (Py_ssize_t i = 0; i < outer_length_vectorspy; i++)
    {
        PyObject *sublist_obj = PyList_GetItem(vectorspy, i);
        Py_ssize_t inner_length = PyList_Size(sublist_obj);
        v_array[i] = malloc(inner_length * sizeof(double));

        // Parse the inner list of vectorspy
        for (Py_ssize_t j = 0; j < inner_length; j++)
        {
            PyObject *float_obj = PyList_GetItem(sublist_obj, j);
            // Get the float value
            v_array[i][j] = PyFloat_AsDouble(float_obj);
        }
    }

    // return to the python the output of the double **fit(double **centriods, double **vectors, int K, int iter) using both arrays and the ints
    double **result = fit(c_array, v_array, Kpy, iterpy, epsilonpy);
    PyObject *py_result = PyList_New(Kpy);
    int inner_length = dimensionOfVector;
    for (int row = 0; row < Kpy; row++)
    {
        PyObject *py_row = PyList_New(inner_length);
        for (int col = 0; col < inner_length; col++)
        {
            PyList_SetItem(py_row, col, PyFloat_FromDouble(result[row][col]));
        }
        PyList_SetItem(py_result, row, py_row);
    }
    return py_result;
}

static PyMethodDef fitmethod[] = {
    {"mykmeanssp",                  /* the Python method name that will be used */
     (PyCFunction)py_fit,           /* the C-function that implements the Python function and returns static PyObject*  */
     METH_VARARGS,                  /* flags indicating parametersaccepted for this function */
     PyDoc_STR("kmeans algorith")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}           /* The last entry must be all NULL as shown to act as a
                                       sentinel. Python looks for this entry to know that all
                                       of the functions for the module have been defined. */
};
static struct PyModuleDef fitmodule = {
    PyModuleDef_HEAD_INIT,
    "fit",    /* name of module */
    NULL,     /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    fitmethod /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_fit(void)
{
    PyObject *m;
    m = PyModule_Create(&fitmodule);
    if (!m)
    {
        return NULL;
    }
    return m;
}

/*endregion PYTHON_WRAPPER*/

/*region MAIN*/
double **fit(double **centriods, double **vectors, int K, int iter, double epsilon)
{
    /* DECLARATION ON VARIABLES */
    int *counters;
    double **oldcentriods;
    int firstiter;
    int centroidindex;
    int counter;
    int i, j;
    EPSILON = epsilon;

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
        exit(1);
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
            exit(1);
        }
    }
    for (i = 0; i < K; i++)
    {
        for (j = 0; j < dimensionOfVector; j++)
        {
            oldcentriods[i][j] = 0;
        }
    }

    if (centriods == NULL)
    {
        printf("An Error Has Occurred");
        for (i = 0; i < numberOfVectors; i++)
        {
            free(vectors[i]);
        }
        free(counters);
        free(oldcentriods);
        exit(1);
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
            exit(1);
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

    free(vectors);
    free(oldcentriods);
    return centriods;
}
/*endregion ALGORITHEM*/
/*endregion MAIN*/
