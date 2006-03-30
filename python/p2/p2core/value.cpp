#include <value.h>
#include <boost/python.hpp>
using namespace boost::python;

class ValueWrap : public Value, public wrapper<Value>
{
public:
  virtual unsigned int size() const {
    return this->get_override("size")();
  };
  virtual const TypeCode typeCode() const {
    return this->get_override("typeCode")();
  };
  virtual const char *typeName() const {
    return this->get_override("typeName")();
  };
  virtual std::string toString() const {
    return this->get_override("toString")();
  };
  virtual int compareTo(ValuePtr) const {
    return this->get_override("compareTo")();
  };
};

void export_value()
{
  class_<ValueWrap, boost::shared_ptr<ValueWrap>, boost::noncopyable>
        ("Value", no_init)
    .def("size",      &Value::size)
    .def("typeCode",  &Value::typeCode)
    .def("typeName",  &Value::typeName)
    .def("toString",  &Value::toString)
    .def("equals",    &Value::equals)
    .def("compareTo", &Value::compareTo)
  ; 
}
