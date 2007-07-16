// -*- c-basic-offset: 2; related-file-name: "slot.C" -*-
/*
 * @(#)$Id$
 * 
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * Or
 * UC Berkeley EECS Computer Science Division, 387 Soda Hall #1776, 
 * Berkeley, CA,  94707. Attention: P2 Group.
 * 
 * DESCRIPTION: Single-tuple queue element for P2 (basically, an
 * explicit scheduling point in the dataflow graph)
 */

#ifndef __SLOT_H__
#define __SLOT_H__

#include "element.h"
#include "elementRegistry.h"

class Slot : public Element { 
public:
  
  Slot(string name);
  Slot(TuplePtr args);

  int push(int port, TuplePtr t, b_cbv cb);
  TuplePtr pull(int port, b_cbv);
  const char *class_name() const		{ return "Slot";}
  const char *processing() const		{ return PUSH_TO_PULL; }
  const char *flow_code() const			{ return "-/-"; }

  DECLARE_PUBLIC_ELEMENT_INITS

private:
  TuplePtr _t;
  b_cbv	_push_cb;
  b_cbv	_pull_cb;

  DECLARE_PRIVATE_ELEMENT_INITS
};


#endif /* __SLOT_H_ */
