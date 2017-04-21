#include <iostream>
#include <bandicoot>

using namespace std;
using namespace bcoot;

// bandicoot_runtime_t bandicoot_runtime;

int
main()
  {
  fmat A(4,5); A.fill(1);
  fmat B(4,5); B.fill(2);
  
  cout << "A.n_rows: " << A.n_rows << endl;
  cout << "A.n_cols: " << A.n_cols << endl;
  
  cout << endl;
  
  cout << A(1) << endl;
  cout << B(2) << endl;
  
  cout << endl;
  
  A(1)   = 2;
  A(2,2) = 3;
  
  B(1)   += 4;
  B(2,2) += 5;
  
  
  cout << endl;
  
  const fmat& AA = A;
  const fmat& BB = B;
  cout << AA(1) << endl;
  cout << BB(2) << endl;
  
  
  A.print("A:");
  B.print("B:");
  
  
  fmat C = A+B;
  
  C.print("C:");
  
  fmat D = A+B+C;
  
  D.print("D:");
  
  fmat P(8,9);  P.fill(0);
  fmat Q(6,5);  Q.fill(-1);
  
  for(uword i=0; i<P.n_elem; ++i)
    {
    P(i) = i+1;
    }
  
  P.print("P:");
  Q.print("Q:");
  
  //P.get_submatrix(Q, 1, 0, 3, 2);
  P.get_submatrix(Q, 0, 1, P.n_rows-1, P.n_cols-1);
  
  Q.print("Q:");
  
  fmat R(8,9); R.fill(-1);
  fmat S(6,5); S.fill(-1);
  
  for(uword i=0; i<S.n_elem; ++i)
    {
    S(i) = i+1;
    }
  
  R.print("R:");
  S.print("S:");
  
  R.set_submatrix(S,1,2);
  
  R.print("R:");
  
  return 0;
  }
