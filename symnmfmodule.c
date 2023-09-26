#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "symnmf.h"

<<<<<<< HEAD
printf("test");

// Python wrapper functions
=======
/*Python wrapper functions*/
>>>>>>> ea54a255ef7fb9271ea136fc4fbe7518e3c5d1b0
static PyObject *py_sym(PyObject *self, PyObject *args)
{
    PyObject *py_list;
    int n, m;

    /*Parse Python arguments.*/
    if (!PyArg_ParseTuple(args, "Oii", &py_list, &n, &m))
    {
        return NULL;
    }

    /*Convert Python list to C array*/
    double **X = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_GetItem(py_list, i);
        X[i] = malloc(m * sizeof(double));
        for (int j = 0; j < m; j++)
        {
            PyObject *py_val = PyList_GetItem(py_row, j);
            if (!py_val)
            {
                printf("Failed to get value at (%d, %d)\n", i, j);
                return NULL;
            }
            X[i][j] = PyFloat_AsDouble(py_val);
        }
    }

    double **result = sym(X, n, m);

    /*Convert C array back to Python list*/
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

    /* Free allocated C array and result array*/
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

    /*Parse Python arguments*/
    if (!PyArg_ParseTuple(args, "Oi", &py_list, &n))
    {
        return NULL;
    }

    /*Convert Python list to C array*/
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

    /*Call C function ddg*/
    double **result = ddg(A, n);

    /*Convert C array to Python list*/
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

    /*Free allocated C array and result array*/
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

    /*Parse Python arguments*/
    if (!PyArg_ParseTuple(args, "Oi", &py_list, &n))
    {
        return NULL;
    }

    /* Convert Python list to C array*/
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

    /*Call C function norm*/
    double **result = norm(A, n);

    /*Convert C array to Python list*/
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

    /*Free allocated C array and result array*/
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
    PyObject *py_list_H, *py_list_W;
    double **H, **W;
    int n, k;

    /*Parse Python arguments*/
    if (!PyArg_ParseTuple(args, "OOii", &py_list_H, &py_list_W, &n, &k))
    {
        return NULL;
    }

    /*Convert Python list to C array for H*/
    H = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_GetItem(py_list_H, i);
        H[i] = malloc(k * sizeof(double));
        for (int j = 0; j < k; j++)
        {
            PyObject *py_value = PyList_GetItem(py_row, j);
            H[i][j] = PyFloat_AsDouble(py_value);
        }
    }

    /*Convert Python list to C array for W*/
    W = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_GetItem(py_list_W, i);
        W[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++)
        {
            PyObject *py_value = PyList_GetItem(py_row, j);
            W[i][j] = PyFloat_AsDouble(py_value);
        }
    }

    /*Call C function*/
    double **result = symnmf(H, W, n, k);

    /*Convert C array to Python list*/
    PyObject *py_result = PyList_New(n);
    for (int i = 0; i < n; i++)
    {
        PyObject *py_row = PyList_New(k);
        for (int j = 0; j < k; j++)
        {
            PyList_SetItem(py_row, j, PyFloat_FromDouble(result[i][j]));
        }
        PyList_SetItem(py_result, i, py_row);
    }

    return py_result;
}

/*Define the methods that will be available in the Python module*/
static PyMethodDef symnmfC_methods[] = {
    {"sym", py_sym, METH_VARARGS, "Calculate sym"},
    {"ddg", py_ddg, METH_VARARGS, "Calculate ddg"},
    {"norm", py_norm, METH_VARARGS, "Calculate norm"},
    {"symnmff", py_symnmf, METH_VARARGS, "Calculate symnmf"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef symnmfC_module = {
    PyModuleDef_HEAD_INIT,
    "symnmfC", /* name of module */
    NULL,      /* module documentation, may be NULL */
    -1,        /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    symnmfC_methods};

/*Initialization function for the module*/
PyMODINIT_FUNC PyInit_symnmfC(void)
{
    return PyModule_Create(&symnmfC_module);
}
