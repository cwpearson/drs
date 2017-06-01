#include <Python.h>

#include <stdint.h>
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

    //fprintf(stderr, "off=%d, inc=%d\n", off, inc);
    //fprintf(stderr, "passed obj: %p\n", np_array);

    PyObject *ctypes = PyObject_GetAttrString(np_array, "ctypes");
    if (NULL == ctypes) {
      PyErr_SetString(PyExc_AttributeError, 
        "Expecting argument 0 to have a ctypes attribute");
      return NULL;
    }
    PyObject *data = PyObject_GetAttrString(ctypes, "data");
    if (NULL == data) {
      PyErr_SetString(PyExc_AttributeError, 
        "Expecting argument 0 to have a ctypes.data attribute");
      return NULL;
    }

    // Get a pointer to the array data
    // see https://docs.scipy.org/doc/numpy/reference/arrays.interface.html
    //PyObject *array_interface = PyObject_GetAttrString(np_array, "__array_interface__");
    //if (NULL == array_interface) {
    //  PyErr_SetString(PyExc_AttributeError, 
    //    "Expecting object in argument 0 to have an __array_interface__ attribute");
    //  return NULL;
    //}
    // PyObject *dataStr = PyString_FromString("data");
    // PyObject *tup = PyObject_GetItem(array_interface, dataStr);
    // if (NULL == tup) {
    //   PyErr_SetString(PyExc_LookupError, 
    //     "Argument 0 should provide __array_interface__['data']");
    //   return NULL;
    // }
    // if (!PyTuple_Check(tup)) {
    //   PyErr_SetString(PyExc_TypeError, "__array_interface__['data'] should be a tuple");
    //   return NULL;
    // }

    // PyObject *pointer_as_int = PyTuple_GetItem(tup, 0);
    // if (!PyInt_Check(pointer_as_int)) {
    //   PyErr_SetString(PyExc_TypeError, "0th tuple item should be an integer");
    //   return NULL;
    // }

    volatile int32_t *ptr = NULL;
    if (PyInt_CheckExact(data)) {
      long l = PyInt_AsLong(data);
      ptr = (volatile int32_t *) l; 
    } else if (PyLong_CheckExact(data)) {
      ptr = PyLong_AsVoidPtr(data);
    } else {
      PyErr_SetString(PyExc_Exception, 
        "Member should be an int or long");
      return NULL;
    }
    //fprintf(stderr, "pointer object:  %p\n", ptr);
    int32_t old = __sync_fetch_and_add(ptr + off, inc);    
    return Py_BuildValue("i", old);
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
