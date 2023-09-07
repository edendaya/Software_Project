#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "symnmf.h"

typedef struct
{
    double **array;
    int rows;
    int cols;
} ArrayInfo;

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

static PyObject *py_symnmf(PyObject *self, PyObject *args)
{
    PyObject *py_list;
    int n, k, maxiter;
    double tol;

    // Parse Python arguments
    if (!PyArg_ParseTuple(args, "Oiiid", &py_list, &n, &k, &maxiter, &tol))
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
    ArrayInfo result = symnmf(A, n, k, maxiter, tol);

    // Convert C array to Python list
    PyObject *py_result = PyList_New(n);
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_New(n);
        for (int j = 0; j < n; j++)
        {
            PyList_SetItem(py_row, j, PyFloat_FromDouble(result.array[i][j]));
        }
        PyList_SetItem(py_result, i, py_row);
    }

    // Free allocated C array and result array
    for (int i = 0; i < n; i++)
    {
        free(A[i]);
        free(result.array[i]);
    }
    free(A);
    free(result.array);

    return py_result;
}

// Define the methods that will be available in the Python module
static PyMethodDef SymNMFMethods[] = {
    {"sym", py_sym, METH_VARARGS, "Calculate sym"},
    {"ddg", py_ddg, METH_VARARGS, "Calculate ddg"},
    {"norm", py_norm, METH_VARARGS, "Calculate norm"},
    {"symnmf", py_symnmf, METH_VARARGS, "Calculate symnmf"},
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
