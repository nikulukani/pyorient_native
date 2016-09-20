#include "Python.h"
#include "encoder.h"
#include "orientc.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"
#include "datetime.h"
#include <iostream>

using namespace Orient;
using namespace std;


const unsigned char* PyRecWriter::serialize(PyObject* pyrec, int *size){
  write_record(pyrec);
  return this->writer->writtenContent(size);
}
  
PyRecWriter::PyRecWriter(){
  this->writer = new RecordWriter("onet_ser_v0");
  PyDateTime_IMPORT;
}

PyRecWriter::~PyRecWriter() {
  delete this->writer;
}

void PyRecWriter::write_record(PyObject *pyrec){
  PyObject *reccls = PyObject_GetAttrString(pyrec,"_class"); // new ref
  if(reccls!=Py_None){
    this->writer->startDocument(PyString_AsString(reccls));
  }
  else{
    this->writer->startDocument("");
  }
  PyObject* rec_data = PyObject_GetAttrString(pyrec,"oRecordData"); // new ref
  int size = PyDict_Size(rec_data);            
  PyObject *keys = PyDict_Keys(rec_data);     // new ref
  PyObject *key;
  PyObject *val;
  int i;
  for(i=0;i<size;i++){
    key = PyList_GetItem(keys, i);          // Borrowed reference
    val = PyDict_GetItem(rec_data, key);    // Borrowed reference
    if(!PyString_Check(key)){
      key = PyObject_Str(key);              // new ref
      this->writer->startField(PyString_AsString(key));
      Py_XDECREF(key);
    }
    else{
      this->writer->startField(PyString_AsString(key));
    }
    write_value(val);
    this->writer->endField(PyString_AsString(key));
  }
  this->writer->endDocument();
  Py_XDECREF(reccls);
  Py_XDECREF(rec_data);
  Py_XDECREF(keys);
}

void PyRecWriter::write_link(PyObject *pylink){
  Link link;
  PyObject *temp = PyObject_GetAttrString(pylink,"clusterID");
  char* cluster = PyString_AsString(temp);
  PyObject *temp1 = PyObject_GetAttrString(pylink,"recordPosition");
  char* position = PyString_AsString(temp1);
  link.cluster = atol(cluster);
  link.position = atoll(position);
  this->writer->linkValue(link);
  Py_XDECREF(temp);
  Py_XDECREF(temp1);
}

void PyRecWriter::write_list(PyObject* pylist){
  int size = PyList_Size(pylist);
  OType type = EMBEDDEDLIST;
  if(!size){
    this->writer->startCollection(0,type);
    this->writer->endCollection(type);
    return;
  }
  // See if it is a link list (list of links)
  
  PyObject *val0=PyList_GetItem(pylist, 0);   // borrowed ref
  // new refs
  PyObject *temp = PyObject_GetAttrString(val0,"__class__");
  PyObject *temp1 = PyObject_GetAttrString(temp,"__name__");
  char* cls = PyString_AsString(temp1);
  if(strcmp(cls,"OrientRecordLink")==0)
    type = LINKLIST;
  this->writer->startCollection(size, type);
  int i;
  for(i=0;i<size;i++){
    write_value(PyList_GetItem(pylist, i));   // borrowed ref
  }
  this->writer->endCollection(type);
  Py_XDECREF(temp);
  Py_XDECREF(temp1);
}

void PyRecWriter::write_dict(PyObject* pydict){
  int size = PyDict_Size(pydict);
  OType type = EMBEDDEDMAP;
  this->writer->startMap(size, type);
  PyObject *keys = PyDict_Keys(pydict);   // new ref
  PyObject *key;
  PyObject *val;
  int i;
  for(i=0;i<size;i++){
    key = PyList_GetItem(keys, i);        // borrowed ref
    val = PyDict_GetItem(pydict, key);    // borrowed ref
    if(!PyString_Check(key)){
      key = PyObject_Str(key);            // new ref
      this->writer->mapKey(PyString_AsString(key));
      Py_XDECREF(key);
    }
    else{
      this->writer->mapKey(PyString_AsString(key));
    }
    write_value(val);
  }
  this->writer->endMap(type);
  Py_XDECREF(keys);
}

void PyRecWriter::write_int(PyObject* pyval){
  // TODO: Python >= 2.7 does not distinguish bet int and long by default
    //       BitLength should be used to figure out whether call write_int
    //       or write_long
  int val = (int) PyInt_AsLong(pyval);
  if (val == -1 && PyErr_Occurred()!=NULL){
    PyObject *temp = PyObject_GetAttrString(pyval,"__class__");
    PyObject *temp1 = PyObject_GetAttrString(temp,"__name__");
    char* cls = PyString_AsString(temp1);
    cout << "Error while converting to int from python object of class" <<
      cls << endl << flush;
    Py_XDECREF(temp);
    Py_XDECREF(temp1);
    return;
  }
  this->writer->intValue(val);
}

void PyRecWriter::write_long(PyObject* pyval){
  long val = PyLong_AsLong(pyval);
  if (val == -1 && PyErr_Occurred()!=NULL){
    PyObject *temp = PyObject_GetAttrString(pyval,"__class__");
    PyObject *temp1 = PyObject_GetAttrString(temp,"__name__");
    char* cls = PyString_AsString(temp1);
    cout << "Error while converting to long from python object of class" <<
      cls << endl << flush;
    Py_XDECREF(temp);
    Py_XDECREF(temp1);
    return;
  }
  this->writer->longValue(val);
}

void PyRecWriter::write_float(PyObject* pyval){
  double val = PyFloat_AsDouble(pyval);
  if (val == -1.0 && PyErr_Occurred()!=NULL){
    PyObject *temp = PyObject_GetAttrString(pyval,"__class__");
    PyObject *temp1 = PyObject_GetAttrString(temp,"__name__");
    char* cls = PyString_AsString(temp1);
    cout << "Error while converting to float from python object of class" <<
      cls << endl << flush;
    Py_XDECREF(temp);
    Py_XDECREF(temp1);
    return;
  }
  this->writer->doubleValue(val);
}

void PyRecWriter::write_binary(PyObject* pyval){
  PyObject* obj = PyByteArray_FromObject(pyval); // new ref
  this->writer->binaryValue((const char *)PyByteArray_AsString(obj),
                             (int) PyByteArray_Size(obj));
  Py_XDECREF(obj);
}

void PyRecWriter::write_date(PyObject* pyval){
  struct tm t = {0};
  t.tm_year = PyDateTime_GET_YEAR(pyval)-1900;
  t.tm_mon = PyDateTime_GET_MONTH(pyval)-1;
  t.tm_mday = PyDateTime_GET_DAY(pyval);
  long long val = static_cast<long long>(mktime(&t));
  this->writer->dateValue(val*1000);
}

void PyRecWriter::write_datetime(PyObject* pyval){
  struct tm t = {0};
  t.tm_year = PyDateTime_GET_YEAR(pyval)-1900;
  t.tm_mon = PyDateTime_GET_MONTH(pyval)-1;
  t.tm_mday = PyDateTime_GET_DAY(pyval);
  t.tm_hour = PyDateTime_DATE_GET_HOUR(pyval);
  t.tm_min = PyDateTime_DATE_GET_MINUTE(pyval);
  t.tm_sec = PyDateTime_DATE_GET_SECOND(pyval);
  t.tm_isdst = -1;
  time_t tt = mktime(&t);
  long long val = static_cast<long long>(tt); //Seconds since EPOCH
  val *= 1000; // Convert to ms since epoch
  // Add ms precision
  val = val + (long long) (PyDateTime_DATE_GET_MICROSECOND(pyval) / 1000);
  this->writer->dateTimeValue(val);
}

void PyRecWriter::write_ridbagtreekey(){
  // Not Implemented
  return;
}

void PyRecWriter::write_value(PyObject *pyval){
  PyObject *temp = PyObject_GetAttrString(pyval,"__class__");
  PyObject *temp1 = PyObject_GetAttrString(temp,"__name__");
  char* cls = PyString_AsString(temp1);
  if(PyString_Check(pyval)){
    this->writer->stringValue(PyString_AsString(pyval));
  }
  else if(PyFloat_Check(pyval)){
    write_float(pyval);
  }
  else if(PyInt_Check(pyval)){
    write_int(pyval);
  }
  else if(PyLong_Check(pyval)){
    write_long(pyval);
  }
  // Convert sets to lists to address the fact that the current deserializatin
  // does not support deserializing to sets.
  else if(PyList_Check(pyval) || PyTuple_Check(pyval) || PySet_Check(pyval)){
    write_list(pyval);
  }
  else if(PyDict_Check(pyval)){
    write_dict(pyval);
  }
  else if(strcmp(cls, "OrientRecord")==0){
    write_record(pyval);
  }
  else if(strcmp(cls, "OrientRecordLink")==0){
    write_link(pyval);
  }
  else if(strcmp(cls, "datetime")==0){
    write_datetime(pyval);
  }
  else if(strcmp(cls, "date")==0){
    write_date(pyval);
  }
  else if(pyval == Py_None){
    this->writer->nullValue();
  }
  // Treat as binary object
  else{
    write_binary(pyval);
  }
  Py_XDECREF(temp);
  Py_XDECREF(temp1);
  
}


