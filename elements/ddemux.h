// -*- c-basic-offset: 2; related-file-name: "ddemux.C" -*-
/*
 * This file is distributed under the terms in the attached INTEL-LICENSE file.
 * If you do not find these files, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * Or
 * UC Berkeley EECS Computer Science Division, 387 Soda Hall #1776, 
 * Berkeley, CA,  94707. Attention: P2 Group.
 * 
 * DESCRIPTION: A dynamic demultiplexing element.  It checks tuples' input field
 * (the first by default) for equality with the registered key for its
 * outputs and routes tuples accordingly, including the demux key.  It
 * pushes back its input only if all of its outputs are backed up (the
 * block-all-block-all policy).  If two or more demux keys are equal,
 * then this thing bombs out (assertion); for duplication an explicit
 * duplicator element should be used.  The element contains one extra
 * output (on port 0) for the "else" case, i.e., for input that matches no key.
 */

#ifndef __DDEMUX_H__
#define __DDEMUX_H__

#include <map>
#include <vector>
#include "element.h"
#include "elementRegistry.h"

class DDemux : public Element { 
public:
  
  DDemux(string, std::vector< ValuePtr >, unsigned f = 0);

  DDemux(TuplePtr args);

  int push(int port, TuplePtr t, b_cbv cb);

  const char *class_name() const		{ return "DDemux";}
  const char *processing() const		{ return "h/h"; }
  const char *flow_code() const			{ return "-/-"; }


  /** Override Element::toDot to show names of outputs */
  void
  toDot(std::ostream*);


  /** A tuple may be dropped without notification if it resolves to an
      output that's held back.  Push back only if all outputs have
      pushed back. */
  int
  push(TuplePtr p, b_cbv cb) const;


  /** Use the port map to find an output by name */
  int
  output(ValuePtr key); 


  DECLARE_PUBLIC_ELEMENT_INITS

private:
  /** The type of port name to port number map */
  typedef std::map<ValuePtr, unsigned, Value::Comparator> PortMap;

  /** My port map */
  PortMap _port_map;

  /** Add output port keyed off ValuePtr argument, returns allocated port # */
  unsigned add_output(ValuePtr);

  /** Remove output port, by port # or key */
  int remove_output(unsigned);
  int remove_output(ValuePtr);


  void remove_block_flag(unsigned port);

  /** The callback for my input */
  b_cbv	_push_cb;

  /** My block flags, one per output port */
  std::vector<bool> _block_flags;

  /** My block flag count. */
  unsigned _block_flag_count;

  /** My block callback function for a given output */
  void unblock(unsigned output);

  /** The input field on which I perform the demultiplexing */
  unsigned _inputFieldNo;

  DECLARE_PRIVATE_ELEMENT_INITS
};


#endif /* __DDEMUX_H_ */

