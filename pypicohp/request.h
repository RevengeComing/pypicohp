#ifndef REQUEST_H

#ifndef STDBOOL_H
#include <stdbool.h>
#endif

#ifndef PYTHON_H
#define PY_SSIZE_T_CLEAN /* Make "s#" use Py_ssize_t rather than int. */
#include <Python.h>
#endif

#ifndef PYTHONSTRUCTMEMBER_H
#include <structmember.h>
#endif

#ifndef PICOHTTPPARSER_H
#include "picohttpparser.h"
#endif

#define MAX_HEADERS 100

typedef struct
{
    PyObject_HEAD;
    PyObject *pypath;
    PyObject *pymethod;
    PyObject *pyheaders;
    PyObject *pyhttp_version;

    bool recalculate_py;
    bool chunked;

    char *buf;
    char *method, *path;
    int pret, minor_version;

    struct phr_header headers[MAX_HEADERS];
    size_t buflen, prevbuflen, method_len, path_len, num_headers, body_len;
    ssize_t rret;
} RequestObject;

#endif
