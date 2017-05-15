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
  
  cout << "accu(A): " << accu(A) << endl;
  
  cout << "accu(A(span::all,span::all)): " << accu(A(span::all,span::all)) << endl;
  
  cout << "accu(A(span(1,A.n_rows-2),span(1,A.n_cols-2))): " << accu(A(span(1,A.n_rows-2),span(1,A.n_cols-2))) << endl;

  cout << "----" << endl;
  


  cout << "accu(B): " << accu(B) << endl;
  
  cout << "accu(B(span::all,span::all)): " << accu(B(span::all,span::all)) << endl;
  
  cout << "accu(B(span(1,B.n_rows-2),span(1,B.n_cols-2))): " << accu(B(span(1,B.n_rows-2),span(1,B.n_cols-2))) << endl;

  cout << "----" << endl;



  cout << "accu(C): " << accu(C) << endl;
  
  cout << "accu(C(span::all,span::all)): " << accu(C(span::all,span::all)) << endl;
  
  cout << "accu(C(span(1,C.n_rows-2),span(1,C.n_cols-2))): " << accu(C(span(1,C.n_rows-2),span(1,C.n_cols-2))) << endl;

  cout << "----" << endl;


  return 0;
  }
