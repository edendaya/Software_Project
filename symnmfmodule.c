#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    double **array;
    int rows;
    int cols;
} ArrayInfo;

// Function prototypes for sym, ddg, and norm
double **sym(double **X, int n);
double **ddg(double **A, int n);
double **norm(double **A, int n);

// Python wrapper functions
static PyObject *py_sym(PyObject *self, PyObject *args)
{
    PyObject *py_list;
    int n;

    // Parse Python arguments
    if (!PyArg_ParseTuple(args, "Oi", &py_list, &n))
    {
        return NULL;
    }

    // Convert Python list to C array
    double **X = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_GetItem(py_list, i);
        X[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++)
        {
            PyObject *py_val = PyList_GetItem(py_row, j);
            X[i][j] = PyFloat_AsDouble(py_val);
        }
    }

    // Call C function
    double **result = sym(X, n);

    // Convert C array to Python list
    PyObject *py_result = PyList_New(n);
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_New(n);
        for (int j = 0; j < n; j++)
        {
            PyList_SetItem(py_row, j, PyFloat_FromDouble(result[i][j]));
        }
        PyList_SetItem(py_result, i, py_row);
    }

    // Free allocated C array and result array
    for (int i = 0; i < n; i++)
    {
        free(X[i]);
        free(result[i]);
    }
    free(X);
    free(result);

    return py_result;
}

static PyObject *py_ddg(PyObject *self, PyObject *args)
{
    PyObject *py_list;
    int n;

    // Parse Python arguments
    if (!PyArg_ParseTuple(args, "Oi", &py_list, &n))
    {
        return NULL;
    }

    // Convert Python list to C array
    double **A = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_GetItem(py_list, i);
        A[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++)
        {
            PyObject *py_val = PyList_GetItem(py_row, j);
            A[i][j] = PyFloat_AsDouble(py_val);
        }
    }

    // Call C function
    double **result = ddg(A, n);

    // Convert C array to Python list
    PyObject *py_result = PyList_New(n);
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_New(n);
        for (int j = 0; j < n; j++)
        {
            PyList_SetItem(py_row, j, PyFloat_FromDouble(result[i][j]));
        }
        PyList_SetItem(py_result, i, py_row);
    }

    // Free allocated C array and result array
    for (int i = 0; i < n; i++)
    {
        free(A[i]);
        free(result[i]);
    }
    free(A);
    free(result);

    return py_result;
}

static PyObject *py_norm(PyObject *self, PyObject *args)
{
    PyObject *py_list;
    int n;

    // Parse Python arguments
    if (!PyArg_ParseTuple(args, "Oi", &py_list, &n))
    {
        return NULL;
    }

    // Convert Python list to C array
    double **A = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_GetItem(py_list, i);
        A[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++)
        {
            PyObject *py_val = PyList_GetItem(py_row, j);
            A[i][j] = PyFloat_AsDouble(py_val);
        }
    }

    // Call C function
    double **result = norm(A, n);

    // Convert C array to Python list
    PyObject *py_result = PyList_New(n);
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_New(n);
        for (int j = 0; j < n; j++)
        {
            PyList_SetItem(py_row, j, PyFloat_FromDouble(result[i][j]));
        }
        PyList_SetItem(py_result, i, py_row);
    }

    // Free allocated C array and result array
    for (int i = 0; i < n; i++)
    {
        free(A[i]);
        free(result[i]);
    }
    free(A);
    free(result);

    return py_result;
}

// Define the methods that will be available in the Python module
static PyMethodDef SymNMFMethods[] = {
    {"sym", py_sym, METH_VARARGS, "Calculate sym"},
    {"ddg", py_ddg, METH_VARARGS, "Calculate ddg"},
    {"norm", py_norm, METH_VARARGS, "Calculate norm"},
    {NULL, NULL, 0, NULL} // Sentinel
};

// Define the Python module
static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmfmodule", // Module name
    NULL,           // Module documentation (NULL means no documentation)
    -1,
    SymNMFMethods // Methods the module exposes
};

// Initialization function for the module
PyMODINIT_FUNC PyInit_symnmfmodule(void)
{
    return PyModule_Create(&symnmfmodule);
}

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

// Function to calculate and output the normalized similarity matrix
double **norm(double **A, int n)
{
    double **W = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        W[i] = malloc(n * sizeof(double));
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double sqrtDegreeI = sqrt(A[i][i]);
            double sqrtDegreeJ = sqrt(A[j][j]);
            W[i][j] = A[i][j] / (sqrtDegreeI * sqrtDegreeJ);
        }
    }
    return W; // Return the dynamically allocated array
}
/*endregion goals functions*/