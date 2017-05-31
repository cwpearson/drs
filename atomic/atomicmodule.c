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
    PyObject *np_array;

    if (!PyArg_ParseTuple(args, "Oii", &np_array, &off, &inc))
        return NULL;

    fprintf(stderr, "off=%d, inc=%d\n", off, inc);
    fprintf(stderr, "passed obj: %p\n", np_array);

    // Get a pointer to the array data
    // see https://docs.scipy.org/doc/numpy/reference/arrays.interface.html

    PyObject *array_interface = PyObject_GetAttrString(np_array, "__array_interface__");
    assert(array_interface && "Expecting object to have an __array_interface__ attribute!");
    //fprintf(stderr, "array_interface: %p\n", array_interface);

    PyObject *dataStr = Py_BuildValue("s", "data");
    assert(dataStr);
    PyObject *tup = PyObject_GetItem(array_interface, dataStr);
    //fprintf(stderr, "tup object pointer: %p\n", tup);
    assert(PyTuple_Check(tup) && "__array_interface__['data'] should be a tuple");

    PyObject *pointer_as_int = PyTuple_GetItem(tup, 0);
    assert(PyInt_Check(pointer_as_int) && "0th tuple item should be an integer");
    //fprintf(stderr, "pointer object:  %p\n", pointer);

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
