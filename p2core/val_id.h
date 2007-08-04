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
 * DESCRIPTION: P2's concrete type system: ring identifier.
 *
 */

#ifndef __VAL_ID_H__
#define __VAL_ID_H__

#include "value.h"
#include "ID.h"
#include "oper.h"

class Val_ID : public Value {

public:  
  // Required fields for all concrete types.

  const Value::TypeCode typeCode() const { return Value::ID; };

  // The type name
  const char *typeName() const { return "ID"; };

  virtual string
  toString() const { return i->toString(); };


  virtual string
  toConfString() const;


  virtual unsigned int
  size() const { return sizeof(ID); }


  // Marshalling and unmarshallng
  void xdr_marshal_subtype( XDR *x );


  static ValuePtr xdr_unmarshal( XDR *x );


  // Constructors
  Val_ID(IDPtr theID) : i(theID) {};


  Val_ID(std::vector<uint32_t> theID);


  Val_ID(uint32_t theID);


  Val_ID(uint64_t theID);


  Val_ID(std::string);


  virtual ~Val_ID() {};

  // Factory
  static ValuePtr mk(IDPtr theID) 
    { ValuePtr p(new Val_ID(theID)); return p; };
  static ValuePtr mk(std::vector<uint32_t> theID) 
    { ValuePtr p(new Val_ID(theID)); return p; };
  static ValuePtr mk(uint32_t theID)              
    { ValuePtr p(new Val_ID(theID)); return p; };
  static ValuePtr mk(uint64_t theID) 
    { ValuePtr p(new Val_ID(theID)); return p; };
  static ValuePtr mk(std::string theID) 
    { ValuePtr p(new Val_ID(theID)); return p; };

  // Strict comparison
  int compareTo(ValuePtr) const;

  // Casting
  static IDPtr cast(ValuePtr v);
  const ValuePtr toMe(ValuePtr other) const { return mk(cast(other)); }

  static const opr::Oper* oper_;
private:
  /** The ID */
  IDPtr i;
};

#endif /* __VAL_ID_H_ */
