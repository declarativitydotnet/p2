// -*- c-basic-offset: 2; related-file-name: "unmarshal.C" -*-
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
 * DESCRIPTION: Element that unmarshals the input tuple's first field
 * (which had better be a string) into an output tuple.
 */

#ifndef __UNMARSHAL_H__
#define __UNMARSHAL_H__

#include "element.h"
#include "elementRegistry.h"
#include <bitset>

class Unmarshal : public Element { 
public:
  Unmarshal(string);
  Unmarshal(TuplePtr args);

  ~Unmarshal();
  
  /** Overridden to perform the projecting. */
  TuplePtr simple_action(TuplePtr p);

  const char *class_name() const		{ return "Unmarshal";}
  const char *processing() const		{ return "a/a"; }
  const char *flow_code() const			{ return "x/x"; }


  DECLARE_PUBLIC_ELEMENT_INITS

private:

  DECLARE_PRIVATE_ELEMENT_INITS
};


#endif /* __UNMARSHAL_H_ */
