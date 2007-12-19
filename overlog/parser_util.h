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
 * DESCRIPTION: Parsing environment for Overlog (the P2 dialect of datalog)
 *
 */

#ifndef __PARSER_UTIL_H__
#define __PARSER_UTIL_H__

#include <deque>
#include <iostream>
#include <map>
#include "value.h"
#include "val_str.h"
#include <string>

class OL_Lexer;
class OL_Context;

inline bool fieldNameEq(string n1, string n2) 
{ 
  if ((n1[0] == '@' &&  n2[0] == '@') 
	  || (n1[0] != '@' && n2[0] != '@')) 
	return(n1==n2); 
  else if (n1[0] == '@' &&  n2[0] != '@') 
	return((n1.substr(1)) == n2); 
  else 
	return((n2.substr(1)) == n1); 
}

/**
 * A Parse_Expr represents expressions in the Overlog language
 */
class Parse_Expr {
public:
  Parse_Expr() : position_(-1) {};

  Parse_Expr(ValuePtr val) : v(val), position_(-1) {};

  Parse_Expr(Parse_Expr* val) : v(val->v), position_(-1) {};

  virtual ~Parse_Expr() {};

  virtual bool operator==(const Parse_Expr &e) 
  { return v == e.v; };

  virtual string toString() = 0;

  virtual void position(int p) { position_ = p; };

  virtual int position() { return position_; }

  ValuePtr v;

  int      position_;
};

typedef std::deque< Parse_Expr* > Parse_ExprList;

typedef std::deque< Parse_ExprList* > Parse_ExprListList;

// Boxing up a ValuePtr so we can pass it through the Bison parser
// union unscathed. 
class Parse_Val : public Parse_Expr { 
public:
  Parse_Val(ValuePtr val) : Parse_Expr(val), id_(false) {};
  virtual void id(bool i) { id_ = i; };
  virtual bool id() { return id_; };

  virtual string toString() { 
    return v->toString(); 
  };

  virtual ValuePtr value() { return v; };

private:
  bool id_;
};

class Parse_Var : public Parse_Expr { 
public:
  Parse_Var(ValuePtr var);

  Parse_Var(const string& var);
  
  /** Returns the location specifier marker along with the name of the
      variable */
  string
  toLocString();

  /** Returns the name of the variable */
  string
  toString();

  bool
  locspec();

  void
  setLocspec();

private:
  bool _locspec;
};

class Parse_Agg : public Parse_Expr {
public:
  string oper;

  static Parse_Expr* DONT_CARE;

  Parse_Agg(Parse_Expr* v,
            std::string o,
            ValuePtr p)
    : Parse_Expr(v), oper(o), parameter(p)
  {
    Parse_Var *pv = dynamic_cast<Parse_Var*>(v);
    if (pv != NULL) {
      locspec = pv->locspec();
    } else {
      locspec = false;
    }
  };

  virtual bool operator==(const Parse_Expr &e);

  virtual string toString();
  virtual string aggName();

  // The parameter of the aggregate, if it's parametric
  ValuePtr parameter;

  /** Am I the loc spec? */
  bool locspec;
};

class Parse_Bool : public Parse_Expr {
public:
  enum Operator {NOT, AND, OR, EQ, NEQ, GT, LT, LTE, GTE, RANGE, NOP};

  Parse_Bool(Operator o, Parse_Expr *l, Parse_Expr *r=NULL, bool id = false); 
  virtual ~Parse_Bool() { delete lhs; if (rhs) delete rhs; };
  virtual bool operator==(const Parse_Expr &e); 

  virtual string toString();

  Operator   oper;
  Parse_Expr *lhs;
  Parse_Expr *rhs;
  bool       id_;
};

class Parse_Range : public Parse_Expr {
public:
  enum Interval{RANGEOO, RANGEOC, RANGECO, RANGECC};

  Parse_Range(Interval i, Parse_Expr *l, Parse_Expr *r)
    : type(i), lhs(l), rhs(r) { };

  virtual bool operator==(const Parse_Expr &e);

  virtual string toString();

  Interval   type;
  Parse_Expr *lhs;
  Parse_Expr *rhs;
};

class Parse_Math : public Parse_Expr {
public:
  enum Operator {LSHIFT,
                 RSHIFT,
                 PLUS,
                 MINUS,
                 TIMES,
                 DIVIDE,
                 MODULUS,
                 BIT_XOR,
                 BIT_AND,
                 BIT_OR,
                 BIT_NOT,
                 NOP};
  Parse_Math(Operator o, Parse_Expr *l, Parse_Expr *r=NULL, bool i = false) 
    : oper(o), id(i), lhs(l), rhs(r) {
      // TODO: if (oper != NOP && rhs == NULL) ERROR!
  };
  ~Parse_Math() { delete lhs; if (rhs) delete rhs; };

  virtual bool operator==(const Parse_Expr &e);

  virtual string toString();

  virtual ValuePtr value();

  Operator   oper;
  bool       id;
  Parse_Expr *lhs;
  Parse_Expr *rhs;
};

class Parse_Function : public Parse_Expr {
public:
  Parse_Function(Parse_Expr *n, Parse_ExprList *a) 
    : Parse_Expr(n), args_(a) { };

  ~Parse_Function() { delete args_; };

  virtual string toString();

  string name() { return v->toString(); };

  void arg(Parse_Expr *arg) { args_->push_back(arg); };
  Parse_Expr* arg(int i) { return args_->at(i); };
  int args() { return args_->size(); };


  Parse_ExprList* args_;
};

class Parse_Vector : public Parse_Expr {
public:
  Parse_Vector(Parse_ExprList *o) : offsets_(o) { };
  ~Parse_Vector() { delete offsets_; };

  virtual string toString();
  virtual bool operator==(const Parse_Expr &e);

  Parse_Expr* offset(int i) const { return offsets_->at(i); };
  int offsets() const { return offsets_->size(); };

  Parse_ExprList *offsets_;

};

class Parse_VecAtom : public Parse_Expr {
public:
  Parse_VecAtom(Parse_Expr *var, Parse_Expr *o) : Parse_Expr(var), offset_(o) { };
  ~Parse_VecAtom() { };

  virtual string toString();
  virtual bool operator==(const Parse_Expr &e);

  Parse_Expr *offset() const { return offset_; }

  Parse_Expr *offset_;
};


class Parse_Matrix : public Parse_Expr {
public:
  Parse_Matrix(Parse_ExprListList *r, OL_Context *ctxt);
  ~Parse_Matrix() { delete rows_;};

  virtual string toString();
  virtual bool operator==(const Parse_Expr &e);

  Parse_Expr* offset(int i, int j) const { return (rows_->at(i)->at(j)); };
  void bounds(uint64_t &r, uint64_t &c) const {r = rows_->size(); c = rows_->at(0)->size();};

  Parse_ExprListList *rows_;
};

class Parse_MatAtom : public Parse_Expr {
public:
  Parse_MatAtom(Parse_Expr *var, Parse_Expr *o1, Parse_Expr *o2) : Parse_Expr(var), offset1_(o1), offset2_(o2) { };
  ~Parse_MatAtom() { };

  virtual string toString();
  virtual bool operator==(const Parse_Expr &e);

  Parse_Expr *offset1() const { return offset1_; }
  Parse_Expr *offset2() const { return offset2_; }

  Parse_Expr *offset1_;
  Parse_Expr *offset2_;
};

class Parse_Term {
public:
  virtual ~Parse_Term() {};

  virtual string toString() = 0;
  void position(int p) { position_ = p; };
  int position() { return position_; };

  int position_;
};
typedef std::deque<Parse_Term *> Parse_TermList;


class Parse_FunctorName {
public:
  Parse_FunctorName(Parse_Expr *n);

  string toString();

  string name;
};


class Parse_Functor : public Parse_Term {
public:
  Parse_Functor(Parse_FunctorName* f,
                Parse_ExprList* a,
                Parse_Expr* l = NULL);

  virtual ~Parse_Functor() {delete fn; delete args_; };

  virtual string toString();

  /** If this functor contains an aggregate, return its field position,
      otherwise -1 */
  int
  aggregate();

  int find(Parse_Expr *arg);
  int find(string argname);

  void arg(Parse_Expr *arg) { args_->push_back(arg); };
  Parse_Expr* arg(int i) { return args_->at(i); };
  int args() { return args_->size(); };
  string getlocspec();

  void replace(int p, Parse_Expr *e);

  
  /** The name of this tuple */
  Parse_FunctorName* fn;


  Parse_ExprList    *args_;
  string loc_;
};

class Parse_Assign : public Parse_Term {
public:
  Parse_Assign(Parse_Expr *v, Parse_Expr *a) 
    : var(NULL), assign(a) {
    var = dynamic_cast<Parse_Var*>(v);
  };
  virtual ~Parse_Assign() { delete var; delete assign; };

  virtual string toString();

  Parse_Var  *var;
  Parse_Expr *assign;
};

class Parse_Select : public Parse_Term {
public:
  Parse_Select(Parse_Expr *s) {
    select = dynamic_cast<Parse_Bool*>(s);
  };
  virtual ~Parse_Select() { delete select; }

  virtual string toString();

  Parse_Bool *select;
};

class Parse_AggTerm : public Parse_Term {
public: 
  Parse_AggTerm(string oper, 
		Parse_ExprList *groupByFields, 
		Parse_ExprList *aggFields, 
		Parse_Term *baseTerm)
    : _groupByFields(groupByFields),
      _aggFields(aggFields), _baseTerm(baseTerm),
      _oper(oper)
  { };

  Parse_ExprList *_groupByFields;
  Parse_ExprList *_aggFields; 
  Parse_Term *_baseTerm;
  string _oper;
  virtual string toString();
};

#endif /* __PARSER_UTIL_H__ */
