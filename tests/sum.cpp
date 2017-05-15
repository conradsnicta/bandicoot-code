#include <iostream>
#include <bandicoot>

using namespace std;
using namespace coot;

// coot_runtime_t coot_runtime;

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
  B.print("B:");
  C.print("C:");
  
  cout << "----" << endl;
  
  sum(A).print("sum(A):");
  
  sum(A,0).print("sum(A,0):");

  sum(A,1).print("sum(A,1):");
  
  cout << "sum(sum(A)): " << sum(sum(A)) << endl;
  
  cout << "----" << endl;
  
  sum(B).print("sum(B):");
  
  sum(B,0).print("sum(B,0):");

  sum(B,1).print("sum(B,1):");
  
  cout << "sum(sum(B)): " << sum(sum(B)) << endl;
  
  cout << "----" << endl;
  
  sum(C).print("sum(C):");
  
  sum(C,0).print("sum(C,0):");

  sum(C,1).print("sum(C,1):");
  
  cout << "sum(sum(C)): " << sum(sum(C)) << endl;
  
  
  return 0;
  }
