// -*- c-basic-offset: 2; related-file-name: "pingpong.C" -*-
/*
 * @(#)$Id$
 * 
 * This file is distributed under the terms in the attached INTEL-LICENSE file.
 * If you do not find these files, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * 
 * DESCRIPTION: UDP Ping/Pong
 */

#ifndef __PING_H__
#define __PING_H__

#include <amisc.h>
#include <time.h>
#include <map>

#include "element.h"
#include "val_opaque.h"

/* Ping element. Issues pings to designated nodes, wait for replies. 
   Outputs Ping Results with RTT, or -1 (if timeouts after a few retries).
   XXX: The pending list is stored in the ping element until we figure out
   how to detect expirations in storage.
*/

class Ping : public Element {
public:

  Ping(str name, int numRetries, int seconds, double retry_interval);
  int push(int port, TupleRef p, b_cbv cb);     
  int initialize();
  
  const char *class_name() const		{ return "Ping";}
  const char *processing() const		{ return "hh/hh"; }
  const char *flow_code() const		{ return "xx/xx"; }
   
  struct Entry {
    TupleRef p; 
    double lastPingTime;
    long numPings;
    Entry(TupleRef p) : p(p) {};
  };

private:

  /* List of pending pings. 
     For now, store one tuple per destination.
     In future, add the time stamp, number of retries, etc. */
  typedef std::map<str, Entry*> EntryMap;
  typedef std::map<str, TupleRef> TupleMap;
  EntryMap _pendingPings;
  TupleMap _pendingResults;
  EntryMap::iterator _iterator;  
  TupleMap::iterator _tupleIterator;

  void wakeup();
  void runTimer();
  void generatePingSuccess(TupleRef p);
  void generatePingFailure(TupleRef p);

  /** The interval in seconds */
  uint _seconds;

  /** My time callback ID. */
  timeCBHandle * _timeCallback;

  /** My wakeup callback */
  b_cbv _wakeupCB, _runTimerCB;

  double _retry_interval;
  int _numPings;

};
  


#endif /* __PING_H_ */
