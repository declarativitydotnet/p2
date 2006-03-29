#include <duplicateConservative.h>
#include <boost/python.hpp>

using namespace boost::python;

void export_duplicateConservative()
{
  class_<DuplicateConservative, bases<Element>, boost::shared_ptr<DuplicateConservative>, boost::noncopyable>
        ("DuplicateConservative", init<std::string, int>())
    .def("class_name", &DuplicateConservative::class_name)
    .def("processing", &DuplicateConservative::processing)
    .def("flow_code",  &DuplicateConservative::flow_code)
  ;
}
