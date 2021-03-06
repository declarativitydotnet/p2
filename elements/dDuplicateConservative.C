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

#include "dDuplicateConservative.h"
#include "val_str.h"
#include "val_int64.h"
#include <boost/bind.hpp>

DEFINE_ELEMENT_INITS(DDuplicateConservative, "DDuplicateConservative");

DDuplicateConservative::DDuplicateConservative(string name, int outputs)
  : Element(name, 1, outputs),
    _push_cb(0),
    _block_flags(),
    _block_flag_count(0)
{
  // Clean out the block flags
  _block_flags.resize(noutputs());
}

/**
 * Generic constructor.
 * Arguments:
 * 2. Val_Str:    Element Name.
 * 3. Val_UInt32: Number of outputs.
 */
DDuplicateConservative::DDuplicateConservative(TuplePtr args)
  : Element(Val_Str::cast((*args)[2]), 1, Val_Int64::cast((*args)[3])),
    _push_cb(0),
    _block_flags(),
    _block_flag_count(0)
{
  // Clean out the block flags
  _block_flags.resize(noutputs());
}

void DDuplicateConservative::unblock(unsigned output)
{
  assert((output >= 0) &&
         (output <= noutputs()));
  
  // Unset a blocked output
  if (_block_flags[output]) {
    ELEM_INFO("unblock output");

    _block_flags[output] = false;
    _block_flag_count--;
    assert(_block_flag_count >= 0);
  }

  // If I have no more blocked outputs, unblock my pusher
  if (_block_flag_count == 0) {
   ELEM_INFO("unblock: propagating aggregate unblock output");
     _push_cb();
    _push_cb = 0;
  }
}

int DDuplicateConservative::push(int port, TuplePtr p, b_cbv cb)
{
  assert(p != 0);
  assert(port == 0);

  // Can I take more?
  if (_block_flag_count > 0) {
    // I'm still blocked
    assert(_push_cb);
    ELEM_WARN("push: Overrun");
    return 0;
  }

  // We're free and clear
  assert(_block_flag_count == 0);
  assert(!_push_cb);

  // For every output
  for (unsigned i = 0;
       i < noutputs();
       i++) {
    // Send it with the appropriate callback
    int result = output(i)->push(p, boost::bind(&DDuplicateConservative::unblock, this, i));

    assert(_block_flags[i] == false);

    // If it can take no more
    if (result == 0) {
      std::cerr << "DUPLICATE CONSERVATIVE BLOCKED FROM ELEMENT " << output(i)->element()->name() << " TUPLE: " << p->toString() << std::endl;
      // update the flags
      _block_flags[i] = true;
      _block_flag_count++;
    }
  }

  // If I just blocked any of my outputs, push back my input
  if (_block_flag_count > 0) {
    _push_cb = cb;
    ELEM_INFO("push: Blocking input");
    return 0;
  } else {
    return 1;
  }
}


unsigned
DDuplicateConservative::add_output()
{
  unsigned port = addOutputPort();

  _block_flags.push_back(false);  
  
  return port;
}

int
DDuplicateConservative::remove_output(unsigned port)
{
  return deleteOutputPort(port);
}
