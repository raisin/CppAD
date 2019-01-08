# ifndef CPPAD_CORE_CHKPOINT_TWO_REVERSE_HPP
# define CPPAD_CORE_CHKPOINT_TWO_REVERSE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file chkpoint_two/reverse.hpp
Second generation checkpoint reverse mode.
*/
/*!
Link from chkpoint_two to reverse mode

\param order_up [in]
highest order Taylor coefficient aht we are computing derivative of

\param taylor_x [in]
Taylor coefficients corresponding to x for this calculation.

\param taylor_y [in]
Taylor coefficient corresponding to y for this calculation

\param partial_x [out]
Partials w.r.t. the x Taylor coefficients.

\param partiay_y [in]
Partials w.r.t. the y Taylor coefficients.

See the reverse mode in user's documentation for atomic_three
*/
template <class Base>
bool chkpoint_two<Base>::reverse(
    size_t                    order_up   ,
    const vector<Base>&       taylor_x   ,
    const vector<Base>&       taylor_y   ,
    vector<Base>&             partial_x  ,
    const vector<Base>&       partial_y  )

{   ADFun<Base>* g_ptr = &g_;
    if( use_in_parallel_ )
    {   size_t thread = thread_alloc::thread_num();
        allocate_member(thread);
        g_ptr = &(member_[thread]->g_);
    }
    // compute forward mode Taylor coefficient orders 0 through order_up
# ifdef NDEBUG
    g_ptr->Forward(order_up, taylor_x);
# else
    vector<Base> check = g_ptr->Forward(order_up, taylor_x);
    CPPAD_ASSERT_UNKNOWN( taylor_y.size() == check.size() )
    for(size_t i = 0; i < taylor_y.size(); ++i)
        CPPAD_ASSERT_UNKNOWN( taylor_y[i] == check[i] );
# endif
    // now can run reverse mode
    partial_x = g_ptr->Reverse(order_up+1, partial_y);
    //
    return true;
}

} // END_CPPAD_NAMESPACE
# endif