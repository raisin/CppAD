// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include "tape.hpp"
/*
{xrst_begin val_op_con_xam.cpp dev}

Constant Operator Example
#########################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_op_con_xam.cpp}
*/
// BEGIN_C++
bool con_xam()
{  bool ok = true;
   //
   // tape
   tape_t<double> tape;
   size_t n_ind = 1;
   size_t index_of_zero = size_t ( tape.set_ind(n_ind) );
   ok &= index_of_zero == n_ind;
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(1);
   //
   // tape
   addr_t five = tape.record_con_op(5.0); // 5.0
   op_arg[0] = 0; // x[0]
   op_arg[1] = five;
   dep_vec[0] = tape.record_op(add_op_enum, op_arg); // x[0] + 5.0
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(1);
   x[0] = 6.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // ok
   ok &= tape.op_vec().size()  == 3;
   ok &= tape.arg_vec().size() == 4;
   ok &= tape.con_vec().size() == 2;
   ok &= tape.n_val()          == n_ind + 3;
   //
   // y
   Vector<double> y(1);
   y[0] = val_vec[ dep_vec[0] ];
   //
   // ok
   ok &= y[0] == x[0] + 5.0;
   //
   return ok;
}
// END_C++
