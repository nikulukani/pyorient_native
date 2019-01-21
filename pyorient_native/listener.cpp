#include "Python.h"
#include "orientc_reader.h"
#include "listener.h"
#include <iostream>
#include <stack>
#include "stdio.h"
#include "time.h"
#include "math.h"
#include "datetime.h"
#include "string.h"
#include "version_compare.h"

using namespace Orient;
using namespace std;

#if PY_MAJOR_VERSION >= 3
    #define PyInt_FromLong PyLong_FromLong
    #define PyString_FromStringAndSize PyUnicode_FromStringAndSize
#endif


void TrackerListener::startDocument(const char * name,size_t name_length) {
  PyObject *cur = this->obj_stack.top();
  OType cur_type = this->types_stack.top();
  PyObject *nw = PyDict_New(); 
  if (name_length > 0){
    PyObject *temp = PyString_FromStringAndSize(name, name_length);
    PyList_Append(cur, temp);
    Py_XDECREF(temp);
  }
  else{
    if (!PyList_Size(this->obj)){
      PyList_Append(this->obj,Py_None);
    }
  }
  switch(cur_type){
    case EMBEDDEDMAP:
      PyDict_SetItemString(cur, this->cur_field->c_str(), nw);
      Py_XDECREF(nw);
      break;
    default:
      PyList_Append(cur, nw);
      Py_XDECREF(nw);
      break;
  }
  this->types_stack.push(EMBEDDEDMAP);
  this->obj_stack.push(nw);
}

void TrackerListener::endDocument() {
  this->types_stack.pop();
  this->obj_stack.pop();
}

void TrackerListener::startField(const char * name,size_t name_length, OType type) {
  delete this->cur_field;
  this->cur_field = new string(name, name_length);
}

void TrackerListener::endField(const char * name,size_t name_length) {
}

void TrackerListener::stringValue(const char * value,size_t value_length) {
  PyObject* val = PyString_FromStringAndSize(value, value_length);
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val);
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val);
      break;
  }
}

void TrackerListener::intValue(long value) {
  
  PyObject* val = PyInt_FromLong(value);
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val);
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val);
      break;
  }
  
}

void TrackerListener::longValue(long long value) {
  PyObject* val = PyLong_FromLong(value);
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val);
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val);
      break;
  }
}

void TrackerListener::shortValue(short value) {
  
  PyObject* val = PyInt_FromLong(value);
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val); 
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val); 
      break;
  }
  
}

void TrackerListener::byteValue(char value) {
  
  #if PY_MAJOR_VERSION >= 3
  PyObject* val = PyBytes_FromFormat("%c",value);
  #else
  PyObject* val = PyString_FromFormat("%c",value);
  #endif
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val); 
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val); 
      break;
  }
  
}

void TrackerListener::booleanValue(bool value) {
  PyObject* val = value? Py_True: Py_False;
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      break;
  }
}

void TrackerListener::floatValue(float value) {
  PyObject* val = PyFloat_FromDouble(value);
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val); 
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val); 
      break;
  }
}

void TrackerListener::doubleValue(double value) {
  PyObject* val = PyFloat_FromDouble(value);
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val); 
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val); 
      break;
      }
}

void TrackerListener::decimalValue(int scale , const char * bytes, int bytes_length){
  long long v = 0;
  int i=bytes_length-1;
  do{
    v |= ((long long) bytes[i]) << (i*8);
    i--;
  }while (i>=0);
  double vd = double(v);
  vd *= pow((double)10,-scale); 
  PyObject* val = PyFloat_FromDouble(vd);
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val); 
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val); 
      break;
  }
  }

void TrackerListener::binaryValue(const char * value, int length) {
  #if PY_MAJOR_VERSION >= 3
  PyObject* val = PyBytes_FromStringAndSize(value, length);
  #else
  PyObject* val = PyString_FromStringAndSize(value, length);
  #endif
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val); 
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val); 
      break;
      }
}

void TrackerListener::dateValue(long long value) {
  long long val_in_sec = (long long) (value/1000);
  struct tm* t = gmtime((time_t *) &val_in_sec);
  PyObject* val = PyDate_FromDate(t->tm_year+1900, t->tm_mon+1, t->tm_mday);
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val); 
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val); 
      break;
  }
}

void TrackerListener::dateTimeValue(long long value) {
  long long val_in_sec = (long long) (value/1000);
  struct tm* t =  localtime((time_t *) &val_in_sec);
  PyObject* val = PyDateTime_FromDateAndTime(t->tm_year+1900, t->tm_mon+1, t->tm_mday,
                                         t->tm_hour, t->tm_min, t->tm_sec, (int)(value % 1000)*1000);
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val); 
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val); 
      break;
  }
}

void TrackerListener::linkValue(struct Link &value) {
  PyObject *val;
  if(this->legacy_link){
    val = Py_BuildValue("(slL)", "OrientRecordLink",value.cluster, value.position);
  }
  else{
    char recordLink[32];
    sprintf(recordLink, "%ld:%lld", value.cluster, value.position);
    PyObject* module = PyImport_ImportModule("pyorient.otypes");
    val = PyObject_CallMethod(module, "OrientRecordLink", "(s)",recordLink);
  }
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), val);
      Py_XDECREF(val);
      break;
    default:
      PyList_Append(this->obj_stack.top(), val);
      Py_XDECREF(val); 
      break;
  }
}

void TrackerListener::startCollection(int size,OType type) {
  PyObject *cur = this->obj_stack.top();
  PyObject *nw = PyList_New(0); 
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(cur, this->cur_field->c_str(), nw);
      Py_XDECREF(nw); 
      break;
    default:
      PyList_Append(cur,nw);
      Py_XDECREF(nw); 
      break;
  }
  this->types_stack.push(type);
  this->obj_stack.push(nw);
}

void TrackerListener::startMap(int size,OType type) {
  PyObject *cur = this->obj_stack.top();
  OType cur_type = this->types_stack.top();
  PyObject *nw = PyDict_New(); 
  switch(cur_type){
    case EMBEDDEDMAP:
      PyDict_SetItemString(cur, this->cur_field->c_str(), nw);
      Py_XDECREF(nw);
      break;
    default:
      PyList_Append(cur, nw);
      Py_XDECREF(nw);
      break;
  }
  this->types_stack.push(EMBEDDEDMAP);
  this->obj_stack.push(nw);
  
}

void TrackerListener::mapKey(const char *key,size_t key_size) {
  delete this->cur_field;
  this->cur_field = new string(key, key_size);
}

void TrackerListener::ridBagTreeKey(long long fileId,long long pageIndex,long pageOffset) {
  //cout << "Warning: ridBagTreeKey not Implemented " << endl;
}

void TrackerListener::nullValue() {
  switch(this->types_stack.top()){
    case EMBEDDEDMAP:
      PyDict_SetItemString(this->obj_stack.top(), this->cur_field->c_str(), Py_None);
      break;
    default:
      PyList_Append(this->obj_stack.top(), Py_None);
      break;
  }
}

void TrackerListener::endMap(OType type) {
  this->types_stack.pop();
  this->obj_stack.pop();
  
}

void TrackerListener::endCollection(OType type) {
  this->types_stack.pop();
  this->obj_stack.pop();
}


TrackerListener::TrackerListener(PyObject* props) {
  this->cur_field = new string();
  this->obj = PyList_New(0);
  this->types_stack.push(EMBEDDEDLIST);
  this->obj_stack.push(this->obj);
  this->props = props;

  /* If pyorient version is <= 1.5.5, set legacy_link and
     return links as tuples of form "OrientRecordLink, <clusterId>, <position>)
     else return instances of OrientRecordLink */
  PyObject *pyorient_constants = PyImport_ImportModule("pyorient.constants");
  PyObject *version_pystring = PyObject_GetAttrString(pyorient_constants, "VERSION");
  char *version = PyString_AsString(version_pystring);
  Py_XDECREF(version_pystring);
  if((Version)(version) > (Version)("1.5.5"))
    this->legacy_link = false;
  else
    this->legacy_link = true;

  PyDateTime_IMPORT;
}

TrackerListener::~TrackerListener() {
  while ( !this->types_stack.empty() )
    this->types_stack.pop(); 
  while ( !this->obj_stack.empty() ){
    this->obj_stack.pop();
  }
  delete this->cur_field;
}
