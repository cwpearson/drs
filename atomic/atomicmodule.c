#include <Python.h>


// The function that will be called for atomic.add_double()
static PyObject *
atomic_add_double(PyObject *self, PyObject *args)
{
    double d;
    int sts;

    if (!PyArg_ParseTuple(args, "f", &d))
        return NULL;
    sts = system(command);
    return Py_BuildValue("i", sts);
}

static PyMethodDef AtomicMethods[] = {
    {"add_double",  atomic_add_double, METH_VARARGS,
     "Atomic add on an array"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initatomic(void)
{
    (void) Py_InitModule("atomic", AtomicMethods);
}
