/*
 * @(#)$Id: 
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * Or
 * UC Berkeley EECS Computer Science Division, 387 Soda Hall #1776, 
 * Berkeley, CA,  94707. Attention: P2 Group.
 * 
 * DESCRIPTION: P2's concrete type system: ring identifier.
 *
 */

#ifndef __TESTERR_H__
#define __TESTERR_H__

#include "reporting.h"

#define FAIL TELL_ERROR << __FILE__ << ":" << __LINE__ << ": **"
#define TIMING TELL_INFO << "Time: "

#endif
