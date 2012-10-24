/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  integer_matrix.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


//---------------------------------------------------------------------
// file generated by notangle from integer_matrix.lw
// please debug or modify LEDA web file
// mails and bugs: leda@mpi-sb.mpg.de
// based on LEDA architecture by S. Naeher, C. Uhrig
// coding: K. Mehlhorn, M. Seel
//---------------------------------------------------------------------

#ifndef LEDA_INTEGER_MATRIX_DECL_H
#define LEDA_INTEGER_MATRIX_DECL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600258
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>
#include <LEDA/core/array.h>
#include <LEDA/numbers/integer_vector.h> 

#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/rational.h>

LEDA_BEGIN_NAMESPACE


/*{\Msubst
RTINT integer
}*/

typedef integer RTINT;

// #define LA_SELFTEST

  
  /*{\Manpage {integer_matrix} {} {Matrices with Integer Entries}}*/

  class __exportC integer_matrix 
  { 

  /*{\Mdefinition

  An instance of data type |integer_matrix| is a matrix of variables
  of type |integer|, the so called ring type. The arithmetic type
  |integer| is required to behave like integers in the mathematical sense.

  The types |integer_matrix| and |integer_vector| together realize many functions of
  basic linear algebra.  All functions on integer matrices compute the
  exact result, i.e., there is no rounding error.  Most functions of
  linear algebra are \emph{checkable}, i.e., the programs can be asked
  for a proof that their output is correct. For example, if the linear
  system solver declares a linear system $A x = b$ unsolvable it also
  returns a vector $c$ such that $c^T A = 0$ and $c^T b \neq 0$.  All
  internal correctness checks can be switched on by the flag
  \texttt{LA\_SELFTEST}. Preconditions are checked by default and can
  be switched off by the compile flag \texttt{LEDA\_CHECKING\_OFF}.}*/

    integer_vector** v; 
    int  d1; 
    int  d2; 

    RTINT&  elem(int i, int j) const { return v[i]->v[j]; }
    
    inline void allocate_mat_space(integer_vector** & vi, int d)
    {
    /* We use this procedure to allocate memory. We use the faster LEDA
       memory allocation scheme. There we first get an appropriate piece
       of memory from the LEDA memory manager and then initialize each
       cell by an inplace new. */

      vi = LEDA_NEW_VECTOR(integer_vector*,d);
      for(integer_vector** p = vi+d-1; p >= vi; p--)  new(p) integer_vector*; 
    }

    inline void deallocate_mat_space(integer_vector** vi, int d)
    {
    /* we use this procedure to deallocate memory. We have to deallocate
       memory from the LEDA memory space.  */

      LEDA_DEL_VECTOR(vi);
      vi = nil;
    }

    inline void check_dimensions(const integer_matrix& mat) const
    { 
      LEDA_PRECOND((d1 == mat.d1 && d2 == mat.d2), 
      "integer_matrix::check_dimensions: incompatible matrix types.") 
    }




  public:

  typedef RTINT          value_type;
  typedef integer_vector vector_type;

  /*{\Mcreation M 4}*/

  integer_matrix(int n, int m); 
  /*{\Mcreate creates an instance |\Mvar| of type |\Mname| of 
              dimension $n \times m$.}*/

  explicit

  integer_matrix(int n = 0); 
  /*{\Mcreate creates an instance |\Mvar| of type |\Mname| of
              dimension $n \times n$. }*/

  integer_matrix(const array< integer_vector >& A); 
  /*{\Mcreate creates an instance |\Mvar| of type |\Mname|. Let $A$
              be an array of $m$ column - vectors of common dimension $n$. 
              |\Mvar| is initialized to an $n \times m$ matrix with the 
              columns as specified by $A$. }*/


  integer_matrix(const integer_matrix&); 


  integer_matrix(const integer_vector&); 


  integer_matrix(int, int, RTINT**); 


  integer_matrix& operator=(const integer_matrix&); 


  ~integer_matrix(); 


  static integer_matrix identity(int n);
  /*{\Mstatic returns an identity matrix of dimension $n$.}*/


  /*{\Moperations 5.0 3}*/

  int     dim1()  const   {   return d1; }
  /*{\Mop  returns $n$, the number of rows of |\Mvar|. }*/

  int     dim2()  const  {  return d2; }
  /*{\Mop  returns $m$, the number of columns of |\Mvar|. }*/

  integer_vector& row(int i) const
  { 
    LEDA_OPT_PRECOND((0<=i && i<d1), "integer_matrix::row: index out of range.")
    return *v[i]; 
  }
  /*{\Mop  returns the $i$-th row of |\Mvar| (an $m$ - vector).\\
           \precond  $0 \le i \le n - 1$. }*/

  integer_vector  col(int i) const; 
  /*{\Mop  returns the $i$-th column of |\Mvar| (an $n$ - vector).\\
           \precond  $0 \le i \le m - 1$. }*/

  integer_vector to_vector() const; 
  friend __exportF integer_vector to_vector(const integer_matrix&); 


  integer_vector& operator[](int i) const  
  { 
    LEDA_OPT_PRECOND((0<=i && i<d1), "integer_matrix::operator[]: \
    index out of range.")
    return row(i); 
  }

  RTINT& operator()(int i, int j)
  { 
    LEDA_OPT_PRECOND((0<=i && i<d1), "integer_matrix::operator(): \
    row index out of range.")
    LEDA_OPT_PRECOND((0<=j && j<d2), "integer_matrix::operator(): \
    column index out of range.") 
    return elem(i,j); 
  }
  /*{\Mfunop returns $M_{ i,j }$. \\
             \precond $0\le i\le n - 1$ and $0\le j\le m - 1$. }*/

  RTINT  operator()(int i, int j) const
  { 
    LEDA_OPT_PRECOND((0<=i && i<d1), "integer_matrix::operator(): \
    row index out of range.")
    LEDA_OPT_PRECOND((0<=j && j<d2), "integer_matrix::operator(): \
    column index out of range.")
    return elem(i,j); 
  }

  int     operator==(const integer_matrix&)    const; 
  int     operator!=(const integer_matrix& x)  const 
  { return !(*this == x); }


  /*{\Mtext {\bf Arithmetic Operators}}*/
  /*{\Mtext
  \settowidth{\typewidth}{|integer_matrixmmmm|}
  \addtolength{\typewidth}{\colsep}
  \setlength{\callwidth}{3cm}
  \computewidths
  }*/

  integer_matrix operator+ (const integer_matrix& M1); 
  /*{\Mbinop Addition. \\
             \precond \\
             |\Mvar|.dim1() == $M1$.dim1() and 
             |\Mvar|.dim2() == $M1$.dim2(). }*/


  integer_matrix operator- (const integer_matrix& M1); 
  /*{\Mbinop Subtraction. \\
             \precond \\
             |\Mvar|.dim1() == $M1$.dim1() and
             |\Mvar|.dim2() == $M1$.dim2(). }*/

  integer_matrix operator-(); // unary


  integer_matrix& operator-=(const integer_matrix&); 


  integer_matrix& operator+=(const integer_matrix&); 


  integer_matrix 
  operator*(const integer_matrix& M1) const; 
  /*{\Mbinop Multiplication. \\
             \precond \\ |\Mvar|.dim2() == $M1$.dim1(). }*/


  integer_vector 
  operator*(const integer_vector& vec) const
  {  return ((*this) * integer_matrix(vec)).to_vector(); }
  /*{\Mbinop  Multiplication with vector.\\
              \precond \\ |\Mvar|.dim2() == $vec$.dim(). }*/

  integer_matrix compmul(const RTINT& x) const; 


  friend __exportF integer_matrix 
  operator*(const integer_matrix& M, const RTINT& x)
  { return M.compmul(x); }
  /*{\Mbinopfunc Multiplication of every entry with integer |x|. }*/

  friend __exportF integer_matrix 
  operator*(const integer_matrix& M, int x)
  { return M.compmul(x); }

  friend __exportF integer_matrix 
  operator*(const RTINT& x, const integer_matrix& M)
  { return M.compmul(x); }
  /*{\Mbinopfunc Multiplication of every entry with integer |x|. }*/

  friend __exportF integer_matrix 
  operator*(int x, const integer_matrix& M)
  { return M.compmul(x); }


  /*{\Mtext {\bf Non-Member Functions}}*/


  friend __exportF integer_matrix 
  transpose(const integer_matrix& M); 
  /*{\Mfunc  returns  $M^T$ ($m\times n$ - matrix). }*/

  friend __exportF  integer_matrix 
  inverse(const integer_matrix& M, RTINT & D)
  /*{\Mfuncl  returns the inverse matrix of |\Mvar|. More precisely, $1/D$ 
              times the matrix returned is the inverse of |\Mvar|.\\
              \precond  determinant(|\Mvar|) $\not= 0$. }*/
  { 
    integer_matrix result; 
    integer_vector c; 
    if (!inverse(M,result,D,c)) 
      LEDA_EXCEPTION(1,"integer_matrix::inverse: \
  matrix is singular."); 
    return result;
  }

  /*{\Mtext
  \settowidth{\typewidth}{|ostream&mmmm|}
  \addtolength{\typewidth}{\colsep}
  \settowidth{\callwidth}{inverse}
  \computewidths
  }*/

  friend __exportF bool 
  inverse(const integer_matrix& M, 
          integer_matrix & inverse,
          RTINT & D, integer_vector & c); 
  /*{\Mfuncl  determines whether |\Mvar| has an inverse. It also computes 
               either the inverse as $(1/D) \cdot |inverse|$ or a vector $c$ 
               such that $c^T \cdot M = 0 $.  }*/

  friend __exportF RTINT 
  determinant(const integer_matrix& M, 
              integer_matrix & L,
              integer_matrix & U, 
              array<int> & q, 
              integer_vector & c);
  /*{\Mfuncl returns the determinant $D$ of |\Mvar| and sufficient information 
              to verify that the value of the determinant is correct. If 
              the determinant is zero then $c$ is a vector such that 
              $c^T \cdot M = 0$. If the determinant is non-zero then $L$ 
              and $U$ are lower and upper diagonal matrices respectively 
              and $q$ encodes a permutation matrix $Q$ with $Q(i,j) = 1$ 
              iff $i = q(j)$ such that $L \cdot M \cdot Q = U$, 
              $L(0,0) = 1$, $L(i,i) = U(i - 1,i - 1)$ for all $i$, 
              $1 \le i < n$, and $D = s \cdot U(n - 1,n - 1)$ where $s$ is 
              the determinant of $Q$. \\
              \precond  |\Mvar| is quadratic. }*/

  friend __exportF bool 
  verify_determinant(const integer_matrix& M, RTINT D,
                     integer_matrix & L, integer_matrix & U, 
                     array<int> q, integer_vector & c);
  /*{\Mfuncl verifies the conditions stated above. }*/

  friend __exportF  RTINT determinant(const integer_matrix& M); 
  /*{\Mfuncl  returns the determinant of |\Mvar|.\\
              \precond  |\Mvar| is quadratic. }*/

  friend __exportF  int sign_of_determinant(const integer_matrix& M); 
  /*{\Mfuncl  returns the sign of the determinant of |\Mvar|.\\
              \precond  |\Mvar| is quadratic. }*/

  bool gauss_solver(
       const integer_vector& b,
       integer_vector& x, 
       RTINT& D, 
       integer_matrix& spanning_vectors, 
       integer_vector& c) const;
  /* we have to package the gauss linear solver into a member
     as we have g++ binding problems with friends */

  friend __exportF bool 
  linear_solver(const integer_matrix& M, 
                const integer_vector& b,
                integer_vector& x, RTINT& D, 
                integer_matrix& spanning_vectors, 
                integer_vector& c) 
  /*{\Mfuncl determines the complete solution space of the linear system 
             $M\cdot x = b$. If the system is unsolvable then 
             $c^T \cdot M = 0$ and $c^T \cdot b \not= 0$. 
             If the system is solvable then $(1/D) x$ is a solution, and 
             the columns of |spanning_vectors| are a maximal set of linearly 
             independent solutions to the corresponding homogeneous system.\\
             \precond |\Mvar|.dim1() == $b$.dim(). }*/
  { return M.gauss_solver(b,x,D,spanning_vectors,c); }

  friend __exportF bool 
  linear_solver(const integer_matrix & M, 
                const integer_vector& b, 
                integer_vector &x, RTINT & D, 
                integer_vector & c) 
  /*{\Mfuncl determines whether the linear system $M\cdot x = b$ is 
             solvable. If yes, then $(1/D) x$ is a solution, if not then 
             $c^T \cdot M = 0$ and $c^T \cdot b \not= 0$.\\
             \precond |\Mvar|.dim1() == $b$.dim(). }*/
  { 
    integer_matrix spanning_vectors; 
    return M.gauss_solver(b,x,D,spanning_vectors,c); 
  }

  friend __exportF bool 
  linear_solver(const integer_matrix & M, 
                const integer_vector& b,
                integer_vector &x, RTINT & D) 
  /*{\Mfuncl as above, but without the witness $c$ \\
             \precond |\Mvar|.dim1() == $b$.dim(). }*/
  { 
    integer_matrix spanning_vectors; 
    integer_vector c; 
    return M.gauss_solver(b,x,D,spanning_vectors,c); 
  }

  friend __exportF bool 
  is_solvable(const integer_matrix & M, 
              const integer_vector& b) 
  /*{\Mfuncl determines whether the system $M \cdot x = b$ is solvable \\
             \precond |\Mvar|.dim1() == $b$.dim(). }*/
  { 
    integer_vector x; 
    RTINT D; 
    integer_matrix spanning_vectors; 
    integer_vector c; 
    return M.gauss_solver(b,x,D,spanning_vectors,c); 
  }

  friend __exportF bool 
  homogeneous_linear_solver(const integer_matrix & M, 
                            integer_vector &x); 
  /*{\Mfuncl determines whether the homogeneous linear system 
             $M\cdot x = 0$ has a non - trivial solution. If
             yes, then $x$ is such a solution. }*/

  friend __exportF int 
  homogeneous_linear_solver(const integer_matrix & M, 
                            integer_matrix& spanning_vecs); 
  /*{\Mfuncl determines the solution space of the homogeneous linear system 
             $M\cdot x = 0$. It returns the dimension of the solution space.
             Moreover the columns of |spanning_vecs| span the solution space. }*/

  friend __exportF void 
  independent_columns(const integer_matrix & M, 
                      array<int>& columns); 
  /*{\Mfuncl returns the indices of a maximal subset of independent 
             columns of |M|. The index range of |columns| starts
             at |0|. }*/

  friend __exportF int rank(const integer_matrix & M); 
  /*{\Mfuncl returns the rank of matrix |\Mvar| }*/

  friend __exportF ostream& operator<<(ostream& O, const integer_matrix& M);
  /*{\Mbinopfunc writes matrix |\Mvar| row by row to the output stream $O$.}*/

  friend __exportF istream& operator>>(istream& I, integer_matrix& M);
  /*{\Mbinopfunc reads matrix |\Mvar| row by row from the input stream $I$.}*/



  static int cmp(const integer_matrix& M1, const integer_matrix& M2);


  /*{\Mimplementation 

  The datatype |\Mname| is implemented by two-dimensional
  arrays of variables of type |integer|.  Operations |determinant|, |inverse|,
  |linear_solver|, and |rank| take time $O(n^3)$, |column| takes time
  $O(n)$, |row|, |dim1|, |dim2|, take constant time, and all
  other operations take time $O(nm)$.  The space requirement is $O(nm)$.

  All functions on integer matrices compute the exact result, i.e.,
  there is no rounding error. The implemenation follows a proposal of
  J. Edmonds (J. Edmonds, Systems of distinct representatives and linear
  algebra, Journal of Research of the Bureau of National Standards, (B),
  71, 241 - 245). Most functions of linear algebra are { \em checkable
  }, i.e., the programs can be asked for a proof that their output is
  correct. For example, if the linear system solver declares a linear
  system $A x = b$ unsolvable it also returns a vector $c$ such that
  $c^T A = 0$ and $c^T b \not= 0$.  }*/ 

  };

  inline int compare(const integer_matrix& M1, 
                     const integer_matrix& M2) 
  { return integer_matrix::cmp(M1,M2); }


extern __exportF RTINT determinant(const integer_matrix&, integer_matrix&,
integer_matrix&, array<int>&, integer_vector&);
// this prototyp is necessary as g++ complains heavily without :-)



#if LEDA_ROOT_INCL_ID == 600258
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif 


