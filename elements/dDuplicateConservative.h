// -*- c-basic-offset: 2; related-file-name: "duplicateConservative.C" -*-
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
 * DESCRIPTION: A duplicating push element.  It take tuples from its
 * single input and duplicateConservatives them on all of its outputs.  It pushes
 * back its input if any of its outputs are backed up (the
 * block-one-block-all policy).
 */

#ifndef __DDUPLICATECONSERVATIVE_H__
#define __DDUPLICATECONSERVATIVE_H__

#include "element.h"
#include "elementRegistry.h"

class DDuplicateConservative : public Element { 
public:
  
  DDuplicateConservative(string, int);

  DDuplicateConservative(TuplePtr args);

  int push(int port, TuplePtr t, b_cbv cb);

  const char *class_name() const		{ return "DDuplicateConservative";}
  const char *processing() const		{ return noutputs() > 0 ? "h/h" : "h/"; }
  const char *flow_code() const			{ return noutputs() > 0 ? "x/x" : "-/"; }

  /** Push back only if all outputs have pushed back. */
  int push(TuplePtr p, b_cbv cb) const;

  /** Handle output addition */
  unsigned add_output();

  /** Handle output removal */
  int remove_output(unsigned);

  DECLARE_PUBLIC_ELEMENT_INITS

private:
  /** The callback for my input */
  b_cbv _push_cb;

  /** My block flags, one per output port */
  std::vector<int> _block_flags;

  /** My block flag count. */
  unsigned _block_flag_count;

  /** My block callback function for a given output */
  void unblock(unsigned output);

  DECLARE_PRIVATE_ELEMENT_INITS
};


#endif /* __DDUPLICATECONSERVATIVE_H_ */
