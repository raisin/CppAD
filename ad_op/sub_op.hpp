# ifndef SUB_OP_HPP
# define SUB_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "binary_op.hpp"

// sub_op_t
template <class Base>
class sub_op_t : public binary_op_t<Base> {
public:
   // op_enum
   op_enum_t op_enum(void) const override
   {  return sub_op_enum; }
   //
   // eval
   void eval(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Base>&         value_vec    ) const override
   {  const Base& left      = value_vec[ arg_vec[arg_index + 0] ];
      const Base& right     = value_vec[ arg_vec[arg_index + 1] ];
      value_vec[res_index]  = left - right;
   }
};
template <class Base>
sub_op_t<Base>* get_sub_op_instance(void)
{  static sub_op_t<Base> instance;
   return &instance;
}

# endif
