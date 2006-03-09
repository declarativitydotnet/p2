class_<FunctorSource, bases<Element>, boost::shared_ptr<FunctorSource>, boost::noncopyable>
      ("FunctorSource", init<std::string, FunctorSource::Generator*>())
  .def("class_name", &FunctorSource::class_name)
  .def("processing", &FunctorSource::processing)
  .def("flow_code",  &FunctorSource::flow_code)
;
