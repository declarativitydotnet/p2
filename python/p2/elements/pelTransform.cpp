#include <pelTransform.h>
#include <boost/python.hpp>
using namespace boost::python;

void export_pelTransform()
{
  class_<PelTransform, bases<Element>, boost::shared_ptr<PelTransform>, boost::noncopyable>
        ("PelTransform", init<std::string, std::string>())
    .def("class_name", &PelTransform::class_name)
    .def("processing", &PelTransform::processing)
    .def("flow_code",  &PelTransform::flow_code)
  ;
}
