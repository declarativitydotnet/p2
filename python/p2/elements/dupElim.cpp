#include <dupElim.h>
#include <boost/python.hpp>

using namespace boost::python;

void export_dupElim()
{
  class_<DupElim, bases<Element>, boost::shared_ptr<DupElim>, boost::noncopyable>
        ("DupElim", init<std::string>())
    .def("class_name", &DupElim::class_name)
    .def("processing", &DupElim::processing)
    .def("flow_code",  &DupElim::flow_code)
  ;
}
