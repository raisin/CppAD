# ifndef  CPPAD_LOCAL_VAL_GRAPH_RENUMBER_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_RENUMBER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/sparse/size_setvec.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/is_pod.hpp>

// Tell pod_vector class that each size_setvec<addr_t>::pair_s_type is
// plain old data and hence the corresponding constructor need not be called.
 namespace CppAD { namespace local {
   template <> inline bool
   is_pod< sparse::size_setvec<addr_t>::pair_s_type > (void)
   {  return true; }
 } }



namespace CppAD { namespace local { namespace val_graph {
//
// op_hash_table_t
template <class Value>
class op_hash_table_t {
   typedef typename tape_t<Value>::op_info_t op_info_t;
private:
   //
   // tape_
   const tape_t<Value>& tape_;
   //
   // hash_value_
   std::hash<Value> hash_value_;
   //
   // table
   CppAD::local::sparse::size_setvec<addr_t> table_;
   //
   // hash_code
   addr_t hash_code(addr_t n_arg, op_enum_t op_enum, addr_t arg_index)
   {  //
      // arg_vec, con_vec
      const Vector<addr_t>& arg_vec = tape_.arg_vec();
      const Vector<Value>&  con_vec = tape_.con_vec();
      //
      size_t code;
      if( op_enum == con_op_enum )
         code = hash_value_( con_vec[  arg_vec[arg_index] ] );
      else
      {  code  = size_t(op_enum);
         for(addr_t i = 0; i < n_arg; ++i)
            code += size_t( arg_vec[arg_index + i] );
      }
      code = code % table_.n_set();
      return addr_t( code );
   }
public:
   // -------------------------------------------------------------------------
   // ctor
   op_hash_table_t(
         const tape_t<Value>&     tape        ,
         addr_t                   n_hash_code
   ) :
   tape_( tape )
   {  // table_
      addr_t n_op = addr_t( tape.op_enum_vec().size() );
      CPPAD_ASSERT_UNKNOWN( size_t(n_op) == tape.op_vec().size() );
      //
      table_.resize( n_hash_code, n_op );
   }
   // -------------------------------------------------------------------------
   Vector<addr_t> size_count()
   {  Vector<addr_t> count;
      addr_t n_set  = table_.n_set();
      for(addr_t i = 0; i < n_set; ++i)
      {  addr_t number_elements = table_.number_elements(i);
         if( size_t( number_elements ) >= count.size() )
         {  size_t old_size = count.size();
            size_t new_size = number_elements + 1;
            count.resize(new_size);
            for(size_t j = old_size; j < new_size; ++j)
               count[j] = 0;
         }
         count[number_elements] += 1;
      }
      return count;
   }
   // -------------------------------------------------------------------------
   // match_op
   addr_t match_op(addr_t i_op, const Vector<addr_t>& new_val_index)
   {  assert( i_op < table_.end() );
      //
      // arg_vec, con_vec, op_vec, op_enum_vec
      const Vector<addr_t>&    arg_vec = tape_.arg_vec();
      const Vector<Value>&     con_vec = tape_.con_vec();
      const Vector<op_info_t>& op_vec = tape_.op_vec();
      const Vector<uint8_t>&   op_enum_vec = tape_.op_enum_vec();
      //
      // op_enum_i, op_ptr
      op_enum_t op_enum_i      = op_enum_t( op_enum_vec[i_op] );
      base_op_t<Value>* op_ptr = tape_t<Value>::base_op_ptr(op_enum_i);
      //
      // arg_index_i, n_arg
      addr_t arg_index_i = op_vec[i_op].arg_index;
      addr_t n_arg       = op_ptr->n_arg(arg_index_i, arg_vec);
      //
      // nan
      if( op_enum_i == con_op_enum )
      {  if( CppAD::isnan( con_vec[ arg_vec[arg_index_i] ] ) )
         {  CPPAD_ASSERT_UNKNOWN( op_enum_t(op_enum_vec[0]) == con_op_enum );
            CPPAD_ASSERT_UNKNOWN( op_vec[0].arg_index == 0 );
            CPPAD_ASSERT_UNKNOWN( arg_vec[0] == 0 );
            CPPAD_ASSERT_UNKNOWN( CppAD::isnan( con_vec[0] ) );
            return 0;
         }
      }
      //
      // code
      addr_t code = hash_code(n_arg, op_enum_i, arg_index_i);
      //
      // itr
      local::sparse::size_setvec_const_iterator<addr_t> itr(table_, code);
      while( *itr != table_.end() )
      {  // op_enum_j, arg_index_j
         addr_t j_op           = *itr;
         op_enum_t op_enum_j   = op_enum_t( op_enum_vec[j_op] );
         addr_t    arg_index_j = op_vec[j_op].arg_index;
         //
         // match
         bool match = op_enum_i == op_enum_j;
         if( match && op_enum_i == con_op_enum )
         {  // 2DO: change to identically equal
            const Value& c_i = con_vec[ arg_vec[arg_index_i] ];
            const Value& c_j = con_vec[ arg_vec[arg_index_j] ];
            match = c_i == c_j;
         }
         else if( match )
         {  for(addr_t k = 0; k < n_arg; ++k)
            {  addr_t val_index_i = new_val_index[ arg_vec[arg_index_i+k] ];
               addr_t val_index_j = new_val_index[ arg_vec[arg_index_j+k] ];
               match &= val_index_i == val_index_j;
            }
            bool communative = op_enum_i == add_op_enum;
            if( communative && ! match )
            {  addr_t val_index_i = new_val_index[ arg_vec[arg_index_i+0] ];
               addr_t val_index_j = new_val_index[ arg_vec[arg_index_j+1] ];
               match = val_index_i == val_index_j;
               //
               val_index_i = new_val_index[ arg_vec[arg_index_i+1] ];
               val_index_j = new_val_index[ arg_vec[arg_index_j+0] ];
               match &= val_index_i == val_index_j;
            }
         }
         if( match )
            return j_op;
         //
         // itr
         ++itr;
      }
      table_.add_element(code, i_op);
      return i_op;
   }
};
/*
-------------------------------------------------------------------------------
{xrst_begin val_tape_renumber dev}
{xrst_spell
   dep
   renumber
   xam
}

Value Re-Numbering
##################

Discussion
**********
This routine uses hash coding to find operators that are equivalent
to a previous operators in the tape.
It changes the use of an operator's results to use of the
results for the equivalent previous operator with lowest index.
This creates an equivalent tape where replaced operators are not removed,
but the are dead code in the new tape.

Compare Operators
*****************
If two or more compare operators are identical, the first will be kept as
is and the others will be changed to compare_no_enum operators.
In this case future calls to eval will on add one to
:ref:`val_tape@eval@compare_false`,
for each unique comparison that is false.
be removed.

dep_vec
*******
This may change the indices corresponding to the dependent vector; i.e.,
:ref:`val_tape@dep_vec`.

Reference
*********
`value numbering <https://en.wikipedia.org/wiki/Value_numbering>`_ .

{xrst_toc_hidden
   val_graph/renumber_xam.cpp
}
Example
*******
The file :ref:`renumber_xam.cpp <val_renumber_xam.cpp-name>` is an
example and test of tape.renumber().

{xrst_end val_tape_renumber}
-------------------------------------------------------------------------------
*/
template <class Value>
void tape_t<Value>::renumber(void)
{
   // -----------------------------------------------------------------------
   // SAS Global Value Renumbering
   // https://en.wikipedia.org/wiki/Value_numbering
   // -----------------------------------------------------------------------
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // thread, initial_inuse
   size_t thread        = thread_alloc::thread_num();
   size_t initial_inuse = thread_alloc::inuse(thread);
# endif
   //
   //
   // n_op
   addr_t n_op = addr_t( op_vec_.size() );
   //
   // op_hash_table
   addr_t n_hash_code = 1 + (n_val_ / 2);
   op_hash_table_t<Value>  op_hash_table(*this, n_hash_code);
   //
   // new_val_index
   Vector<addr_t> new_val_index( n_val_ );
   for(addr_t i = 0; i < addr_t(n_val_); ++i)
      new_val_index[i] = i;
   //
   // i_op
   for(addr_t i_op = 0; i_op < n_op; ++i_op)
   {  //
      // op_enum, op_ptr
      op_enum_t op_enum        = get_op_enum(i_op);
      base_op_t<Value>* op_ptr = base_op_ptr(op_enum);
      //
      // j_op
      addr_t j_op = op_hash_table.match_op(i_op, new_val_index);
      if( j_op != i_op )
      {  assert( j_op < i_op );
         //
         // new_val_index
         // mapping so that op_j results will be used instead of op_i results;
         // i.e., op_i becomes dead code.
         addr_t arg_index_i = op_vec_[i_op].arg_index;
         addr_t res_index_i = op_vec_[i_op].res_index;
         addr_t res_index_j = op_vec_[j_op].res_index;
         addr_t n_res       = op_ptr->n_res(arg_index_i, arg_vec_);
         if( n_res == 0 )
         {
# ifndef NDEBUG
            CPPAD_ASSERT_UNKNOWN( op_enum == comp_op_enum );
# endif
            arg_vec_[arg_index_i + 0] = compare_no_enum;
         }
         else for(addr_t k = 0; k < n_res; ++k)
            new_val_index[res_index_i + k] = res_index_j + k;
      }
   }
   //
   // arg_vec_
   for(addr_t i_op = 0; i_op < n_op; ++i_op)
   {  //
      // op_enum, op_ptr
      op_enum_t op_enum        = get_op_enum(i_op);
      base_op_t<Value>* op_ptr = base_op_ptr(op_enum);
      //
      // arg_index, n_arg
      addr_t    arg_index = op_vec_[i_op].arg_index;
      addr_t    n_arg     = op_ptr->n_arg(arg_index, arg_vec_);
      //
      // n_before, n_x
      addr_t n_before = op_ptr->n_before();
      addr_t n_x = n_arg - n_before - op_ptr->n_after();
      //
      for(addr_t i = 0; i < n_x; ++i)
      {  addr_t val_index = arg_index + n_before + i;
         arg_vec_[val_index] = new_val_index[ arg_vec_[val_index] ];
      }
   }
   //
   // dep_vec_
   for(size_t i = 0; i < dep_vec_.size(); ++i)
      dep_vec_[i] = new_val_index[ dep_vec_[i] ];

# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // A set size more than one represents a collision
   Vector<addr_t> size_count = op_hash_table.size_count();
   for(size_t i = 0; i < size_count.size(); ++i)
      std::cout << "size = " << i << ", count = " << size_count[i] << "\n";
   //
   // inuse
   size_t final_inuse = thread_alloc::inuse(thread);
   std::cout << "renumber:  inuse = " << final_inuse - initial_inuse << "\n";
# endif
   return;
}
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
