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

#ifndef __SKR_H__
#define __SKR_H__

#include <vector>
#include "element.h"

class Route;

class SimpleKeyRouter : public Element {
public:

  SimpleKeyRouter(string name, ValuePtr id, bool retry=false);
  SimpleKeyRouter(TuplePtr args);

  const char *class_name() const	{ return "SimpleKeyRouter";};
  const char *processing() const	{ return "hh/hh"; };
  const char *flow_code() const		{ return "-/-"; };

  int push(int port, TuplePtr tp, b_cbv cb);

  void route(ValuePtr key, ValuePtr loc);

  boost::shared_ptr< std::vector< ValuePtr > > neighbors();
  boost::shared_ptr< std::vector< ValuePtr > > routes();

private:
  REMOVABLE_INLINE int greedyRoute(TuplePtr tp);
  REMOVABLE_INLINE TuplePtr tagRoute(TuplePtr tp, uint r);
  REMOVABLE_INLINE TuplePtr untagRoute(TuplePtr tp);
  REMOVABLE_INLINE int getRoute(TuplePtr tp);
  REMOVABLE_INLINE ValuePtr getKey(TuplePtr tp);

  ValuePtr my_id_;
  bool     retry_;
  std::vector<Route*> routes_;
};

#endif /* __SimpleNetSim_H_ */
