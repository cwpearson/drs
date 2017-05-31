#include <Python.h>

#include <stdint.h>
#include <stdatomic.h>

// The function that will be called for atomic.add_int()
// should take a numpy array, and offset, and an increment
static PyObject *
atomic_add_int(PyObject *self, PyObject *args)
{
    int32_t off;
    int32_t inc;
    PyObject *np_array;

    if (!PyArg_ParseTuple(args, "Oii", &np_array, &off, &inc))
        return NULL;

    fprintf(stderr, "off=%d, inc=%d\n", off, inc);
    //fprintf(stderr, "%p\n", objptr);

    // Treat the object like a raw int and increment it
    //atomic_fetch_add_explicit((atomic_int*)objptr, inc, memory_order_relaxed);    
    return Py_BuildValue("i", inc);
}

static PyMethodDef AtomicMethods[] = {
    {"add_int",  atomic_add_int, METH_VARARGS,
     "Atomic integer addition"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initatomic(void)
{
    (void) Py_InitModule("atomic", AtomicMethods);
}
