#ifndef ENCODER_H
#define ENCODER_H
#include "Python.h"
#include "orientc.h"

using namespace Orient;
using namespace std;

class PyRecWriter{
 public:
  const unsigned char* serialize(PyObject* pyrec, int *size);
  
  PyRecWriter() ;
  ~PyRecWriter() ;
 private:
  RecordWriter* writer;
  void write_record(PyObject *pyrec);
  void write_value(PyObject *pyval);
  void write_list(PyObject* pylist);
  void write_dict(PyObject* pydict);
  void write_int(PyObject* pyval);
  void write_long(PyObject* pyval);
  void write_float(PyObject* pyval);
  void write_binary(PyObject* pyval);
  void write_date(PyObject* pyval);
  void write_datetime(PyObject* pyval);
  void write_link(PyObject* pylink);
  void write_ridbagtreekey();
};

#endif
