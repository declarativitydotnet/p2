// -*- c-basic-offset: 2; related-file-name: "oper.C" -*-
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
 * DESCRIPTION: P2's operator system for basic concrete types.
 * 
 * So you'd like to add a new operator... Read on then.
 * The following is a set of steps for adding a new overloaded operator.
 * 1. Define the operator interface method in class Oper. 
 *    1a. The method must be virtual, return a ValuePtr as the operator 
 *        result type, and have 1-3 const ValuePtr reference formals.
 *    1b. The body of the method will contain one of the 3 NOSUP# macros
 *        listed below. Which of the 3 depends on how many ValuePtr formals
 *        are defined by your operator. 
 * 2. Define C++ overload operator interface method.
 *    The C++ overload operator definition is for syntactic sugar only.
 *    If your P2 operator does not have a corresponding C++ operator then
 *    you may skip this step. However, you may wish to define some dispatch
 *    method that you can call, within PEL for instance.
 * 3. Define the body of your C++ overload operator method.
 *    The body of this method must contain a dispatch to your operator interface
 *    method that you defined in step 1. The exact operator implementation called
 *    is dependent on the concrete types of the listed formals. The operator table
 *    (oper_table_) static member variable of class Oper is a jump table
 *    to your operator.  The dispatch simply uses the typeName of the operator
 *    formals to lookup the correct operator method in oper_table_. 
 * 4. Define your new operator in one of two template classes defined below. 
 *    Please see the descriptions of the classes (i.e., OperCompare, OperImpl) below for 
 *    the best match. 
 * Optional:
 * 5. Add the PEL operator that will make use of your new operator definition.
 * 6. Add test cases to pel.C that test your new operator under PEL.
 */

#ifndef __OPER_IMPL_H__
#define __OPER_IMPL_H__

#include "value.h"

namespace opr {
  /**
   * MACROS for throwing operator not supported exceptions. 
   * An operator is not supported if the types of the operands
   * do not override the operator. 
   */
  #define NOSUP1(o, t1) do { \
    ostringstream oss; \
    oss << "Oper("<<string(o)<<") not supported for types "<<string(t1); \
    throw OperException(oss.str()); \
  } while (0)
  
  #define NOSUP2(o, t1, t2) { \
    ostringstream oss; \
    oss << "Oper("<<string(o)<<") not supported for types " \
                <<string(t1)<<", "<<string(t2); \
    throw OperException(oss.str()); \
  }
  
  #define NOSUP3(o, t1, t2, t3) do { \
    ostringstream oss; \
    oss << "Oper("<<string(o)<<") not supported for types " \
                <<string(t1)<<", "<<string(t2)<<", "<<string(t3); \
    throw OperException(oss.str()); \
  } while (0)
  
  class Oper {
  public:
    virtual ~Oper() {};

    /**
     * Operator Type Table
     * This is an NxN matrix of operator functions. Most operators
     * take operate on two operands. The types of those operators are
     * used to index this table, which will hold the operator function
     * of the base type of the operands. The initialization of this table
     * occurs in oper.C, but this be changed to support a different type 
     * conversion lattice.
     * See also value.h for defining new P2 concrete types and how such
     * new types affect the operator table definition.
     */
    static const Oper** oper_table_[Value::TYPES][Value::TYPES];
  
    /**
     * Thrown when operand types do not override an operator function.
     */ 
    class OperException {
    public:
      OperException(string d) : desc_(d) {};
  
      string description() {return desc_;};

      operator string() { return desc_; };
  
      private:
        string desc_;
    };
  
    /**
     * Thrown when a division by zero occurs. Contains the value that
     * was (or was cast to) 0.
     */ 
    class DivisionByZeroException {
    public:
      DivisionByZeroException(ValuePtr v) : value_(v) {};
      
    private:
      ValuePtr value_;
    };
  
    /**
     * OPERATOR FUNCTIONS
     * An operator function is where the functionality of the operator
     * exists. The default operator functions listed below will throw
     * an exception, meaning that the operator is not supported for that
     * type. Each P2 concrete type will define a subclass of Oper and
     * override those operator functions that have definitions for the
     * respective type.
     */
    virtual ValuePtr _bnot (const ValuePtr& v) const
      { NOSUP1("~", v->typeName()); return ValuePtr(); };
    virtual ValuePtr _band (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("&", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _bor (const ValuePtr& v1, const ValuePtr& v2)const
      { NOSUP2("|", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _bxor (const ValuePtr& v1, const ValuePtr& v2)const
      { NOSUP2("^", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _lshift (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("<<", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _rshift (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2(">>", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _plus (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("+", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _minus (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("-", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _times (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("*", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _divide (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("/", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _mod (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("\%", v1->typeName(), v2->typeName()); return ValuePtr(); };
    virtual ValuePtr _dec (const ValuePtr& v1) const
      { NOSUP1("--", v1->typeName()); return ValuePtr(); };
    virtual ValuePtr _inc (const ValuePtr& v1) const
      { NOSUP1("++", v1->typeName()); return ValuePtr(); };
  
    virtual bool _eq (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("==", v1->typeName(), v2->typeName()); return false; };
    virtual bool _neq (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("!=", v1->typeName(), v2->typeName()); return false; };
    virtual bool _gt (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2(">", v1->typeName(), v2->typeName()); return false; };
    virtual bool _gte (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2(">=", v1->typeName(), v2->typeName()); return false; };
    virtual bool _lt (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("<", v1->typeName(), v2->typeName()); return false; };
    virtual bool _lte (const ValuePtr& v1, const ValuePtr& v2) const
      { NOSUP2("<=", v1->typeName(), v2->typeName()); return false; };
  
    virtual bool _inOO (const ValuePtr& v1, const ValuePtr& v2, 
                            const ValuePtr& v3) const { 
      NOSUP3("()", v1->typeName(), v2->typeName(), v3->typeName()); 
      return false; 
    };
    virtual bool _inOC (const ValuePtr& v1, const ValuePtr& v2, 
                            const ValuePtr& v3) const { 
      NOSUP3("(]", v1->typeName(), v2->typeName(), v3->typeName()); 
      return false; 
    };
    virtual bool _inCO (const ValuePtr& v1, const ValuePtr& v2, 
                            const ValuePtr& v3) const { 
      NOSUP3("[)", v1->typeName(), v2->typeName(), v3->typeName()); 
      return false; 
    };
    virtual bool _inCC (const ValuePtr& v1, const ValuePtr& v2, 
                            const ValuePtr& v3) const { 
      NOSUP3("[]", v1->typeName(), v2->typeName(), v3->typeName()); 
      return false; 
    };
  };
   
  /**
   * C++ Operator functions
   * The job of these functions is to simply lookup the base
   * type of the type operands and call the operator function 
   * (defined by the subclass of Oper) defined by the base type. 
   */
  ValuePtr operator<<(const ValuePtr& v1, const ValuePtr& v2);
  ValuePtr operator>>(const ValuePtr& v1, const ValuePtr& v2); 
  ValuePtr operator+ (const ValuePtr& v1, const ValuePtr& v2); 
  struct timespec operator+ (const struct timespec& v1, const struct timespec& v2); 
  ValuePtr operator- (const ValuePtr& v1, const ValuePtr& v2); 
  struct timespec operator- (const struct timespec& v1, const struct timespec& v2); 
  ValuePtr operator--(const ValuePtr& v1); 
  ValuePtr operator++(const ValuePtr& v1); 
  ValuePtr operator* (const ValuePtr& v1, const ValuePtr& v2); 
  ValuePtr operator/ (const ValuePtr& v1, const ValuePtr& v2); 
  ValuePtr operator% (const ValuePtr& v1, const ValuePtr& v2); 
  ValuePtr operator~ (const ValuePtr& v);
  ValuePtr operator& (const ValuePtr& v1, const ValuePtr& v2); 
  ValuePtr operator| (const ValuePtr& v1, const ValuePtr& v2); 
  ValuePtr operator^ (const ValuePtr& v1, const ValuePtr& v2); 
  
  bool     operator==(const ValuePtr& v1, const ValuePtr& v2); 
  bool     operator==(const struct timespec& v1, const struct timespec& v2); 
  bool     operator!=(const ValuePtr& v1, const ValuePtr& v2); 
  bool     operator!=(const struct timespec& v1, const struct timespec& v2); 
  bool     operator< (const ValuePtr& v1, const ValuePtr& v2); 
  bool     operator< (const struct timespec& v1, const struct timespec& v2); 
  bool     operator<=(const ValuePtr& v1, const ValuePtr& v2); 
  bool     operator<=(const struct timespec& v1, const struct timespec& v2); 
  bool     operator> (const ValuePtr& v1, const ValuePtr& v2); 
  bool     operator> (const struct timespec& v1, const struct timespec& v2); 
  bool     operator>=(const ValuePtr& v1, const ValuePtr& v2); 
  bool     operator>=(const struct timespec& v1, const struct timespec& v2); 
  
  bool     inOO(const ValuePtr& v1, const ValuePtr& v2, const ValuePtr& v3);
  bool     inOC(const ValuePtr& v1, const ValuePtr& v2, const ValuePtr& v3);
  bool     inCO(const ValuePtr& v1, const ValuePtr& v2, const ValuePtr& v3);
  bool     inCC(const ValuePtr& v1, const ValuePtr& v2, const ValuePtr& v3);
  
  /**
   * Basic Operator Function Template for comparison based operators.
   * All concrete P2 types define a compareTo method. The
   * OperCompare template will override the comparison operators (==, !=, etc.)
   * of Oper by implementing the respective operator using the compareTo logic.
   */
  template <class T> class OperCompare : public Oper { 
  public: 
    virtual bool _eq(const ValuePtr& v1, const ValuePtr& v2) const {
      ValuePtr c1 = T::mk(T::cast(v1));
      ValuePtr c2 = T::mk(T::cast(v2));
      return c2->compareTo(c1) == 0;
    };
    virtual bool _neq (const ValuePtr& v1, const ValuePtr& v2) const {
      ValuePtr c1 = T::mk(T::cast(v1));
      ValuePtr c2 = T::mk(T::cast(v2));
      return c2->compareTo(c1) != 0;
    };
    virtual bool _gt (const ValuePtr& v1, const ValuePtr& v2) const {
      ValuePtr c1 = T::mk(T::cast(v1));
      ValuePtr c2 = T::mk(T::cast(v2));
      return c2->compareTo(c1) > 0;
    };
    virtual bool _gte (const ValuePtr& v1, const ValuePtr& v2) const {
      ValuePtr c1 = T::mk(T::cast(v1));
      ValuePtr c2 = T::mk(T::cast(v2));
      return c2->compareTo(c1) >= 0;
    };
    virtual bool _lt (const ValuePtr& v1, const ValuePtr& v2) const {
      ValuePtr c1 = T::mk(T::cast(v1));
      ValuePtr c2 = T::mk(T::cast(v2));
      return c2->compareTo(c1) < 0;
    };
    virtual bool _lte (const ValuePtr& v1, const ValuePtr& v2) const {
      ValuePtr c1 = T::mk(T::cast(v1));
      ValuePtr c2 = T::mk(T::cast(v2));
      return c2->compareTo(c1) <= 0;
    };
  
    virtual bool _inOO(const ValuePtr& vc, const ValuePtr& fc,
                       const ValuePtr& tc) const {
      ValuePtr v = T::mk(T::cast(vc));
      ValuePtr f = T::mk(T::cast(fc));
      ValuePtr t = T::mk(T::cast(tc));
      return (((v->compareTo(f) >  0) && (v->compareTo(t) <  0)) ||
              ((t->compareTo(f) <= 0) && (v->compareTo(f) >  0)) ||
              ((v->compareTo(t) <  0) && (t->compareTo(f) <= 0)));
    }
  
    virtual bool _inOC(const ValuePtr& vc, const ValuePtr& fc,
                       const ValuePtr& tc) const {
      ValuePtr v = T::mk(T::cast(vc));
      ValuePtr f = T::mk(T::cast(fc));
      ValuePtr t = T::mk(T::cast(tc));
      return (((v->compareTo(f) >  0) && (v->compareTo(t) <= 0)) ||
              ((t->compareTo(f) <= 0) && (v->compareTo(f) >  0)) ||
              ((v->compareTo(t) <= 0) && (t->compareTo(f) <= 0)));
    }
  
    virtual bool _inCO(const ValuePtr& vc, const ValuePtr& fc,
                       const ValuePtr& tc) const {
      ValuePtr v = T::mk(T::cast(vc));
      ValuePtr f = T::mk(T::cast(fc));
      ValuePtr t = T::mk(T::cast(tc));
      return (((v->compareTo(f) >= 0) && (v->compareTo(t) <  0)) ||
              ((t->compareTo(f) <= 0) && (v->compareTo(f) >= 0)) ||
              ((v->compareTo(t) <  0) && (t->compareTo(f) <= 0)));
    }
  
    virtual bool _inCC(const ValuePtr& vc, const ValuePtr& fc,
                       const ValuePtr& tc) const {
      ValuePtr v = T::mk(T::cast(vc));
      ValuePtr f = T::mk(T::cast(fc));
      ValuePtr t = T::mk(T::cast(tc));
      return (((v->compareTo(f) >= 0) && (v->compareTo(t) <= 0)) ||
              ((t->compareTo(f) <= 0) && (v->compareTo(f) >= 0)) ||
              ((v->compareTo(t) <= 0) && (t->compareTo(f) <= 0)));
    }
  };
  
  /**
   * Basic Operator Function Template.
   * This template provides basic functionality for ALL operator functions
   * defined in Oper. Only the most basic concrete types will be able to
   * make use of this template (e.g., Int32, UInt32, Int64, UInt64, Double).
   */
  template <class T> class OperImpl : public OperCompare<T> { 
  public: 
    virtual ValuePtr _bnot (const ValuePtr& v) const {
      return T::mk(~(T::cast(v)));
    };
    virtual ValuePtr _band (const ValuePtr& v1, const ValuePtr& v2) const {
      return T::mk(T::cast(v1) & T::cast(v2));
    };
    virtual ValuePtr _bor (const ValuePtr& v1, const ValuePtr& v2) const {
      return T::mk(T::cast(v1) | T::cast(v2));
    };
    virtual ValuePtr _bxor (const ValuePtr& v1, const ValuePtr& v2) const {
      return T::mk(T::cast(v1) ^ T::cast(v2));
    };
    virtual ValuePtr _lshift (const ValuePtr& v1, const ValuePtr& v2) const {
      return T::mk(T::cast(v1) << T::cast(v2));
    };
    virtual ValuePtr _rshift (const ValuePtr& v1, const ValuePtr& v2) const {
      return T::mk(T::cast(v1) >> T::cast(v2));
    };
    virtual ValuePtr _mod (const ValuePtr& v1, const ValuePtr& v2) const {
      return T::mk(T::cast(v1) % T::cast(v2));
    };
    virtual ValuePtr _plus (const ValuePtr& v1, const ValuePtr& v2) const {
      return T::mk(T::cast(v1) + T::cast(v2));
    };
    virtual ValuePtr _minus (const ValuePtr& v1, const ValuePtr& v2) const {
      return T::mk(T::cast(v1) - T::cast(v2));
    };
    virtual ValuePtr _times (const ValuePtr& v1, const ValuePtr& v2) const {
      return T::mk(T::cast(v1) * T::cast(v2));
    };
    virtual ValuePtr _divide (const ValuePtr& v1, const ValuePtr& v2) const {
      if (T::cast(v2) == T::cast(T::mk(0))) {
        // Dividing by zero
        throw Oper::DivisionByZeroException(v2);
      } else {
        return T::mk(T::cast(v1) / T::cast(v2));
      }
    };
    virtual ValuePtr _dec (const ValuePtr& v1) const {
      return T::mk((T::cast(v1)) - 1);
    };
    virtual ValuePtr _inc (const ValuePtr& v1) const {
      return T::mk((T::cast(v1)) + 1);
    };
  };
};

#endif /* __OPER_H_ */
