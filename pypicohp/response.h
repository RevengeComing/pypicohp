#ifndef PARSER_H

#ifndef PYTHON_H
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#endif

#ifndef PYTHONSTRUCTMEMBER_H
#include <structmember.h>
#endif

#ifndef PICOHTTPPARSER_H
#include "picohttpparser.h"
#endif

typedef struct
{
    PyObject_HEAD;
    PyObject *protocol;

    bool has_message_begin;

    bool has_url;
    char *url;
    int url_len;

    bool has_header_parsed;

} ParserObject;

#endif
