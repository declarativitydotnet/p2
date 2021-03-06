/* 
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * Or
 * UC Berkeley EECS Computer Science Division, 387 Soda Hall #1776, 
 * Berkeley, CA,  94707. Attention: P2 Group.
 */

#include "boost/test/unit_test.hpp"

#include <sstream>

#include "value.h"
#include "val_null.h"
#include "val_str.h"
#include "val_int64.h"
#include "val_double.h"
#include "val_opaque.h"
#include "val_tuple.h"
#include "val_time.h"
#include "val_id.h"
#include "val_list.h"

#include "testLists.h"

class testLists
{
public:
   testLists()
   {
   }
   
   
public:

void testListSize(List theList, uint32_t expected) 
{
   std::string testID;
   {
      std::ostringstream ID;
      ID << "List size test.";
      testID = ID.str();
   }
   
   uint32_t listsize = theList.size();
   
   std::ostringstream message;
   message << testID
           << "The size of the list " << theList.toString() 
           << " was reported incorrectly as " << listsize;
   
   BOOST_CHECK_MESSAGE(listsize == expected, 
                        message.str().c_str());
}

#define TEST_LISTAT(list, valTypeExt, value, pos) \
  { \
    std::string testID; \
    { \
      std::ostringstream ID; \
      ID << "List containment test. "; \
      testID = ID.str(); \
    } \
 \
    ValuePtr v = Val_##valTypeExt::mk(value); \
    ValuePtr  memberAtPos = (list).at(pos); \
    std::ostringstream message;                 \
    message << testID                                          \
            << "Checking whether " #value   \
            << " is present at " << pos << " position of " << (list).toString() \
            << " returned an unexpected result (" \
            << v->toString() \
            << ")."; \
    BOOST_CHECK_MESSAGE(memberAtPos->compareTo(v) == 0,                            \
                        message.str().c_str());                       \
  }

#define TEST_LISTMEMBER(list, valTypeExt, value, result) \
  { \
    std::string testID; \
    { \
      std::ostringstream ID; \
      ID << "Value casting test. "; \
      testID = ID.str(); \
    } \
 \
    ValuePtr v = Val_##valTypeExt::mk(value); \
    bool success; \
    if((list).member(v) == 1) { \
      success = true;\
    } else {\
      success = false;\
    }\
    std::ostringstream message;                 \
    message << testID                                          \
            << "Checking whether " #value   \
            << " is a member of " << (list).toString() \
            << " returned an unexpected result (" \
            << success \
            << ")."; \
    BOOST_CHECK_MESSAGE(success == result,                            \
                        message.str().c_str());                       \
  }

void testListIntersect(ListPtr list1, ListPtr list2, ListPtr expected)
{
   std::string testID;
   {
      std::ostringstream ID;
      ID << "List intersection test.";
      testID = ID.str();
   }
   
   ListPtr result = list1->intersect(list2);
   
   std::ostringstream message;
   message << testID
           << "The intersection of lists " << list1->toString() 
           << " and " << list2->toString() 
           << " should have been " << expected->toString() 
           << " but was " << result->toString();
           
   BOOST_CHECK_MESSAGE(result->compareTo(expected) == 0, 
                        message.str().c_str());
}

void testListMSIntersect(ListPtr list1, ListPtr list2, ListPtr expected)
{
   std::string testID;
   {
      std::ostringstream ID;
      ID << "List intersection test.";
      testID = ID.str();
   }
   
   ListPtr result = list1->multiset_intersect(list2);
   
   std::ostringstream message;
   message << testID
           << "The intersection of lists " << list1->toString() 
           << " and " << list2->toString() 
           << " should have been " << expected->toString() 
           << " but was " << result->toString();
           
   BOOST_CHECK_MESSAGE(result->compareTo(expected) == 0, 
                        message.str().c_str());
}


void testListConcat(ListPtr list1, ListPtr list2)
{
   std::string testID;
   {
      std::ostringstream ID;
      ID << "List intersection test.";
      testID = ID.str();
   }
   
   ListPtr test = List::mk();
      
   ValPtrList::const_iterator iter = list1->begin();

   ValPtrList::const_iterator end = list1->end();
   
   while(iter != end) {
      test->append(*iter);
      iter++;
   }
      

   iter = list2->begin();
   end = list2->end();
   
   while(iter != end) {
      test->append(*iter);
      iter++;
   }
   
   
   ListPtr result = list1->concat(list2);
   
   std::ostringstream message;
   message << testID
           << "The concatenation of lists " << list1->toString() 
           << " and " << list2->toString() 
           << " should have been " << test->toString() 
           << " but was " << result->toString();
   
      BOOST_CHECK_MESSAGE(result->compareTo(test) == 0, 
                        message.str().c_str());
}

void testListCompare(ListPtr L1, ListPtr L2, int expected) {
   std::string testID;
   {
      std::ostringstream ID;
      ID << "List comparison test.";
      testID = ID.str();
   }
   
   string expectedString;
   string resultString;
      
   string EQ = "equal";
   string LT = "less than";
   string GT = "greater than";
   
   int result = L1->compareTo(L2);
   
   if(result == 0) {
      resultString = EQ;
   } 
   else if(result > 0) {
      resultString = GT;
      result = 1;
   } 
   else {
      resultString = LT;
      result = -1;
   }
   
   if(expected == 0) {
      expectedString = EQ; 
   }
   else if(expected > 0) {
      expectedString = GT;
      expected = 1;
   } 
   else {
      expectedString = LT;
      expected = -1;
   }
   
   std::ostringstream message;
   
   message << testID
           << "The comparison of lists " << L1->toString() 
           << " and " << L1->toString() 
           << " should have been " << expectedString
           << " but was " << resultString;
   
      BOOST_CHECK_MESSAGE(result - expected == 0, 
                        message.str().c_str());
}

// ==================
// Intersection tests
// ==================

void testIntersection()
{
   // Make some sample lists.
   ListPtr test1 = List::mk();
   ListPtr test2 = List::mk();
   ListPtr test3 = List::mk();
   ListPtr empty = List::mk();
   
   // Populate the lists with values (which should henceforth retain 
   // their ordering.
   test1->append(Val_Int64::mk(42));
   test1->append(Val_Int64::mk(17));
   test1->append(Val_Int64::mk(570));
   test1->append(Val_Int64::mk(-12));
   test1->append(Val_Int64::mk(0));
   test1->append(Val_Int64::mk(-450));
   
   test2->append(Val_Int64::mk(42));
   test2->append(Val_Int64::mk(97));
   test2->append(Val_Int64::mk(-12));
   test2->append(Val_Int64::mk(-5));   
   test2->append(Val_Int64::mk(570));

   test3->append(Val_Str::mk("garply"));
   test3->append(Val_Str::mk("bar"));
   test3->append(Val_Str::mk("baz"));
   test3->append(Val_Str::mk("quux"));
   test3->append(Val_Str::mk("quuuux"));

   // Test list sizes
   testListSize(*test1, 6);
   testListSize(*test2, 5);
   testListSize(*test3, 5);
   testListSize(*empty, 0);

   ListPtr intersectList = List::mk();
   intersectList->append(Val_Int64::mk(-12));
   intersectList->append(Val_Int64::mk(42));
   intersectList->append(Val_Int64::mk(570));

   testListIntersect(test1, test2, intersectList);
   
   ListPtr test1sorted = List::mk();
   
   test1sorted->append(Val_Int64::mk(-450));
   test1sorted->append(Val_Int64::mk(-12));
   test1sorted->append(Val_Int64::mk(0));
   test1sorted->append(Val_Int64::mk(17));
   test1sorted->append(Val_Int64::mk(42));
   test1sorted->append(Val_Int64::mk(570));
   
   testListIntersect(test1, empty, empty);
   testListIntersect(test1, test3, empty);
   testListIntersect(test1, test1, test1sorted);
   testListMSIntersect(test1, test1, test1);
}

// ================
// Membership tests
// ================

void testMembership()
{
   // Make some sample lists.
   ListPtr test1 = List::mk();
   ListPtr test2 = List::mk();
   ListPtr test3 = List::mk();
   ListPtr empty = List::mk();
   
   // Populate the lists with values (which should henceforth retain 
   // their ordering.
   test1->append(Val_Int64::mk(42));
   test1->append(Val_Int64::mk(17));
   test1->append(Val_Int64::mk(570));
   test1->append(Val_Int64::mk(-12));
   test1->append(Val_Int64::mk(0));
   test1->append(Val_Int64::mk(-450));
   
   test2->append(Val_Int64::mk(42));
   test2->append(Val_Int64::mk(97));
   test2->append(Val_Int64::mk(-12));
   test2->append(Val_Int64::mk(-5));   
   test2->append(Val_Int64::mk(570));

   test3->append(Val_Str::mk("garply"));
   test3->append(Val_Str::mk("bar"));
   test3->append(Val_Str::mk("baz"));
   test3->append(Val_Str::mk("quux"));
   test3->append(Val_Str::mk("quuuux"));

   // Test list sizes
   testListSize(*test1, 6);
   testListSize(*test2, 5);
   testListSize(*test3, 5);
   testListSize(*empty, 0);
   
      // Membership tests
   TEST_LISTMEMBER(*test3, Str, "garply", true);
   TEST_LISTMEMBER(*test3, Str, "nothere", false);
   TEST_LISTMEMBER(*test3, Double, 4.2, false);
   TEST_LISTMEMBER(*test3, Int64, 75, false);
   TEST_LISTMEMBER(*test2, Int64, 75, false);
   TEST_LISTMEMBER(*test1, Int64, -12, true);
   TEST_LISTMEMBER(*test1, Str, "NotANumber", false);

   TEST_LISTAT(*test3, Str, "garply", 0); // start counting at 0
   TEST_LISTAT(*test1, Int64, -12, 3);


}

// =============
// Concat tests
// =============

void testConcat()
{
   // Make some sample lists.
   ListPtr test1 = List::mk();
   ListPtr test2 = List::mk();
   ListPtr test3 = List::mk();
   ListPtr empty = List::mk();
   
   // Populate the lists with values (which should henceforth retain 
   // their ordering.
   test1->append(Val_Int64::mk(42));
   test1->append(Val_Int64::mk(17));
   test1->append(Val_Int64::mk(570));
   test1->append(Val_Int64::mk(-12));
   test1->append(Val_Int64::mk(0));
   test1->append(Val_Int64::mk(-450));
   
   test2->append(Val_Int64::mk(42));
   test2->append(Val_Int64::mk(97));
   test2->append(Val_Int64::mk(-12));
   test2->append(Val_Int64::mk(-5));   
   test2->append(Val_Int64::mk(570));

   test3->append(Val_Str::mk("garply"));
   test3->append(Val_Str::mk("bar"));
   test3->append(Val_Str::mk("baz"));
   test3->append(Val_Str::mk("quux"));
   test3->append(Val_Str::mk("quuuux"));
   
   testListConcat(test1, test2);
   testListConcat(test2, test3);
   testListConcat(test2, empty);
   testListConcat(test3, test2);
   testListConcat(test1, empty);
   testListConcat(empty, empty);
   testListConcat(test1, test1);
}

void testCompare() {
   // (1,2,3)
   ListPtr test1 = List::mk();
   test1->append(Val_Int64::mk(1));
   test1->append(Val_Int64::mk(2));
   test1->append(Val_Int64::mk(3));
   // (2,3,4)
   ListPtr test2 = List::mk();
   test2->append(Val_Int64::mk(2));
   test2->append(Val_Int64::mk(3));
   test2->append(Val_Int64::mk(4));
   // (5)
   ListPtr test3 = List::mk();
   test3->append(Val_Int64::mk(5));
   // (1,2,3,4)
   ListPtr test4 = List::mk();
   test4->append(Val_Int64::mk(1));
   test4->append(Val_Int64::mk(2));
   test4->append(Val_Int64::mk(3));
   test4->append(Val_Int64::mk(4));
   
   testListCompare(test1, test1, 0);
   
   testListCompare(test1, test2, -1);
   testListCompare(test2, test1, 1);
   
   testListCompare(test1, test4, -1);
   testListCompare(test4, test1, 1);
   
   testListCompare(test3, test2, 1);
   testListCompare(test2, test3, -1);
   
   testListCompare(test4, test2, -1);
   testListCompare(test2, test4, 1);
}

};

#undef TEST_LISTMEMBER

testLists_testSuite::testLists_testSuite() 
  : boost::unit_test_framework::test_suite("testLists")
{
  boost::shared_ptr<testLists> instance(new testLists());
  add(BOOST_CLASS_TEST_CASE(&testLists::testIntersection, instance));
  add(BOOST_CLASS_TEST_CASE(&testLists::testMembership, instance));
  add(BOOST_CLASS_TEST_CASE(&testLists::testConcat, instance));
  add(BOOST_CLASS_TEST_CASE(&testLists::testCompare, instance));
}
