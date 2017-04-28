#include <iostream>
#include <bandicoot>

using namespace std;
using namespace coot;

int
main(int argc, char** argv)
  {
  cout << "Bandicoot version " << coot_version::as_string() << endl;
  
  fmat A(1,1);
  A.fill(123);
  
  cout << as_scalar(A) << endl;
  
  fmat B(5,6);
  B.fill(234);
  
  cout << as_scalar( B(span(1),span(1)) ) << endl;
  
  
  cout << as_scalar( B(span(1),span(1)) + 345 ) << endl;
  
  return 0;
  }
