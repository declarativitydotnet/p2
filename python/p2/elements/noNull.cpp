#include <noNull.h>
#include <boost/python.hpp>
using namespace boost::python;

void export_noNull()
{
  class_<NoNull, bases<Element>, boost::shared_ptr<NoNull>, boost::noncopyable>
        ("NoNull", init<std::string>())
    .def("class_name", &NoNull::class_name)
    .def("processing", &NoNull::processing)
    .def("flow_code",  &NoNull::flow_code)
  ;
}
