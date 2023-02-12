# ifndef  CPPAD_LOCAL_VAL_GRAPH_COMP_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_COMP_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_comp_op dev}
{xrst_spell
   xam
}

The Compare Value Operator
##########################

Prototype
*********
{xrst_literal
   // BEGIN_COM_OP_T
   // END_COM_OP_T
}

Context
*******
This class checks if the comparison between two values has changed.
It is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a comp_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``comp_op_enum`` .

n_before
********
This override of :ref:`val_base_op@n_before` return 1.

n_after
*******
This override of :ref:`val_base_op@n_after` return 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 3.

n_res
*****
This override of :ref:`val_base_op@n_res` return 1.

eval
****
This override of :ref:`val_base_op@eval` .
If the comparison is false (true), one (zero) is added to
the compare_false argument to eval.

trace
=====
If trace is true, this member function prints the following values:

.. csv-table::
   :widths: auto

   **meaning**,name,left_index,right_index,compare_false
   **width**,11,5,5,5,10

#. width is the number of characters used to print the value
   not counting an extra space that is placed between values
#. name is the name of the operator
#. left_index is the value vector index for the left operand
#. right_index is the value vector index for the right operand
#. compare_false is the value of the compare_false argument after the eval

{xrst_toc_hidden
   val_graph/comp_xam.cpp
}
Example
*******
The file :ref:`com_xam.cpp <val_comp_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_comp_op}
*/
// BEGIN_COM_OP_T
template <class Value>
class comp_op_t : public base_op_t<Value> {
public:
   // n_before
   addr_t n_before(void) const override
   {  return 1; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 0; }
   //
   // get_instance
   static comp_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static comp_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return comp_op_enum; }
// END_COM_OP_T
   //
   // n_arg
   addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 3; }
   //
   // n_res
   addr_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 0; }
   //
   // eval
   void eval(
      bool                  trace          ,
      addr_t                arg_index      ,
      const Vector<addr_t>& arg_vec        ,
      const Vector<Value>&  con_vec        ,
      addr_t                res_index      ,
      size_t&               compare_false  ,
      Vector<Value>&        val_vec        ) const override
   {
      //
      // compare_enum, left_index, right_index
      compare_enum_t compare_enum = compare_enum_t( arg_vec[arg_index + 0] );
      addr_t left_index           = arg_vec[arg_index + 1];
      addr_t right_index          = arg_vec[arg_index + 2];
      //
      // left, right
      const Value&   left          = val_vec[left_index];
      const Value&   right         = val_vec[right_index];
      //
      // res, name
      bool res;
      const char* name;
      switch( compare_enum )
      {  //
         // eq
         case compare_eq_enum:
         res  = left == right;
         name = "eq";
         break;
         //
         // ne
         case compare_ne_enum:
         res  = left != right;
         name = "ne";
         break;
         //
         // lt
         case compare_lt_enum:
         res  = left < right;
         name = "lt";
         break;
         //
         // le
         case compare_le_enum:
         res  = left <= right;
         name = "le";
         break;
         //
         // no
         case compare_no_enum:
         res  = true;
         name = "no";
         break;
         //
         default:
         CPPAD_ASSERT_UNKNOWN(false);
         res  = false; // to avoid compiler warning
         name = "";
      }
      //
      // compare_false
      if( ! res )
         ++compare_false;
      //
      // trace
      if( ! trace )
         return;
      //
      const char* res_str;
      if( res )
         res_str = "true";
      else
         res_str = "false";
      //
      std::printf(
         "%11s %5d %5d %10s\n", name, left_index, right_index, res_str
      );
      return;
   }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
