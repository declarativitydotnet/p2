// -*- c-basic-offset: 2; related-file-name: "lookup2.C" -*-
/*
 * @(#)$Id$
 * 
 * This file is distributed under the terms in the attached
 * LICENSE file.  If you do not find this file, copies can be
 * found by writing to: Intel Research Berkeley, 2150 Shattuck Avenue,
 * Suite 1300, Berkeley, CA, 94704.  Attention: Intel License Inquiry.
 * Or
 * UC Berkeley EECS Computer Science Division, 387 Soda Hall #1776, 
 * Berkeley, CA,  94707. Attention: P2 Group.
 *
 * The lookup element for the new table structure.  It has a single
 * input where a lookup tuple arrives; the inputKey determines the key
 * used for this lookup.  The search of the table is performed according
 * to the index (if any) held for the table's lookupKey.  On the output
 * all matches for the lookup are returned with successive pulls.
 * Outputs consist of tuples that contain two embedded tuples, the first
 * holding the lookup tuple and the second holding the result if any.
 * The last output tuple to be returned for a given search is tagged
 * with END_OF_SEARCH.  If a lookup yields no results, an output tuple
 * with the lookup tuple as the first value and an empty tuple as the
 * second value is returned (also tagged with END_OF_SEARCH).
 */

#ifndef __LOOKUP2_H__
#define __LOOKUP2_H__

#include "element.h"
#include "table2.h"
#include "val_tuple.h"
#include "val_null.h"

class Lookup2 : public Element {
public:
  Lookup2(string name,
          Table2Ptr table,
          Table2::Key inputKey,
          Table2::Key lookupKey,
          b_cbv completion_cb = 0);
  ~Lookup2();
  
  
  const char *class_name() const		{ return "Lookup2";}
  const char *processing() const		{ return "h/l"; }
  const char *flow_code() const			{ return "-/-"; }
  

  /** Receive a new lookup tuple */
  int
  push(int port, TuplePtr, b_cbv cb);

  
  /** Return a match to the current lookup */
  TuplePtr
  pull(int port, b_cbv cb);

  
  /** The END_OF_SEARCH tuple tag. */
  static string END_OF_SEARCH;
  



private:
  /** My table */
  Table2Ptr _table;
  

  /** My pusher's callback */
  b_cbv _pushCallback;

  
  /** My puller's callback */
  b_cbv _pullCallback;

  
  /** My completion callback */
  b_cbv _compCallback;

  
  /** My current lookup tuple */
  TuplePtr _lookupTuple;

  
  /** My encapsulated lookup tuple for all results */
  ValuePtr _lookupTupleValue;

  
  /** My current iterator */
  Table2::IteratorPtr _iterator;


  /** My input key */
  Table2::Key _inputKey;
  
  
  /** My index key */
  Table2::Key _indexKey;
};

#endif /* __LOOKUP2_H_ */
