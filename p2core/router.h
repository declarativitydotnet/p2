// -*- c-basic-offset: 2; related-file-name: "router.C" -*-
/*
 * @(#)$Id$
 * Modified from the Click Router class by Eddie Kohler
 * 
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Mazu Networks, Inc.
 * Copyright (c) 2004 Regents of the University of California
 * Copyright (c) 2004 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software")
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 * 
 * This file is distributed under the terms in the attached INTEL-LICENSE file.
 * If you do not find these files, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * 
 * DESCRIPTION: The router shell providing plumbing functionality.
 */

#ifndef __ROUTER_H__
#define __ROUTER_H__
#include <inlines.h>
#include <element.h>
#include <timer.h>
#include <sync.h>
#include <task.h>
#include <click_string.h>
//#include <click/standard/threadsched.hh>

class Master;
class ElementFilter;
class RouterThread;
class NotifierSignal;
class ThreadSched;

class Router {
 public:

  struct Handler;
  struct Hookup {
    int idx;
    int port;
    Hookup()					: idx(-1) { }
    Hookup(int i, int p)			: idx(i), port(p) { }
  };
  
  Router(const String &configuration, Master *);
  ~Router();
  void use()					{ _refcount++; }
  void unuse();

  static void static_initialize();
  static void static_cleanup();

  enum { ROUTER_NEW,
         ROUTER_PRECONFIGURE,
         ROUTER_PREINITIALIZE,
         ROUTER_LIVE,
         ROUTER_DEAD };
  bool initialized() const			{ return _state == ROUTER_LIVE; }

  // ELEMENTS
  int nelements() const				{ return _elements.size(); }
  const Vector<Element *> &elements() const	{ return _elements; }
  static Element *element(const Router *, int);
  Element *element(int e) const			{ return element(this, e); }
  // element() returns 0 on bad index/no router, root_element() on index -1
  Element *root_element() const			{ return _root_element; }
  
  const String &ename(int) const;
  const String &elandmark(int) const;
  const String &default_configuration_string(int) const;
  void set_default_configuration_string(int, const String &);
  
  Element *find(const String &, ErrorHandler * = 0) const;
  Element *find(const String &, Element *context, ErrorHandler * = 0) const;
  Element *find(const String &, String prefix, ErrorHandler * = 0) const;
  
  int downstream_elements(Element *, int o, ElementFilter*, Vector<Element*>&);
  int downstream_elements(Element *, int o, Vector<Element *> &);
  int downstream_elements(Element *, Vector<Element *> &);
  int upstream_elements(Element *, int i, ElementFilter*, Vector<Element*>&);
  int upstream_elements(Element *, int i, Vector<Element *> &);  
  int upstream_elements(Element *, Vector<Element *> &);

  // HANDLERS
  enum { FIRST_GLOBAL_HANDLER = 0x40000000 };
  int nhandlers() const				{ return _nhandlers; }
  static int nglobal_handlers();
  static int hindex(const Element *, const String &);
  static void element_hindexes(const Element *, Vector<int> &);

  // 'const Handler *' result pointers last only until a new handler is added
  static const Handler *handler(const Router *, int);
  static const Handler *handler(const Element *, int);
  const Handler *handler(int) const;
  static const Handler *handler(const Element *, const String &);
  
  static void add_read_handler(const Element *, const String &, ReadHandler, void *);
  static void add_write_handler(const Element *, const String &, WriteHandler, void *);
  static int change_handler_flags(const Element *, const String &, uint32_t clear_flags, uint32_t set_flags);

  // ATTACHMENTS AND REQUIREMENTS
  void *attachment(const String &) const;
  void *&force_attachment(const String &);
  void *set_attachment(const String &, void *);
  const Vector<String> &requirements() const	{ return _requirements; }

  ErrorHandler *chatter_channel(const String &) const;

  ThreadSched *thread_sched() const		{ return _thread_sched; }
  void set_thread_sched(ThreadSched *ts)	{ _thread_sched = ts; }
  int initial_thread_preference(Task *, bool scheduled) const;

  int new_notifier_signal(NotifierSignal &);

  // MASTER
  Master *master() const			{ return _master; }
  enum { RUNNING_DEAD = -2, RUNNING_INACTIVE = -1, RUNNING_PAUSED = 0, RUNNING_BACKGROUND = 1, RUNNING_ACTIVE = 2 };
  
  // RUNCOUNT AND RUNCLASS
  void please_stop_driver()			{ adjust_runcount(-1); }
  void reserve_driver()				{ adjust_runcount(1); }
  void set_runcount(int);
  void adjust_runcount(int);
  int runcount() const				{ return _runcount; }

  // UNPARSING
  const String &configuration_string() const	{ return _configuration; }
  void unparse(StringAccum &, const String & = String()) const;
  void unparse_requirements(StringAccum &, const String & = String()) const;
  void unparse_classes(StringAccum &, const String & = String()) const;
  void unparse_declarations(StringAccum &, const String & = String()) const;
  void unparse_connections(StringAccum &, const String & = String()) const;
  
  String element_ports_string(int) const;
  
  // INITIALIZATION
  void add_requirement(const String &);
  int add_element(Element *, const String &name, const String &conf, const String &landmark);
  int add_connection(int from_idx, int from_port, int to_idx, int to_port);
  
  int initialize(ErrorHandler *);
  void activate(bool foreground, ErrorHandler *);
  void activate(ErrorHandler *errh)		{ activate(true, errh); }

 private:

  Master *_master;
    
  volatile int _runcount;

  atomic_uint32_t _refcount;
  
  Vector<Element *> _elements;
  Vector<String> _element_names;
  Vector<String> _element_configurations;
  Vector<String> _element_landmarks;
  Vector<int> _element_configure_order;

  Vector<Hookup> _hookup_from;
  Vector<Hookup> _hookup_to;
  
  Vector<int> _input_pidx;
  Vector<int> _output_pidx;
  Vector<int> _input_eidx;
  Vector<int> _output_eidx;
  
  Vector<int> _hookpidx_from;
  Vector<int> _hookpidx_to;

  Vector<String> _requirements;

  int _state;
  mutable bool _allow_star_handler : 1;
  int _running;
  
  Vector<int> _ehandler_first_by_element;
  Vector<int> _ehandler_to_handler;
  Vector<int> _ehandler_next;

  Vector<int> _handler_first_by_name;
  
  Handler *_handlers;
  int _nhandlers;
  int _handlers_cap;

  Vector<String> _attachment_names;
  Vector<void *> _attachments;
  
  Element *_root_element;
  String _configuration;

  enum { NOTIFIER_SIGNALS_CAPACITY = 4096 };
  atomic_uint32_t *_notifier_signals;
  int _n_notifier_signals;
  ThreadSched *_thread_sched;

  Router *_next_router;
  
  Router(const Router &);
  Router &operator=(const Router &);
  
  void remove_hookup(int);
  void hookup_error(const Hookup &, bool, const char *, ErrorHandler *);
  int check_hookup_elements(ErrorHandler *);
  void notify_hookup_range();
  void check_hookup_range(ErrorHandler *);
  void check_hookup_completeness(ErrorHandler *);  
  
  int processing_error(const Hookup&, const Hookup&, bool, int, ErrorHandler*);
  int check_push_and_pull(ErrorHandler *);
  
  void make_pidxes();
  int ninput_pidx() const			{ return _input_eidx.size(); }
  int noutput_pidx() const			{ return _output_eidx.size(); }
  REMOVABLE_INLINE int input_pidx(const Hookup &) const;
  REMOVABLE_INLINE int input_pidx_element(int) const;
  REMOVABLE_INLINE int input_pidx_port(int) const;
  REMOVABLE_INLINE int output_pidx(const Hookup &) const;
  REMOVABLE_INLINE int output_pidx_element(int) const;
  REMOVABLE_INLINE int output_pidx_port(int) const;
  void make_hookpidxes();
  
  void set_connections();
  
  String context_message(int element_no, const char *) const;
  int element_lerror(ErrorHandler *, Element *, const char *, ...) const;

  // private handler methods
  void initialize_handlers(bool, bool);
  int find_ehandler(int, const String &) const;
  static REMOVABLE_INLINE Handler fetch_handler(const Element *, const String &);
  void store_local_handler(int, const Handler &);
  static void store_global_handler(const Handler &);
  static REMOVABLE_INLINE void store_handler(const Element *, const Handler &);

  int downstream_inputs(Element *, int o, ElementFilter *, Bitvector &);
  int upstream_outputs(Element *, int i, ElementFilter *, Bitvector &);

  // global handlers
  static String router_read_handler(Element *, void *);

  friend class Master;
  friend class Task;
  
};


class Router::Handler {
 public:
  
  enum { DRIVER_FLAG_0 = 1,
         DRIVER_FLAG_1 = 2,
         DRIVER_FLAG_2 = 4,
         DRIVER_FLAG_3 = 8,
         USER_FLAG_SHIFT = 4,
         USER_FLAG_0 = 1 << USER_FLAG_SHIFT };
  
  const String &name() const	{ return _name; }
  uint32_t flags() const	{ return _flags; }
  
  bool readable() const		{ return _read; }
  bool read_visible() const	{ return _read; }
  bool writable() const		{ return _write; }
  bool write_visible() const	{ return _write; }
  bool visible() const		{ return read_visible() || write_visible(); }

  String call_read(Element *) const;
  int call_write(const String &, Element *, ErrorHandler *) const;
  
  String unparse_name(Element *) const;
  static String unparse_name(Element *, const String &);

 private:
  
  String _name;
  ReadHandler _read;
  void *_read_thunk;
  WriteHandler _write;
  void *_write_thunk;
  uint32_t _flags;
  int _use_count;
  int _next_by_name;

  Handler();
  Handler(const String &);

  bool compatible(const Handler &) const;
  
  friend class Router;
  
};

/* The largest size a write handler is allowed to have. */
#define LARGEST_HANDLER_WRITE 65536

#endif
