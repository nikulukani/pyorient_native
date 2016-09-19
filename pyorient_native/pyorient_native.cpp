#include "Python.h"
#include "orientc.h"
#include "listener.h"
#include "encoder.h"

#include <iostream>
using namespace Orient;
using namespace std;


static PyObject*
native_deserialize(PyObject *self, PyObject *args){
  PyObject * pycontent;
  int len;
  PyObject * props  = NULL;
  PyArg_ParseTuple(args, "|SiO", &pycontent, &len, &props);

  RecordParser reader("onet_ser_v0");
  TrackerListener* listener;

  listener = new TrackerListener(props);
  #if PY_MAJOR_VERSION >= 3
  reader.parse((unsigned char*)PyBytes_AsString(pycontent), len, *listener);
  #else
  reader.parse((unsigned char*)PyString_AsString(pycontent), len, *listener);
  #endif
  PyObject *ret = listener->obj;
  delete listener;
  //Py_XDECREF(pycontent);
  return ret;
}

static PyObject* native_serialize(PyObject* self, PyObject *args){
  PyObject *pyrec;
  int size;
  const char *content;
  PyArg_ParseTuple(args, "O", &pyrec);

  PyRecWriter writer;
  content = (const char *) writer.serialize(pyrec, &size);

  #if PY_MAJOR_VERSION >= 3
  return PyBytes_FromStringAndSize(content, size);
  #else
  return PyString_FromStringAndSize(content, size);
  #endif
  
}



static PyMethodDef native_methods[] = {
  {"deserialize", native_deserialize, METH_VARARGS, ""},
  {"serialize", native_serialize, METH_VARARGS, ""},
  {NULL, NULL,0, NULL}
};


#if PY_MAJOR_VERSION >= 3
#define MOD_DEF(ob, name, doc, methods) \
  static struct PyModuleDef moduledef = { \
    PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
  ob = PyModule_Create(&moduledef);
#else
#define MOD_DEF(ob, name, doc, methods) \
  ob = Py_InitModule3(name, methods, doc);
#endif


static PyObject *
moduleinit(void)
{
  PyObject *m;
    MOD_DEF(m, "pyorient_native",
          "This is the module docstring",
          native_methods)

    if (m == NULL)
      return NULL;

  
  return m;
}

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initpyorient_native(void)
{
  moduleinit();
}
#else
PyMODINIT_FUNC PyInit_pyorient_native(void)
{
  return moduleinit();
}
#endif



