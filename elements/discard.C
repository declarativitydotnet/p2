// -*- c-basic-offset: 2; related-file-name: "discard.h" -*-
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
 * DESCRIPTION: Element discarding all tuples pushed into it
 */

#include "discard.h"

DEFINE_ELEMENT_INITS(Discard, "Discard");

Discard::Discard(string name) :
  Element(name, 1,0)
{
}

Discard::Discard(TuplePtr args) :
  Element((*args)[2]->toString(), 1,0)
{
}

int Discard::push(int port, TuplePtr, b_cbv cb)
{
  // Send as many more tuples as you want
  return 1;
}
