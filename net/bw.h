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

#ifndef __Bandwidth_H__
#define __Bandwidth_H__

// #include <sys/time.h>
#include <winsock2.h> // for time stuff
#include "element.h"

class Bandwidth : public Element {
public:
  Bandwidth(string name = "Bandwidth");


  const char*
  class_name() const {return "Bandwidth";};


  const char*
  processing() const {return "a/a";};


  const char*
  flow_code() const {return "x/x";};


  TuplePtr
  simple_action(TuplePtr p);	// Adds the next sequence num to tuple stream. 


  operator double() {return bw_;};


  void
  setMarkup(std::string m = "unspecified");

private:
  REMOVABLE_INLINE time_t
  now_s() const;


  time_t prev_t_;


  unsigned int bytes_;


  double bw_;

  
  std::string mMarkup_;
};

#endif /* __Bandwidth_H_ */
