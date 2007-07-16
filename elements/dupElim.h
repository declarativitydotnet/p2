// -*- c-basic-offset: 2; related-file-name: "dupElim.C" -*-
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
 * DESCRIPTION: Store incoming tuples in a table.  Previously unseen
 * tuples go out the output.
 */

#ifndef __DUPELIM_H__
#define __DUPELIM_H__

#include "element.h"
#include "elementRegistry.h"
#include "set"

class DupElim : public Element { 
public:
  DupElim(string);

  DupElim(TuplePtr args);

  /** Overridden to perform the tranformation. */
  TuplePtr simple_action(TuplePtr p);

  const char *class_name() const		{ return "DupElim";}
  const char *processing() const		{ return "a/a"; }
  const char *flow_code() const			{ return "x/x"; }


  DECLARE_PUBLIC_ELEMENT_INITS

private:
  /** My tupleref comparison function for the set */
  struct tuplePtrCompare
  {
    bool operator()(const TuplePtr first, const TuplePtr second) const
    {
      return first->compareTo(second) < 0;
    }
  };
  
  /** My hash table. */
  std::set< TuplePtr, tuplePtrCompare > _table;

  DECLARE_PRIVATE_ELEMENT_INITS
};


#endif /* __DUPELIM_H_ */
