// -*- c-basic-offset: 2; related-file-name: "router.C" -*-
/*
 * @(#)$Id$
 * Loosely inspired from the Click Router class, by Eddie Kohler
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
#include <vec.h>
#include <master.h>

class Router {
public:
  
  /** An auxilliary structure to help pass around element connection
      specifications */
  struct Hookup {
    /** The element from which this hookup originates */
    ElementRef fromElement;
    
    /** The port number at the fromElement */
    int fromPortNumber;

    /**  The element to which this hookup goes */
    ElementRef toElement;

    /** The port number at the toElement */
    int toPortNumber;

    Hookup(ElementRef fe, int fp,
           ElementRef te, int tp)
      : fromElement(fe), fromPortNumber(fp),
        toElement(te), toPortNumber(tp) {};
  };

  typedef ref< Hookup > HookupRef;

  struct Configuration {
    /** The elements */
    ref< vec< ElementRef > > elements;

    /** The hookups */
    ref< vec< HookupRef > > hookups;

    Configuration(ref< vec< ElementRef > > e,
                  ref< vec< HookupRef > > h)
      : elements(e), hookups(h) {};
  };
  typedef ref< Configuration > ConfigurationRef;

  /** Create a new router given a configuration of constructed but not
      necessarily configured elements. */
  Router(ConfigurationRef configuration,
         MasterRef master);

  ~Router();

  // INITIALIZATION
  
  /** Initialize the engine from the configuration */
  int initialize();

  /** Start the router */
  void activate();





  static void static_initialize();
  static void static_cleanup();

  /** Router state */
  enum { ROUTER_NEW,
         ROUTER_PRECONFIGURE,
         ROUTER_PREINITIALIZE,
         ROUTER_LIVE,
         ROUTER_DEAD };

  bool initialized() const			{ return _state == ROUTER_LIVE; }

  // ELEMENTS
  int nelements() const				{ return _elements.size(); }
  const vec< ElementRef > &elements() const{ return _elements; }

  // MASTER scheduler
  MasterRef master() const			{ return _master; }
  enum { RUNNING_DEAD = -2,
         RUNNING_INACTIVE = -1,
         RUNNING_PAUSED = 0,
         RUNNING_BACKGROUND = 1,
         RUNNING_ACTIVE = 2 };
  
private:

  /** The scheduler */
  MasterRef _master;
    
  vec< ElementRef > _elements;

  /** The router state */
  int _state;

  /** The configuration spec of the router */
  ConfigurationRef _configuration;

  /** Are the configuration hookups refering to existing elements and
      ports? */
  int check_hookup_elements();

  friend class Master;
  friend class Task;
};

/** A handy dandy type for router references */
typedef ref< Router > RouterRef;

#endif
