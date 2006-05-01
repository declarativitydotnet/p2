#include <overlogCompiler.h>
#include <boost/python.hpp>
using namespace boost::python;

void export_overlogCompiler()
{
  class_<OverlogCompiler, bases<Element>, boost::shared_ptr<OverlogCompiler>, boost::noncopyable>
        ("OverlogCompiler", init<std::string, string, string>())
    .def("class_name", &OverlogCompiler::class_name)
    .def("processing", &OverlogCompiler::processing)
    .def("flow_code",  &OverlogCompiler::flow_code)
  ;
}
