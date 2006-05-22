/* 
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * Or
 * UC Berkeley EECS Computer Science Division, 387 Soda Hall #1776, 
 * Berkeley, CA,  94707. Attention: P2 Group.
 */

#ifndef __TESTTABLE2_H__
#define __TESTTABLE2_H__

#include "boost/test/unit_test.hpp"

class testTable2
{
public:
  testTable2()
  {
  }

  /** Create table, destroy table, with different primary keys, tuple
      lifetimes, size limits. */
  void
  testCreateDestroy();
};


class testTable2_testSuite
  : public boost::unit_test_framework::test_suite
{
public:
  testTable2_testSuite();
};


#endif
