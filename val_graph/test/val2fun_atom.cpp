// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include "../atomic_xam.hpp"

namespace { // BEIGN_EMPTY_NAMESPACE
// ----------------------------------------------------------------------------
bool dynamic_atom(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // atomic_xam
   val_atomic_xam atomic_xam;
   //
   // tape, ok
   tape_t<double> tape;
   size_t n_ind = 5;
   size_t index_of_nan = size_t ( tape.set_ind(n_ind) );
   ok &= index_of_nan == n_ind;
   //
   // op_arg
   Vector<addr_t> call_arg(4), op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // atomic_index
   size_t atomic_index = atomic_xam.atomic_index();
   //
   // g_0(x) = p[0] + p[1]
   // g_1(x) = p[2] * p[3]
   for(addr_t i = 0; i < 4; ++i)
      call_arg[i] = i;
   size_t call_id    = 0;
   size_t n_call_res = 2;
   addr_t res_index = tape.record_call_op(
      atomic_index, call_id, n_call_res, call_arg
   );
   //
   // dep_vec[0] = x[0] + p[0] + p[1]
   op_arg[0]  = 4;          // x[0]
   op_arg[1]  = res_index;  // p[0] - p[1]
   dep_vec[0] = tape.record_op(add_op_enum, op_arg);
   //
   // dep_vec[1] = x[0] - p[2] * p[3]
   op_arg[0]  = 4;              // x[1]
   op_arg[1]  = res_index + 1;  // p[2] * p[3]
   dep_vec[1] = tape.record_op(sub_op_enum, op_arg);
   //
   // set_dep
   tape.set_dep( dep_vec );
   /*
   // trace
   bool trace = true;
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = double(5 + i);
   tape.eval(trace, val_vec);
   */
   //
   // ADFun
   Vector<size_t> dyn_ind(4), var_ind(1);
   for(addr_t i = 0; i < 4; i++)
      dyn_ind[i] = i;
   var_ind[0] = 4;
   CppAD::ADFun<double> f;
   f.val2fun( tape, dyn_ind, var_ind );
   //

   // p, x
   Vector<double> p(4), x(1), y(2);
   for(addr_t i = 0; i < 4; i++)
      p[i] = double(i + 5);
   x[0] = 9.0;
   //
   // f, y
   f.new_dynamic(p);
   y = f.Forward(0, x);
   //
   ok &= y[0] == x[0] + (p[0] + p[1]);
   ok &= y[1] == x[0] - (p[2] * p[3]);
   //
   return ok;
}
// ----------------------------------------------------------------------------
bool variable_atom(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // atomic_xam
   val_atomic_xam atomic_xam;
   //
   // tape, ok
   tape_t<double> tape;
   size_t n_ind = 5;
   size_t index_of_nan = size_t ( tape.set_ind(n_ind) );
   ok &= index_of_nan == n_ind;
   //
   // op_arg
   Vector<addr_t> call_arg(4), op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // atomic_index
   size_t atomic_index = atomic_xam.atomic_index();
   //
   // g_0(x) = x[0] + x[1]
   // g_1(x) = x[2] * x[3]
   for(addr_t i = 0; i < 4; ++i)
      call_arg[i] = i;
   size_t call_id    = 0;
   size_t n_call_res = 2;
   addr_t res_index = tape.record_call_op(
      atomic_index, call_id, n_call_res, call_arg
   );
   //
   // dep_vec[0] = p[0] + x[0] + x[1]
   op_arg[0]  = 4;          // p[0]
   op_arg[1]  = res_index;  // x[0] - x[1]
   dep_vec[0] = tape.record_op(add_op_enum, op_arg);
   //
   // dep_vec[1] = p[0] - x[2] * x[3]
   op_arg[0]  = 4;              // p[1]
   op_arg[1]  = res_index + 1;  // x[2] * x[3]
   dep_vec[1] = tape.record_op(sub_op_enum, op_arg);
   //
   // set_dep
   tape.set_dep( dep_vec );
   /*
   // trace
   bool trace = true;
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = double(5 + i);
   tape.eval(trace, val_vec);
   */
   // ADFun
   Vector<size_t> var_ind(4), dyn_ind(1);
   for(addr_t i = 0; i < 4; i++)
      var_ind[i] = i;
   dyn_ind[0] = 4;
   CppAD::ADFun<double> f;
   f.val2fun( tape, dyn_ind , var_ind);
   //
   // p, x
   Vector<double> x(4), p(1), y(2);
   for(addr_t i = 0; i < 4; i++)
      x[i] = double(i + 5);
   p[0] = 9.0;
   //
   // f, y
   f.new_dynamic(p);
   y = f.Forward(0, x);
   //
   ok &= y[0] == p[0] + (x[0] + x[1]);
   ok &= y[1] == p[0] - (x[2] * x[3]);
   //
   return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_val2fun_atom(void)
{  bool ok = true;
   ok     &= dynamic_atom();
   ok     &= variable_atom();
   return ok;
}
