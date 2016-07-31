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

  reader.parse((unsigned char*)PyString_AsString(pycontent), len, *listener);
  return listener->obj;
}

static PyObject* native_serialize(PyObject* self, PyObject *args){
  PyObject *pyrec;
  int size;
  const char *content;
  PyArg_ParseTuple(args, "O", &pyrec);

  PyRecWriter writer;
  content = (const char *) writer.serialize(pyrec, &size);

  cout << content << endl << flush;
  return PyString_FromStringAndSize(content, size);
}



static PyMethodDef native_methods[] = {
  {"deserialize", native_deserialize, METH_VARARGS, ""},
  {"serialize", native_serialize, METH_VARARGS, ""},
  {NULL, NULL,0, NULL}
};


PyMODINIT_FUNC
initpyorient_native(void)
{
  (void) Py_InitModule("pyorient_native", native_methods);
}


