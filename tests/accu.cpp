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
  
  const float result_A = accu(A);
  cout << "result_A: " << result_A << endl;

  cout << "----" << endl;
  
  const float result_B = accu(B);
  cout << "result_B: " << result_B << endl;
  
  cout << "----" << endl;
  
  const float result_C = accu(C);
  cout << "result_C: " << result_C << endl;
  
  return 0;
  }
