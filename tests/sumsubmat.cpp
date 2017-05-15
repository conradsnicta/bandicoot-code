#include <iostream>
#include <bandicoot>

using namespace std;
using namespace coot;

// coot_runtime_t coot_runtime;


template<typename eT>
void
run_sum(const Mat<eT>& X, const uword dim)
  {
  sum(X,dim).print("sum(X,dim):");
  
  sum(X(span::all,span::all),dim).print("sum(X(span::all,span::all),dim):");
  
  sum(X(span(1,X.n_rows-2),span(1,X.n_cols-2)),dim).print("sum(X(span(1,X.n_rows-2),span(1,X.n_cols-2)),dim):");
  }



int
main()
  {
  fmat A(8,9);  A.fill(1);
  fmat B(5,5);
  fmat C(9,9);  C.fill(-1);
  
  for(uword i=0; i<B.n_elem; ++i)
    {
    B(i) = i+1;
    }
  
  A.print("A:");
  
  cout << endl;
  cout << "dim: 0" << endl;
  run_sum(A,0);
  
  cout << endl;
  cout << "dim: 1" << endl;
  run_sum(A,1);
  
  cout << endl;
  cout << "----" << endl;
  cout << endl;
  
  
  
  B.print("B:");
  
  cout << endl;
  cout << "dim: 0" << endl;
  run_sum(B,0);
  
  cout << endl;
  cout << "dim: 1" << endl;
  run_sum(B,1);
  
  cout << endl;
  cout << "----" << endl;
  cout << endl;
  
  
  
  C.print("C:");
  
  cout << endl;
  cout << "dim: 0" << endl;
  run_sum(C,0);
  
  cout << endl;
  cout << "dim: 1" << endl;
  run_sum(C,1);
  
  
  
  
  return 0;
  }
