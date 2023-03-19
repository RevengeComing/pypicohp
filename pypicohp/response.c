#include "parser.h"

static PyObject *
Parser_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    ParserObject *self;
    self = (ParserObject *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static void
Parser_dealloc(ParserObject *obj)
{
    // Py_XDECREF(obj->pyheaders);
    // free(obj->buf);
    Py_TYPE(obj)->tp_free((PyObject *)obj);
}

static int
Parser_init(ParserObject *self, PyObject *args, PyObject *kwds)
{
    PyObject *protocol;

    if (!PyArg_ParseTuple(args, "O", &protocol))
    {
        PyErr_SetString(PyExc_Exception, "Invalid input");
        return 1;
    }
    self->protocol = protocol;

    return 0;
}

static int
Parser_feed_data(ParserObject *self, PyObject *args, PyObject *kwds)
{
    char *data;
    Py_ssize_t size;

    if (!PyArg_ParseTuple(args, "s#", &data, &size))
    {
        PyErr_SetString(PyExc_Exception, "Invalid input");
        return NULL;
    }

    return 0;
}

static PyMemberDef Parser_members[] = {
    {NULL} /* Sentinel */
};

static PyMethodDef Parser_methods[] = {
    {"feed_data", (PyCFunction)Parser_feed_data, METH_VARARGS,
     "feed_data docstring"},
    {NULL} /* Sentinel */
};

static PyTypeObject ParserType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "pypicohp..Parser",
    .tp_doc = "Parser objects",
    .tp_basicsize = sizeof(ParserObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = Parser_new,
    .tp_init = (initproc)Parser_init,
    // .tp_alloc = (allocfunc)Parser_alloc, TODO: use freelist (?)
    .tp_dealloc = (destructor)Parser_dealloc,
    .tp_members = Parser_members,
    .tp_methods = Parser_methods,
};

static PyMethodDef ParserMethods[] = {
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static PyModuleDef parsermodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "parser",
    .m_doc = "Parser module that creates a parser type.",
    .m_size = -1,
    ParserMethods,
};

PyMODINIT_FUNC
PyInit_parser(void)
{
    PyObject *m;
    if (PyType_Ready(&ParserType) < 0)
        return NULL;

    m = PyModule_Create(&parsermodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&ParserType);
    if (PyModule_AddObject(m, "Parser", (PyObject *)&ParserType) < 0)
    {
        Py_DECREF(&ParserType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
};
