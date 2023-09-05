#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *fit(PyObject *self, PyObject *args)
{
    PyObject *centroidspy;
    PyObject *vectorspy;
    int Kpy;
    int iterpy;

    /*Parse the input tuple*/
    if (!PyArg_ParseTuple(args, "OOii", &centroidspy, &vectorspy, &Kpy, &iterpy))
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

    // return the 2d array of the fit function using both arrays and the ints
    return Py_BuildValue("OOii", c_array, v_array, Kpy, iterpy);
}

static PyMethodDef fitmethod[] = {
    {"fit",                         /* the Python method name that will be used */
     (PyCFunction)fit,              /* the C-function that implements the Python function and returns static PyObject*  */
     METH_VARARGS,                  /* flags indicating parameters accepted for this function */
     PyDoc_STR("kmeans algorith")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}           /* The last entry must be all NULL as shown to act as a
                                       sentinel. Python looks for this entry to know that all
                                       of the functions for the module have been defined. */
};
static struct PyModuleDef mykmeanssp = {
    PyModuleDef_HEAD_INIT,
    "fit",    /* name of module */
    NULL,     /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    fitmethod /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_fit(void)
{
    PyObject *m;
    m = PyModule_Create(&mykmeanssp);
    if (!m)
    {
        return NULL;
    }
    return m;
}
