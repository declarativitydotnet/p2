#include <element.h>
#include <loop.h>
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/utility.hpp>
using namespace boost::python;
using namespace boost::python::api;

class ElementWrap : public Element, public wrapper<Element>
{
public:
  ElementWrap(std::string n) 
    : Element(n) {};
  ElementWrap(std::string n, int i, int o) 
    : Element(n, i, o) {};

  int py_push(int port, TuplePtr tp, object callback) {
    return output(port)->push(tp, boost::function<void (void)>(callback));
  }

  TuplePtr py_pull(int port, object callback) {
    return input(port)->pull(boost::function<void (void)>(callback));
  }

  virtual const char *processing() const {
    if (override f = this->get_override("processing")) {
      return f();
    }
    return Element::processing();
  }

  virtual const char *flow_code() const {
    if (override f = this->get_override("flow_code")) {
      return f();
    }
    return Element::flow_code();
  }

  virtual const char* class_name() const {
    return this->get_override("class_name")();
  }

  virtual int initialize() {
    if (override f = this->get_override("initialize")) {
      return f();
    }
    return Element::initialize();
  }

  virtual int push(int port, TuplePtr tp, b_cbv cb) {
    if (override f = this->get_override("push")) {
      return f(port, tp, cb);
    }
    return Element::push(port, tp, cb);
  }

  virtual TuplePtr pull(int port, b_cbv cb) {
    if (override f = this->get_override("pull")) {
      return f(port, cb);
    }
    return Element::pull(port, cb);
  }

  virtual TuplePtr simple_action(TuplePtr p) {
    if (override f = this->get_override("simple_action")) {
      return f(p);
    }
    return Element::simple_action(p);
  }

  timeCBHandle* set_delay(double secondDelay, object callback) {
    return delayCB(secondDelay, 
                   boost::function<void (void)>(callback)); 
  }

  void cancel_delay(timeCBHandle* handle) {
    ::timeCBRemove(handle);
  }
};

unsigned (Element::*ai_void)(void)      = &Element::add_input;
unsigned (Element::*ai_value)(ValuePtr) = &Element::add_input;
unsigned (Element::*ao_void)(void)      = &Element::add_output;
unsigned (Element::*ao_value)(ValuePtr) = &Element::add_output;
int      (Element::*ri_int)(unsigned)   = &Element::remove_input;
int      (Element::*ri_value)(ValuePtr) = &Element::remove_input;
int      (Element::*ro_int)(unsigned)   = &Element::remove_output;
int      (Element::*ro_value)(ValuePtr) = &Element::remove_output;

void export_element()
{
scope outer =
  class_<ElementWrap, boost::shared_ptr<ElementWrap>, boost::noncopyable>
        ("Element", init<std::string>())
    .def(init<std::string, int, int>())
  
    .def("class_name", pure_virtual(&Element::class_name))
  
    .def("name",           &Element::name)
    .def("push",           &Element::push)
    .def("pull",           &Element::pull)
    .def("simple_action",  &Element::simple_action)
  
    .def("ninputs",        &Element::ninputs)
    .def("noutputs",       &Element::noutputs)
    .def("ports_frozen",   &Element::ports_frozen)
  
    .def("add_input",     ai_void)
    .def("add_input",     ai_value)
    .def("add_output",    ao_void)
    .def("add_output",    ao_value)
    .def("remove_input",  ri_int)
    .def("remove_input",  ri_value)
    .def("remove_output", ro_int)
    .def("remove_output", ro_value)

    // CONFIGURATION
    .def("connect_input",  &Element::connect_input)
    .def("connect_output", &Element::connect_output)
    .def("initialize",     &Element::initialize)
    
    // PROCESSING, FLOW, AND FLAGS
    .def("processing",     &Element::processing)
    .def("flow_code",      &Element::flow_code)
    .def("flags",          &Element::flags)

    // PYTHON SUPPORT FOR ELEMENTS
    .def("push",           &ElementWrap::push)
    .def("pull",           &ElementWrap::pull)
    .def("simple_action",  &ElementWrap::simple_action)
    .def("py_push",        &ElementWrap::py_push)
    .def("py_pull",        &ElementWrap::py_pull)
    .def("set_delay",      &ElementWrap::set_delay, return_internal_reference<>())
    .def("cancel_delay",   &ElementWrap::cancel_delay)
  ; 

  class_<Element::Exception, Element::Exception*> ("Exception", init<string>())
  ;
}
