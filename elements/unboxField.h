// -*- c-basic-offset: 2; related-file-name: "unmarshalField.C" -*-
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
 * DESCRIPTION: Element that unmarshals a single field in-place within a
 * tuple.  The field in question must be of type OPAQUE.
 */

#ifndef __UNBOXFIELD_H__
#define __UNBOXFIELD_H__

#include "element.h"
#include "elementRegistry.h"

class UnboxField : public Element { 
public:
  UnboxField(string, unsigned);
  UnboxField(TuplePtr args);

  ~UnboxField();
  
  TuplePtr simple_action(TuplePtr p);

  const char *class_name() const		{ return "UnboxField";}
  const char *processing() const		{ return "a/a"; }
  const char *flow_code() const			{ return "x/x"; }


  DECLARE_PUBLIC_ELEMENT_INITS

private:
  /** The field number I'm unmarshalling */
  unsigned _fieldNo;

  DECLARE_PRIVATE_ELEMENT_INITS
};


#endif /* __UNMARSHALFIELD_H_ */
