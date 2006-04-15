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

#ifndef __RCCR_H__
#define __RCCR_H__

#include <deque>
#include <vector>
#include "tuple.h"
#include "element.h"
#include "inlines.h"

typedef uint64_t SeqNum;

class TupleInfo;
class LossRec;

class RateCCR : public Element {
public:
  RateCCR(string name, int dest=0, int src=1, int seq=2, int rtt=3, int ts=4);
  const char *class_name() const { return "RateCCR";};
  const char *processing() const { return "a/al"; };
  const char *flow_code()  const { return "-/--"; };

  TuplePtr simple_action(TuplePtr p);

  TuplePtr pull(int port, b_cbv cb);

  int push(int port, TuplePtr tp, b_cbv cb);	// Rate control input

private:
  class Connection;
  REMOVABLE_INLINE TuplePtr strip(TuplePtr p);

  b_cbv  _ack_cb;	// Indicates when it is once again acceptable to send acks
  int    dest_field_;	// Tuple location of the destination address
  int    src_field_;	// Tuple location of the source address
  int    seq_field_;	// Tuple location of the sequence number
  int    rtt_field_;	// Tuple location of the round trip time
  int    ts_field_;	// Tuple location of the timestamp

  std::deque <TuplePtr>           ack_q_;	// Output ack queue
  std::map <string, Connection*>  cmap_;	// Interval weights
};
  
#endif /* __RCCR_H_ */
