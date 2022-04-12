#include "request.h"

static PyObject *
Request_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    RequestObject *self;
    self = (RequestObject *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static void
Request_dealloc(RequestObject *obj)
{
    Py_XDECREF(obj->pyheaders);
    free(obj->buf);
    Py_TYPE(obj)->tp_free((PyObject *)obj);
}

static int
Request_init(RequestObject *self, PyObject *args, PyObject *kwds)
{
    self->recalculate_py = true;
    self->chunked = false;
    return 0;
}

static PyObject *
Request_feed_data(RequestObject *self, PyObject *args, PyObject *kwds)
{
    char *data;
    Py_ssize_t size;

    if (!PyArg_ParseTuple(args, "s#", &data, &size))
    {
        PyErr_SetString(PyExc_Exception, "Invalid input");
        return NULL;
    }

    self->buf = (char *)realloc(self->buf, sizeof(char) * (self->buflen + size + 1));
    if (self->buflen == 0)
    {
        self->buf[0] = '\0';
    }
    strcat(self->buf, data);
    self->prevbuflen = self->buflen;
    self->buflen += size;

    self->num_headers = MAX_HEADERS;
    self->pret = phr_parse_request(self->buf, self->buflen, (const char **)&(self->method),
                                   &(self->method_len), (const char **)&(self->path), &(self->path_len),
                                   &(self->minor_version), self->headers, &(self->num_headers), self->prevbuflen);

    if (self->pret > 0)
    {
        self->recalculate_py = true;
        return PyLong_FromSize_t(self->pret);
    }

    else if (self->pret == -1)
    {
        PyErr_SetString(PyExc_Exception, "Parse Error.");
        return NULL;
    }

    if (self->buflen == sizeof(self->buf))
    {
        PyErr_SetString(PyExc_Exception, "Request is too long.");
        return NULL;
    }

    self->recalculate_py = true;
    /* request is incomplete, continue the feed */
    Py_RETURN_FALSE;
}

int is_chunked(struct phr_header headers[], size_t num_headers)
{
    for (size_t i = 0; i < num_headers; i++)
    {
        if (strcmp(headers[i].name, "transfer-encoding") == 0 && strcmp(headers[i].value, "chunked") == 0)
        {
            return 1;
        }
    }
    return 0;
}

static PyObject *
Request_get_method(RequestObject *self)
{
    if (self->pymethod == NULL || self->recalculate_py)
    {
        self->pymethod = PyBytes_FromStringAndSize(self->method, self->method_len);
    }
    return self->pymethod;
}

static PyObject *
Request_get_path(RequestObject *self)
{
    if (self->pypath == NULL || self->recalculate_py)
    {
        self->pypath = PyBytes_FromStringAndSize(self->path, self->path_len);
    }
    return self->pypath;
};

static PyObject *
Request_get_headers(RequestObject *self)
{
    if (self->pyheaders == NULL || self->recalculate_py)
    {
        self->pyheaders = PyDict_New();
        for (size_t i = 0; i != self->num_headers; ++i)
        {
            PyObject *py_value = PyBytes_FromStringAndSize(self->headers[i].value, self->headers[i].value_len);
            PyObject *py_name = PyBytes_FromStringAndSize(self->headers[i].name, self->headers[i].name_len);
            PyDict_SetItem(self->pyheaders, py_name, py_value);
            Py_DECREF(py_value);
            Py_DECREF(py_name);
        }
    }
    return self->pyheaders;
};

static PyObject *
Request_get_http_version(RequestObject *self)
{
    if (self->pyhttp_version == NULL || self->recalculate_py)
    {
        char str[6];
        sprintf(str, "1.%d", self->minor_version);
        self->pyhttp_version = PyBytes_FromStringAndSize(str, 3);
    }
    return self->pyhttp_version;
}

static PyMemberDef Request_members[] = {
    {NULL} /* Sentinel */
};

static PyMethodDef Request_methods[] = {
    {"feed_data", (PyCFunction)Request_feed_data, METH_VARARGS,
     "feed_data docstring"},
    {"get_method", (PyCFunction)Request_get_method, METH_NOARGS,
     "get_method docstring"},
    {"get_path", (PyCFunction)Request_get_path, METH_NOARGS,
     "get_path docstring"},
    {"get_headers", (PyCFunction)Request_get_headers, METH_NOARGS,
     "get_headers docstring"},
    {"get_http_version", (PyCFunction)Request_get_http_version, METH_NOARGS,
     "get_http_version docstring"},
    {NULL} /* Sentinel */
};

static PyTypeObject RequestType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "pypicohp.Request",
    .tp_doc = "Request objects",
    .tp_basicsize = sizeof(RequestObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = Request_new,
    .tp_init = (initproc)Request_init,
    // .tp_alloc = (allocfunc)Request_alloc, TODO: use freelist (?)
    .tp_dealloc = (destructor)Request_dealloc,
    .tp_members = Request_members,
    .tp_methods = Request_methods,
};

static PyMethodDef ParserMethods[] = {
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static PyModuleDef parsermodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "request",
    .m_doc = "Request module that creates an request type.",
    .m_size = -1,
    ParserMethods,
};

PyMODINIT_FUNC
PyInit_request(void)
{
    PyObject *m;
    if (PyType_Ready(&RequestType) < 0)
        return NULL;

    m = PyModule_Create(&parsermodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&RequestType);
    if (PyModule_AddObject(m, "Request", (PyObject *)&RequestType) < 0)
    {
        Py_DECREF(&RequestType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
};
