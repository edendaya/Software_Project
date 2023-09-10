#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "symnmf.h"

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

// Assuming symnmf is defined elsewhere
// ArrayInfo symnmf(double **H, double **W, double *vectors, int k);

static PyObject *py_symnmf(PyObject *self, PyObject *args)
{
    PyObject *py_list_H, *py_list_W, *py_list_vectors;
    double **H, **W, *vectors;
    int k;

    // Parse Python arguments
    if (!PyArg_ParseTuple(args, "OOOOi", &py_list_H, &py_list_W, &py_list_vectors, &k))
    {
        return NULL;
    }

    // Convert Python list to C array for H
    int rows_H = PyList_Size(py_list_H);
    H = malloc(rows_H * sizeof(double *));
    for (int i = 0; i < rows_H; i++)
    {
        PyObject *py_row = PyList_GetItem(py_list_H, i);
        int cols_H = PyList_Size(py_row);
        H[i] = malloc(cols_H * sizeof(double));
        for (int j = 0; j < cols_H; j++)
        {
            PyObject *py_value = PyList_GetItem(py_row, j);
            H[i][j] = PyFloat_AsDouble(py_value);
        }
    }

    // Convert Python list to C array for W
    int rows_W = PyList_Size(py_list_W);
    W = malloc(rows_W * sizeof(double *));
    for (int i = 0; i < rows_W; i++)
    {
        PyObject *py_row = PyList_GetItem(py_list_W, i);
        int cols_W = PyList_Size(py_row);
        W[i] = malloc(cols_W * sizeof(double));
        for (int j = 0; j < cols_W; j++)
        {
            PyObject *py_value = PyList_GetItem(py_row, j);
            W[i][j] = PyFloat_AsDouble(py_value);
        }
    }

    // Convert Python list to C array for vectors
    int len_vectors = PyList_Size(py_list_vectors);
    vectors = malloc(len_vectors * sizeof(double));
    for (int i = 0; i < len_vectors; i++)
    {
        PyObject *py_value = PyList_GetItem(py_list_vectors, i);
        vectors[i] = PyFloat_AsDouble(py_value);
    }

    // Call C function
    ArrayInfo result = symnmf(H, W, vectors, k);

    // Convert C array to Python list
    PyObject *py_result = PyList_New(result.rows);
    for (int i = 0; i < result.rows; i++)
    {
        PyObject *py_row = PyList_New(result.cols);
        for (int j = 0; j < result.cols; j++)
        {
            PyList_SetItem(py_row, j, PyFloat_FromDouble(result.array[i][j]));
        }
        PyList_SetItem(py_result, i, py_row);
    }

    // Free allocated C array and result array
    for (int i = 0; i < result.rows; i++)
    {
        free(H[i]);
        free(W[i]);
        free(result.array[i]);
    }
    free(H);
    free(W);
    free(vectors);
    free(result.array);

    return py_result;
}

// Define the methods that will be available in the Python module
static PyMethodDef symnmfC[] = {
    {"sym", py_sym, METH_VARARGS, "Calculate sym"},
    {"ddg", py_ddg, METH_VARARGS, "Calculate ddg"},
    {"norm", py_norm, METH_VARARGS, "Calculate norm"},
    {"symnmfC", py_symnmf, METH_VARARGS, "Calculate symnmf"},
    {NULL, NULL, 0, NULL} // Sentinel
};

// Define the Python module
static struct PyModuleDef symnmfC = {
    PyModuleDef_HEAD_INIT,
    "symnmfC", // Module name
    NULL,      // Module documentation (NULL means no documentation)
    -1,
    symnmfC // Methods the module exposes
};

// Initialization function for the module
PyMODINIT_FUNC PyInit_symnmfC(void)
{
    return PyModule_Create(&symnmfC);
}
