/*
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * Or
 * UC Berkeley EECS Computer Science Division, 387 Soda Hall #1776, 
 * Berkeley, CA,  94707. Attention: P2 Group.
 * 
 * DESCRIPTION: P2's concrete type system: the kmeans type. 
 * 
 */
 
 
#ifndef __VAL_GAUSSIAN_MIXTURE_H__
#define __VAL_GAUSSIAN_MIXTURE_H__
 
#include "value.h"
#include "oper.h"
#include <iostream>
#include <string>

#include <prl/variable.hpp>
#include <prl/assignment.hpp>
#include <prl/numeric.hpp>
#include <prl/math/bindings/lapack.hpp>
#include <prl/datastructure/array_dataset.hpp>
#include <prl/factor/gaussian_factors.hpp>
#include <prl/factor/table_factor.hpp>
#include <prl/factor/mixture.hpp>
#include "prl/detail/shortcuts_def.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/lexical_cast.hpp>
#include <prl/learning/em_mog.hpp>


using namespace std;
using namespace prl;

 
class Val_Gaussian_Mixture : public Value {    

public:
  //! The matrix representation
  typedef math::bindings::lapack::double_matrix matrix_type;

  //! The vector representation
  typedef math::bindings::lapack::double_vector vector_type;

  //! The type of factors stored in this value
  typedef moment_gaussian<matrix_type, vector_type> factor_type;
  
  typedef mixture< factor_type > mixture_type;
  typedef em_mog< array_data<> > em_engine;
  
  // Required fields for all concrete types.
  // The type name
  const Value::TypeCode typeCode() const { return Value::GAUSSIAN_MIXTURE; };
  const char *typeName() const { return "gaussian_mixture"; };
  // Print the matrix as a string.
  string toString() const ;
  virtual string toConfString() const;

  // Marshal/unmarshal a matrix.
  void xdr_marshal_subtype( XDR *x );
  static ValuePtr xdr_unmarshal( XDR *x );

  // Constructors
  Val_Gaussian_Mixture() {};
  Val_Gaussian_Mixture(string filename, int64_t dim, int64_t var, double regul) {
    size_t k = 2;
    boost::mt19937 rng;
    var_vector v = u.new_vector_variables(var, dim); // var variables of dim dimensions
    array_data<> data = load_plain< array_data<> >(filename, v);
    engine(&data, k);
    mixture = engine.initialize(rng, regul);
  }
  Val_Gaussian_Mixture(const mixture_type& mixture) : mixture(mixture) { }
  virtual ~Val_Gaussian_Mixture() {};

  virtual unsigned int size() const { return sizeof(int64_t); }
  
  // Factory
  static ValuePtr mk();
  static ValuePtr mk(const mixture_type& mixture);
  static ValuePtr mk(string filename, int64_t dim, int64_t var, double regul); 
  
  // strict comparison
  int compareTo(ValuePtr v) const;
  
  // Casting methods;
  static const mixture_type& cast(ValuePtr v);
  const ValuePtr toMe(ValuePtr other) const { return mk(cast(other)); }
         
  static const opr::Oper* oper_;
  
  //! The set of all variables known to this host.
  static prl::universe u;
  
  //! Performs an iteration of EM and updates the estimates
  ValuePtr emupdate();
  
private:
  mixture_type mixture;
  double regul;
  em_engine engine;
};



#endif /* __VAL_GAUSSIAN_MIXTURE_H_*/

/* 
 * End of file
 */
