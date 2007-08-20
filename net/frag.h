// -*- c-basic-offset: 2; related-file-name: "element.C" -*-
/*
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * Or
 * UC Berkeley EECS Computer Science Division, 387 Soda Hall #1776, 
 * Berkeley, CA,  94707. Attention: P2 Group.
 * 
 */

#ifndef __Frag_H__
#define __Frag_H__

#include <deque>
#include "tuple.h"
#include "element.h"
#include "elementRegistry.h"

class Frag : public Element { 
public:

  Frag(string name="fragment", unsigned bs=1024, unsigned mqs=1000);
  Frag(TuplePtr args);
  const char *class_name() const { return "Frag";};
  const char *processing() const { return PUSH_TO_PULL; };
  const char *flow_code()  const { return "-/-"; };

  int push(int port, TuplePtr t, b_cbv cb);

  TuplePtr pull(int port, b_cbv cb);

  DECLARE_PUBLIC_ELEMENT_INITS

 private:
  void fragment(TuplePtr t);

  b_cbv _push_cb;
  b_cbv _pull_cb;

  const unsigned block_size_;
  const unsigned max_queue_size_;
  
  std::deque <TuplePtr> fragments_;

  DECLARE_PRIVATE_ELEMENT_INITS
};

#endif /* __Frag_H_ */
