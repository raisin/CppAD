# ifndef CPPAD_UTILITY_SPARSE_RC_HPP
# define CPPAD_UTILITY_SPARSE_RC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

/*
$begin sparse_rc$$
$spell
    CppAD
    const
    nnz
    cppad
    hpp
    rc
    nr
    nc
    resize
    std
    ostream
$$
$section Row and Column Index Sparsity Patterns$$

$head Syntax$$

$subhead include$$
$codei%# include <cppad/utility/sparse_rc.hpp>
%$$

$subhead Constructor$$
$codei%sparse_rc<%SizeVector%>  %empty%
%$$
$codei%sparse_rc<%SizeVector%>  %pattern%(%nr%, %nc%, %nnz%)
%$$
$codei%sparse_rc<%SizeVector%>  %pattern%(%other%)
%$$

$subhead Assignment$$
$icode%pattern% = %other%
%$$
$icode%pattern%.swap(%other%)
%$$

$subhead Equality$$
$icode%equal% = %pattern% == %other%
%$$.

$subhead Setting$$
$icode%resize%(%nr%, %nc%, %nnz%)
%$$
$icode%pattern%.set(%k%, %r%, %c%)
%$$
$icode%pattern%.push_back(%r%, %c%)
%$$

$subhead Properties$$
$icode%pattern%.nr()
%$$
$icode%pattern%.nc()
%$$
$icode%pattern%.nnz()
%$$
$codei%const %SizeVector%& %row%( %pattern%.row() )
%$$
$codei%const %SizeVector%& %col%( %pattern%.col() )
%$$
$icode%row_major% = %pattern%.row_major()
%$$
$icode%col_major% = %pattern%.col_major()
%$$

$subhead Output$$
$icode%os% << %pattern%
%$$

$head SizeVector$$
We use $icode SizeVector$$ to denote $cref SimpleVector$$ class
$cref/with elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.
In addition, $icode SimpleVector$$ must support the $code swap$$ operation
between two of its vectors.

$head empty$$
This is an empty sparsity pattern. To be specific,
the corresponding number of rows $icode nr$$,
number of columns $icode nc$$,
and number of possibly non-zero values $icode nnz$$,
are all zero.

$head pattern$$
This object is used to hold a sparsity pattern for a matrix.
The sparsity $icode pattern$$ is $code const$$
except during its constructor, $code resize$$, and $code set$$.

$head other$$

$subhead Assignment and Constructor$$
In the assignment and constructor, $icode other$$ has prototype
$codei%
    const sparse_rc<%SizeVector%>&  %other%
%$$
After the assignment and constructor, $icode pattern$$ is an independent copy
of $icode other$$; i.e. it has all the same values as $icode other$$
and changes to $icode pattern$$ do not affect $icode other$$.

$subhead Move Semantics Assignment and Constructor$$
In the assignment and constructor, if $icode other$$ has prototype
$codei%
    sparse_rc<%SizeVector%>&&  %other%
%$$
A move semantics version of the assignment or constructor is used; e.g.,
when $icode other$$ is a function return value.

$subhead swap$$
In the swap operation, $icode other$$ has prototype
$codei%
    sparse_rc<%SizeVector%>&  %other%
%$$
After the swap operation $icode other$$ ($icode pattern$$) is equivalent
to $icode pattern$$ ($icode other$$) before the operation.

$subhead Equality$$
In the equality operation, $icode other$$ has prototype
$codei%
    const sparse_rc<%SizeVector%>&  %other%
%$$
The two sparsity patterns are equal if the following conditions hold:
$list number$$
The number of rows
$icode%pattern%.nr()%$$ and $icode%other%.nr()%$$ are equal.
$lnext
The number of columns
$icode%pattern%.nc()%$$ and $icode%other%.nc()%$$ are equal.
$lnext
The number of non-zero values
$icode%pattern%.nnz()%$$ and $icode%other%.nnz()%$$ are equal.
$lnext
The set of (row, column) pairs corresponding to
$icode pattern$$ and $icode other$$, are equal.
$lend
Determining equality requires sorting both patterns

$head nr$$
This argument has prototype
$codei%
    size_t %nr%
%$$
It specifies the number of rows in the sparsity pattern.
The function call $code nr()$$ returns the value of $icode nr$$.

$head nc$$
This argument has prototype
$codei%
    size_t %nc%
%$$
It specifies the number of columns in the sparsity pattern.
The function call $code nc()$$ returns the value of $icode nc$$.

$head nnz$$
This argument has prototype
$codei%
    size_t %nnz%
%$$
It specifies the number of possibly non-zero
index pairs in the sparsity pattern.
The function call $code nnz()$$ returns the value of $icode nnz$$.

$head resize$$
The current sparsity pattern is lost and a new one is started
with the specified parameters. The elements in the $icode row$$
and $icode col$$ vectors should be assigned using $code set$$.

$head set$$
This function sets the values
$codei%
    %row%[%k%] = %r%
    %col%[%k%] = %c%
%$$

$head push_back$$
This function  the value $icode r$$ to the back of $icode row$$,
the value $icode c$$ to the back of $icode col$$,
and increases $icode nnz$$ by one.
This operation requires $icode SizeVector$$ to support the
$code push_back$$ operation
(which is not part of the SimpleVector requirements).

$subhead k$$
This argument has type
$codei%
    size_t %k%
%$$
and must be less than $icode nnz$$.

$subhead r$$
This argument has type
$codei%
    size_t %r%
%$$
It specifies the value assigned to $icode%row%[%k%]%$$ and must
be less than $icode nr$$.

$subhead c$$
This argument has type
$codei%
    size_t %c%
%$$
It specifies the value assigned to $icode%col%[%k%]%$$ and must
be less than $icode nc$$.

$head row$$
This vector has size $icode nnz$$ and
$icode%row%[%k%]%$$
is the row index of the $th k$$ possibly non-zero
index pair in the sparsity pattern.

$head col$$
This vector has size $icode nnz$$ and
$icode%col%[%k%]%$$ is the column index of the $th k$$ possibly non-zero
index pair in the sparsity pattern.

$head row_major$$
This vector has prototype
$codei%
    %SizeVector% %row_major%
%$$
and its size $icode nnz$$.
It sorts the sparsity pattern in row-major order.
To be specific,
$codei%
    %col%[ %row_major%[%k%] ] <= %col%[ %row_major%[%k%+1] ]
%$$
and if $icode%col%[ %row_major%[%k%] ] == %col%[ %row_major%[%k%+1] ]%$$,
$codei%
    %row%[ %row_major%[%k%] ] < %row%[ %row_major%[%k%+1] ]
%$$
This routine generates an assert if there are two entries with the same
row and column values (if $code NDEBUG$$ is not defined).

$head col_major$$
This vector has prototype
$codei%
    %SizeVector% %col_major%
%$$
and its size $icode nnz$$.
It sorts the sparsity pattern in column-major order.
To be specific,
$codei%
    %row%[ %col_major%[%k%] ] <= %row%[ %col_major%[%k%+1] ]
%$$
and if $icode%row%[ %col_major%[%k%] ] == %row%[ %col_major%[%k%+1] ]%$$,
$codei%
    %col%[ %col_major%[%k%] ] < %col%[ %col_major%[%k%+1] ]
%$$
This routine generates an assert if there are two entries with the same
row and column values (if $code NDEBUG$$ is not defined).

$children%
    example/utility/sparse_rc.cpp
%$$
$head Example$$
The file $cref sparse_rc.cpp$$
contains an example and test of this class.

$head os$$
If $icode os$$ is an $code std::ostream$$, the operation
$codei%
    %os% << %pattern%
%$$
outputs $icode pattern$$ to the $icode os$$ stream.
The output begins with a left brace $code {$$
and ends with a right brace $code }$$.
The output is in row major order and has one line for each row.
The row index is output at the beginning of a line
and the column indices follow.

$end
*/
# include <cstddef> // for size_t
# include <cppad/core/cppad_assert.hpp>  // for CPPAD_ASSERT
# include <cppad/utility/index_sort.hpp> // for row and column major ordering

namespace CppAD { // BEGIN CPPAD_NAMESPACE

// sparsity pattern for a matrix with indices of type size_t
template <class SizeVector>
class sparse_rc {
private:
    // number of rows in the sparsity pattern
    size_t nr_;
    //
    // number of columns in the sparsity pattern
    size_t nc_;
    //
    // number of possibly non-zero index pairs
    size_t nnz_;
    //
    // row_[k] is the row index for the k-th possibly non-zero entry
    SizeVector row_;
    //
    // col_[k] is the column index for the k-th possibly non-zero entry
    SizeVector col_;
public:
    // default constructor
    // Eigen vector is ambiguous for row_(0), col_(0) so use default ctor
    sparse_rc(void)
    : nr_(0), nc_(0), nnz_(0)
    { }
    //
    // move semantics constructor
    // (none of the default constructor values are used by destructor)
    sparse_rc(sparse_rc&& other)
    {   swap(other); }
    //
    // destructor
    ~sparse_rc(void)
    { }
    //
    // sizing constructor
    // Eigen vector is ambiguous for row_(0), col_(0) so use default ctor
    sparse_rc(size_t nr, size_t nc, size_t nnz)
    : nr_(nr), nc_(nc), nnz_(nnz)
    {   row_.resize(nnz);
        col_.resize(nnz);
    }
    //
    // copy constructor
    sparse_rc(const sparse_rc& other)
    :
    nr_(other.nr_)   ,
    nc_(other.nc_)   ,
    nnz_(other.nnz_) ,
    row_(other.row_) ,
    col_(other.col_)
    { }
    //
    // assignment
    void operator=(const sparse_rc& other)
    {   nr_  = other.nr_;
        nc_  = other.nc_;
        nnz_ = other.nnz_;
        // simple vector assignment requires vectors to have same size
        row_.resize(nnz_);
        col_.resize(nnz_);
        row_ = other.row_;
        col_ = other.col_;
    }
    //
    // swap
    void swap(sparse_rc& other)
    {   std::swap( nr_ , other.nr_ );
        std::swap( nc_ , other.nc_ );
        std::swap( nnz_ , other.nnz_ );
        //
        row_.swap( other.row_ );
        col_.swap( other.col_ );
    }
    //
    // move semantics assignment
    void operator=(sparse_rc&& other)
    {   swap(other); }
    //
    // equality
    bool operator==(const sparse_rc& other) const
    {   bool result = true;
        result &= nr_  == other.nr_;
        result &= nc_  == other.nc_;
        result &= nnz_ == other.nnz_;
        if( ! result )
            return result;
        //
        SizeVector this_order  = row_major();
        SizeVector other_order = other.row_major();
        for(size_t k = 0; k < nnz_; ++k)
        {   size_t this_k  = this_order[k];
            size_t other_k = other_order[k];
            result &= row_[this_k] == other.row_[other_k];
            result &= col_[this_k] == other.col_[other_k];
        }
        return result;
    }
    //
    // resize
    void resize(size_t nr, size_t nc, size_t nnz)
    {   nr_ = nr;
        nc_ = nc;
        nnz_ = nnz;
        row_.resize(nnz);
        col_.resize(nnz);
    }
    //
    // set row and column for a possibly non-zero element
    void set(size_t k, size_t r, size_t c)
    {   CPPAD_ASSERT_KNOWN(
            k < nnz_,
            "The index k is not less than nnz in sparse_rc::set"
        );
        CPPAD_ASSERT_KNOWN(
            r < nr_,
            "The index r is not less than nr in sparse_rc::set"
        );
        CPPAD_ASSERT_KNOWN(
            c < nc_,
            "The index c is to not less than nc in sparse_rc::set"
        );
        row_[k] = r;
        col_[k] = c;
        //
    }
    //
    // push_back
    void push_back(size_t r, size_t c)
    {   CPPAD_ASSERT_KNOWN(
            r < nr_,
            "The index r is not less than nr in sparse_rc::push_back"
        );
        CPPAD_ASSERT_KNOWN(
            c < nc_,
            "The index c is to not less than nc in sparse_rc::push_back"
        );
        row_.push_back(r);
        col_.push_back(c);
        ++nnz_;
        CPPAD_ASSERT_UNKNOWN( row_.size() == nnz_ );
        CPPAD_ASSERT_UNKNOWN( col_.size() == nnz_ );
    }
    //
    // number of rows in matrix
    size_t nr(void) const
    {   return nr_; }
    //
    // number of columns in matrix
    size_t nc(void) const
    {   return nc_; }
    //
    // number of possibly non-zero elements in matrix
    size_t nnz(void) const
    {   return nnz_; }
    //
    // row indices
    const SizeVector& row(void) const
    {   return row_; }
    //
    // column indices
    const SizeVector& col(void) const
    {   return col_; }
    //
    // row-major order
    SizeVector row_major(void) const
    {   SizeVector keys(nnz_), row_major(nnz_);
        for(size_t k = 0; k < nnz_; k++)
        {   CPPAD_ASSERT_UNKNOWN( row_[k] < nr_ );
            keys[k] = row_[k] * nc_ + col_[k];
        }
        index_sort(keys, row_major);
# ifndef NDEBUG
        for(size_t ell = 0; ell + 1 < nnz_; ell++)
        {   size_t k  = row_major[ ell ];
            size_t kp = row_major[ ell + 1 ];
            CPPAD_ASSERT_KNOWN(
                row_[k] != row_[kp] || col_[k] != col_[kp],
                "sparse_rc: row_major: duplicate entry in this pattern"
            );
            CPPAD_ASSERT_UNKNOWN(
                row_[k]<row_[kp] || (row_[k]==row_[kp] && col_[k]<col_[kp])
            );
        }
# endif
        return row_major;
    }
    //
    // column-major indices
    SizeVector col_major(void) const
    {   SizeVector keys(nnz_), col_major(nnz_);
        for(size_t k = 0; k < nnz_; k++)
        {   CPPAD_ASSERT_UNKNOWN( col_[k] < nc_ );
            keys[k] = col_[k] * nr_ + row_[k];
        }
        index_sort(keys, col_major);
# ifndef NDEBUG
        for(size_t ell = 0; ell + 1 < nnz_; ell++)
        {   size_t k  = col_major[ ell ];
            size_t kp = col_major[ ell + 1 ];
            CPPAD_ASSERT_KNOWN(
                col_[k] != col_[kp] || row_[k] != row_[kp],
                "sparse_rc: col_major: duplicate entry in this pattern"
            );
            CPPAD_ASSERT_UNKNOWN(
                col_[k]<col_[kp] || (col_[k]==col_[kp] && row_[k]<row_[kp])
            );
        }
# endif
        return col_major;
    }
};
//
// output
template <class SizeVector>
std::ostream& operator << (
    std::ostream&                       os      ,
    const CppAD::sparse_rc<SizeVector>& pattern )
{   size_t nnz = pattern.nnz();
    if( nnz == 0 )
    {   os << "{ }";
        return os;
    }
    const SizeVector& row       = pattern.row();
    const SizeVector& col       = pattern.col();
    SizeVector        row_major = pattern.row_major();
    //
    // k, r, c
    size_t k = 0;
    size_t r = row[ row_major[k] ];
    size_t c = col[ row_major[k] ];
    //
    // os
    os << "{\nrow = " << r << ", col = " << c;
    while(++k < nnz )
    {   bool new_row = r != row[ row_major[k] ];
        r = row[ row_major[k] ];
        c = col[ row_major[k] ];
        if( new_row )
            os << "\nrow = " << r << ", col = " << c;
        else
            os << ", " << c;
    }
    os << "\n}";
    //
    return os;
}
} // END_CPPAD_NAMESPACE

# endif
