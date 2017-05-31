#include <Python.h>

#include <stdint.h>
#include <stdatomic.h>
#include <assert.h>
// The function that will be called for atomic.add_int()
// should take a numpy array, and offset, and an increment
static PyObject *
atomic_add_int32(PyObject *self, PyObject *args)
{
    int32_t off;
    int32_t inc;
    PyObject *np_array = NULL;

    if (!PyArg_ParseTuple(args, "Oii", &np_array, &off, &inc))
        return NULL;

    fprintf(stderr, "off=%d, inc=%d\n", off, inc);
    fprintf(stderr, "passed obj: %p\n", np_array);

    // Get a pointer to the array data
    // see https://docs.scipy.org/doc/numpy/reference/arrays.interface.html
    PyObject *array_interface = PyObject_GetAttrString(np_array, "__array_interface__");
    if (NULL == array_interface) {
      PyErr_SetString(PyExc_AttributeError, 
        "Expecting object in argument 0 to have an __array_interface__ attribute");
      return NULL;
    }

    PyObject *dataStr = PyString_FromString("data");
    PyObject *tup = PyObject_GetItem(array_interface, dataStr);
    if (NULL == tup) {
      PyErr_SetString(PyExc_LookupError, 
        "Argument 0 should provide __array_interface__['data']");
      return NULL;
    }
    if (!PyTuple_Check(tup)) {
      PyErr_SetString(PyExc_TypeError, "__array_interface__['data'] should be a tuple");
      return NULL;
    }

    PyObject *pointer_as_int = PyTuple_GetItem(tup, 0);
    if (!PyInt_Check(pointer_as_int)) {
      PyErr_SetString(PyExc_TypeError, "0th tuple item should be an integer");
      return NULL;
    }

    atomic_int *ptr = NULL;
    if (PyInt_CheckExact(pointer_as_int)) {
      long l = PyInt_AsLong(pointer_as_int);
      ptr = (void *) l; 
      
    } else if (PyLong_CheckExact(pointer_as_int)) {
      ptr = PyLong_AsVoidPtr(pointer_as_int);
    } else {
      assert(0 && "Unhandled.");
    }
    assert(ptr);
    fprintf(stderr, "pointer object:  %p\n", ptr);
    // Treat the object like a raw int and increment it
    atomic_fetch_add_explicit(ptr + off, inc, memory_order_relaxed);    
    return Py_BuildValue("i", inc);
}

static PyMethodDef AtomicMethods[] = {
    {"add_int32",  atomic_add_int32, METH_VARARGS,
     "Atomic integer addition"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initatomic(void)
{
    (void) Py_InitModule("atomic", AtomicMethods);
}
