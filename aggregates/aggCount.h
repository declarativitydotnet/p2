/*
 * @(#)$Id$
 *
 * Copyright (c) 2005 Intel Corporation. All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE file.
 * If you do not find these files, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * 
 * DESCRIPTION: Simple COUNT aggregate object. It counts matching
 * tuples.
 *
 */

#ifndef __AGGCOUNT_H__
#define __AGGCOUNT_H__

#include "commonTable.h"
#include "value.h"
#include "aggFactory.h"

class AggCount :
  public CommonTable::AggFunc
{
public:
  AggCount();
  
  
  virtual ~AggCount();

  
  void
  reset();
  
  
  void
  first(ValuePtr);
  

  void
  process(ValuePtr);
  
  
  ValuePtr
  result();


  // This is necessary for the class to register itself with the
  // factory.
  DECLARE_PUBLIC_INITS(AggCount)


private:
  /** The current count value for this aggregate */
  uint64_t _current;


  // This is necessary for the class to register itself with the
  // factory.
  DECLARE_PRIVATE_INITS


};


#endif // AGGCOUNT_H
