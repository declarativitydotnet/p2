/*
 * $Id$
 * 
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * Or
 * UC Berkeley EECS Computer Science Division, 387 Soda Hall #1776, 
 * Berkeley, CA,  94707. Attention: P2 Group.
 * 
 * DESCRIPTION: An element that receives P2DL specifications in "script"
 * tuples at its push input, attempts to installs the specifications in
 * the given plumber, and pushes out a "dataflowInstallationResult"
 * result tuple. The I/O tuple schemata are as follows:
 * script(@Localnode, Sendingnode, P2dl) and
 * dataflowInstallationResult(@Localnode, Sendingnode, Result, Message),
 * where Result is 0 for success and -1 for failure. All other variables
 * are strings. Localnode and Sendingnode must have the format
 * "hostname:port". P2dl is a P2DL script as a string.
 */

#ifndef __DATAFLOWINSTALLER_H__
#define __DATAFLOWINSTALLER_H__

#include <Python.h>
#include <boost/python.hpp>
#include "element.h"
#include "plumber.h"

class DataflowInstaller : public Element { 
public:
  
  DataflowInstaller(string, PlumberPtr, 
                    boost::python::api::object = boost::python::api::object());

  const char *class_name() const		{ return "DataflowInstaller";}
  const char *processing() const		{ return "h/h"; }
  const char *flow_code() const			{ return "-/-"; }

  /** Overridden since I have no outputs */
  int push(int port, TuplePtr, b_cbv cb);

private:
  int install(string, ostringstream&);
  string readScript( string fileName );

  PlumberPtr                 plumber_;
  boost::python::api::object parser_;
};


#endif /* __DATAFLOWINSTALLER_H_ */
