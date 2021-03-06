// -*- c-basic-offset: 2; related-file-name: "aggFuncLoader.h" -*-
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
 */

#include "aggFuncLoader.h"

#include "aggMin.h"
#include "aggMkSet.h"
#include "aggMkList.h"
#include "aggSchema.h"
#include "aggMax.h"
#include "aggCount.h"
#include "aggCountDistinct.h"


void
AggFuncLoader::loadAggFunctions()
{
  AggCount::ensureInit();
  AggMin::ensureInit();
  AggMkSet::ensureInit();
  AggMkList::ensureInit();
  AggSchema::ensureInit();
  AggMax::ensureInit();
  AggCountDistinct::ensureInit();
}

