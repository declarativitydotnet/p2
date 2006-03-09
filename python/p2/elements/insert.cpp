class_<Insert, bases<Element>, boost::shared_ptr<Insert>, boost::noncopyable>
      ("Insert", init<std::string, TablePtr>())
  .def("class_name", &Insert::class_name)
  .def("processing", &Insert::processing)
  .def("flow_code",  &Insert::flow_code)
;
